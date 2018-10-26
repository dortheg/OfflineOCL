/*******************************************************************************
 * Copyright (C) 2016 Vetle W. Ingeberg                                        *
 * Author: Vetle Wegner Ingeberg, v.w.ingeberg@fys.uio.no                      *
 *                                                                             *
 * --------------------------------------------------------------------------- *
 * This program is free software; you can redistribute it and/or modify it     *
 * under the terms of the GNU General Public License as published by the       *
 * Free Software Foundation; either version 3 of the license, or (at your      *
 * option) any later version.                                                  *
 *                                                                             *
 * This program is distributed in the hope that it will be useful, but         *
 * WITHOUT ANY WARRANTY; without even the implied warranty of                  *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General   *
 * Public License for more details.                                            *
 *                                                                             *
 * You should have recived a copy of the GNU General Public License along with *
 * the program. If not, see <http://www.gnu.org/licenses/>.                    *
 *                                                                             *
 *******************************************************************************/

#include "UserSort.h"

#include "DefineFile.h"

#include "WordBuffer.h"
#include "Histogram1D.h"
#include "Histogram2D.h"
#include "Histograms.h"
#include "Event.h"


#include <string>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <fstream>

#include <stdint.h>
#include <stdlib.h>

//If calculate with PPACS
#define FISSION 1
//If gating on specified energies in ede
const int GATING = 0;
//If cut in energy_particle_time, as seen in book p 152-153
const int GATING_EDE_TIME = 0;

static bool set_par(Parameters& parameters, std::istream& ipar,
                    const std::string& name, int size)
{
    Parameter* p = parameters.Find(name);
    if( !p )
        return false;

    std::vector<Parameter::param_t> values;
    float tmp;
    for(int i=0; i<size; ++i) {
        tmp = 0;
        ipar >> tmp;
        values.push_back(tmp);
    }
    p->Set( values );

    return true;
}

static bool set_gainshift(Parameters& parameters, std::istream& ipar)
{
    bool p1 = set_par(parameters, ipar, "gain_labr",    NUM_LABR_DETECTORS );
    bool p2 = set_par(parameters, ipar, "shift_labr",   NUM_LABR_DETECTORS );
    bool p3 = set_par(parameters, ipar, "gain_de", NUM_SI_DE_DET );
    bool p4 = set_par(parameters, ipar, "shift_de", NUM_SI_DE_DET );
    bool p5 = set_par(parameters, ipar, "gain_e", NUM_SI_DE_DET ); //Changed to DE, to get 64 coeff
    bool p6 = set_par(parameters, ipar, "shift_e", NUM_SI_DE_DET ); //Changed to DE, to get 64 coeff
    bool p7 = set_par(parameters, ipar, "shift_time_labr", NUM_LABR_DETECTORS );
    bool p8 = set_par(parameters, ipar, "shift_time_de", NUM_SI_DE_DET );
    bool p9 = set_par(parameters, ipar, "shift_time_e", NUM_SI_E_DET );
    bool p10 = set_par(parameters, ipar, "shift_time_ppac", NUM_PPAC );
    return (p1 && p2 && p3 && p4 && p5 && p6 && p7 && p8 && p9 && p10);
}

UserSort::UserSort()
    : gain_labr( GetParameters(), "gain_labr", NUM_LABR_DETECTORS, 1)
    , shift_labr( GetParameters(), "shift_labr", NUM_LABR_DETECTORS, 0)
    , gain_dE( GetParameters(), "gain_de", NUM_SI_DE_DET, 1)
    , shift_dE( GetParameters(), "shift_de", NUM_SI_DE_DET, 0)
    , gain_E( GetParameters(), "gain_e", NUM_SI_DE_DET, 1) //Changed to DE, to get 64 coeff
    , shift_E( GetParameters(), "shift_e", NUM_SI_DE_DET, 0) //Changed to DE, to get 64 coeff
    , shift_time_labr( GetParameters(), "shift_time_labr", NUM_LABR_DETECTORS, 0)
    , shift_time_de( GetParameters(), "shift_time_de", NUM_SI_DE_DET, 0)
    , shift_time_e( GetParameters(), "shift_time_e", NUM_SI_E_DET, 0)
    , shift_time_ppac( GetParameters(), "shift_time_ppac", NUM_PPAC, 0)
    , ex_from_ede    ( GetParameters(), "ex_from_ede", NUM_SI_RINGS*3)
//    , ede_rect       ( GetParameters(), "ede_rect", 4         )
    , thick_range    ( GetParameters(), "thick_range", 2      )
    , labr_time_cuts  ( GetParameters(), "labr_time_cuts", 2*2  )
    , ppac_time_cuts ( GetParameters(), "ppac_time_cuts", 2*2 )
    , ede_time_cuts ( GetParameters(), "ede_time_cuts", 2*2 )
{
}


double UserSort::CalibrateE(const word_t &w) const
{
    DetectorInfo_t info = GetDetector(w.address);
    switch ( info.type ) {

    case labr : {
        return 1.00740*gain_labr[info.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_labr[info.detectorNum];
    }
    case deDet : {
        return gain_dE[info.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_dE[info.detectorNum];
    }
    case eDet : {
        return gain_E[info.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info.detectorNum];
    }
    case ppac : {
        return w.adcdata;
    }
    case rfchan : {
        return w.adcdata;
    }
    default:
        return w.adcdata;
    }
}


double UserSort::CalibrateOnlyE(const word_t &w, const word_t &de_strip) const
{
    DetectorInfo_t info_e = GetDetector(w.address);
    DetectorInfo_t info_de = GetDetector(de_strip.address);

    if (info_e.type != eDet && info_de.type != deDet){
        std::cout << "CalibrateE: Type Error" << std::endl;
        return w.adcdata;
    }
    return gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum];

}


double UserSort::CalcTimediff(const word_t &start, const word_t &stop) const
{
    // First we fetch the correct shift parameters.
    double start_shift, stop_shift;

    DetectorInfo_t info_start = GetDetector(start.address);
    DetectorInfo_t info_stop = GetDetector(stop.address);

    switch ( info_start.type ){
        case labr : {
            start_shift = shift_time_labr[info_start.detectorNum];
            break;
        }
        case deDet : {
            start_shift = shift_time_de[info_start.detectorNum];
            break;
        }
        case eDet : {
            start_shift = shift_time_e[info_start.detectorNum];
            break;
        }
        case ppac : {
            start_shift = shift_time_ppac[info_start.detectorNum];
            break;
        }
        default : {
            start_shift = 0;
            break;
        }
    }

    switch ( info_stop.type ){
        case labr : {
            stop_shift = shift_time_labr[info_stop.detectorNum];
            break;
        }
        case deDet : {
            stop_shift = shift_time_de[info_stop.detectorNum];
            break;
        }
        case eDet : {
            stop_shift = shift_time_e[info_stop.detectorNum];
            break;
        }
        case ppac : {
            stop_shift = shift_time_ppac[info_stop.detectorNum];
            break;
        }
        default : {
            stop_shift = 0;
            break;
        }
    }

    // 'Coarse' time difference.
    int64_t diff_coarse = stop.timestamp - start.timestamp;

    // 'Fine' time difference
    double diff_fine = stop.cfdcorr -start.cfdcorr;

    // Actual time difference.
    double diff = diff_coarse + diff_fine;

    // Now we add shift of stop time and subtract shift of start time.
    diff += (stop_shift - start_shift);

    return diff;

}

double UserSort::CalcAbsTime(const word_t &start) const
{
    // First we fetch the correct shift parameters.
    double start_shift;

    DetectorInfo_t info_start = GetDetector(start.address);

    switch ( info_start.type ){
        case labr : {
            start_shift = shift_time_labr[info_start.detectorNum];
            break;
        }
        case deDet : {
            start_shift = shift_time_de[info_start.detectorNum];
            break;
        }
        case eDet : {
            start_shift = shift_time_e[info_start.detectorNum];
            break;
        }
        case ppac : {
            start_shift = shift_time_ppac[info_start.detectorNum];
            break;
        }
        default : {
            start_shift = 0;
            break;
        }
    }

    // 'Coarse' time difference.
    int64_t time_abs = start.timestamp;

    return time_abs;

}


bool UserSort::UserCommand(const std::string &cmd)
{
    std::istringstream icmd(cmd.c_str());

    std::string name, tmp;
    icmd >> name;

    if ( name == "Gain" ) {
        icmd >> tmp;
        if ( tmp == "file" ) {
            std::string filename;
            icmd >> filename;
            std::ifstream gainfile(filename.c_str());
            if ( !set_gainshift(GetParameters(), gainfile) ){
                std::cerr << __PRETTY_FUNCTION__ << ", gain file: error reading '" << filename << "'" << std::endl;
                return false;
            }
        } else if ( tmp=="data" ){
            if ( !set_gainshift(GetParameters(), icmd) ){
                std::cerr << __PRETTY_FUNCTION__ << ", gain data: error reading calibration data" << std::endl;
                return false;
            }
        } else {
            std::cerr << __PRETTY_FUNCTION__ << ", gain: Expected 'file' or 'data', not '" << tmp << "'" << std::endl;
        }
    } else if ( name == "parameter" ){
        return GetParameters().SetAll(icmd);
    } else {
        return false;
    }
    return true;
}


void UserSort::CreateSpectra()
{
    char tmp[1024], tmp2[1024];;

    // Allocating the LaBr 'singles' spectra
    for (int i = 0 ; i < NUM_LABR_DETECTORS ; ++i){

        // Create energy spectra
        sprintf(tmp, "energy_raw_labr_%02d", i+1);
        energy_labr_raw[i] = Spec(tmp, tmp, 32768, 0, 32768, "Energy [ch]");

        sprintf(tmp, "energy_labr_%02d", i+1);
        energy_labr[i] = Spec(tmp, tmp, 10000, 0, 10000, "Energy [keV]");

        sprintf(tmp, "energy_time_labr_%02d", i+1);
        energy_time_labr[i] = Mat(tmp, tmp, 1000, 0, 16000, "LaBr energy [keV]", 2000, -100, 100, "Time difference [ns]");

        sprintf(tmp, "energy_time_labr_aboveSn_%02d", i+1);
        //Mat(tmp,tmp,nrbins,start,stop, name of xaxis, and then if 2D, again)
        energy_time_labr_above[i] = Mat(tmp, tmp, 1000, 0, 16000, "LaBr energy [keV]", 2000, -100, 100, "Time difference [ns]");
    }

    // Allocating the dE 'singles' spectra
    for (int i = 0 ; i < NUM_SI_DE_DET ; ++i){

        // Create energy spectra
        sprintf(tmp, "energy_raw_dE_%02d", i);
        energy_dE_raw[i] = Spec(tmp, tmp, 32768, 0, 32768, "Energy [ch]");

        sprintf(tmp, "energy_dE_%02d", i);
        energy_dE[i] = Spec(tmp, tmp, 10000, 0, 10000, "Energy [keV]");
    }

    // Allocating the E 'singles' spectra
    for (int i = 0 ; i < NUM_SI_E_DET ; ++i){

        // Create energy spectra
        sprintf(tmp, "energy_raw_E_%02d", i);
        energy_E_raw[i] = Spec(tmp, tmp, 32768, 0, 32768, "Energy [ch]");

        sprintf(tmp, "energy_E_%02d", i);
        energy_E[i] = Spec(tmp, tmp, 10000, 0, 10000, "Energy [keV]");
    }

    // Making all spectra that are indexed [NUM_SI_RINGS]
    for (int i = 0 ; i < NUM_SI_RINGS ; ++i){

        // e_de_time spectrum
        sprintf(tmp, "e_de_time_%02d", i);
        e_de_time[i] = Mat(tmp, tmp, 3000, -1500, 1500, "Time t_{dE} - t_{E} [ns]", NUM_SI_RINGS, 0, NUM_SI_RINGS, "Ring number");

        // Making all spectra indexed [NUM_SI_RINGS][NUM_SI_RINGS].
        for (int j = 0 ; j < NUM_SI_RINGS ; ++j){

            // Make the 'raw' ede spectrum.
            sprintf(tmp, "ede_raw_b%d_f%d", i, j);
            sprintf(tmp2, "E : DE raw, pad %d, ring %d", i, j);
            ede_raw[i][j] = Mat(tmp, tmp2, 2048, 0, 32768, "Back energy [ch]", 2048, 0, 32768, "Front energy [ch]");

            // Make 'calibrated' ede spectrum.
            sprintf(tmp, "ede_b%d_f%d", i, j);
            sprintf(tmp2, "E : DE calibrated, pad %d, ring %d", i, j);
            ede[i][j] = Mat(tmp, tmp2, 2000, 0, 20000, "Back energy [keV]", 500, 0, 5000, "Front energy [keV]");

            // Make total energy spectra.
            sprintf(tmp, "h_ede_b%d_f%d", i, j);
            sprintf(tmp2, "Total energy deposited, pad %d, ring %d", i, j);
            h_ede[i][j] = Spec(tmp, tmp2, 20000, 0, 20000, "Total energy deposited [keV]");


            // Make excitation spectra.
            sprintf(tmp, "h_ex_b%d_f%d", i, j);
            sprintf(tmp2, "Singles excitation spectrum, pad %d, ring %d", i, j);
            h_ex[i][j] = Spec(tmp, tmp2, 20000, 0, 20000, "Excitation energy [keV]");
        }
    }

    for (int i = 0 ; i < NUM_PPAC ; ++i){

        sprintf(tmp, "excitation_time_ppac_%d", i);
        sprintf(tmp2, "Excitation : ppac time, PPAC %d", i);
        excitation_time_ppac[i] = Mat(tmp, tmp2, 2000, 0, 10000, "Excitation energy [keV]", 2000, -100, 100, "t_{PPAC} - t_{#Delta E} [ns]");

        sprintf(tmp, "energy_time_ppac_%d", i);
        sprintf(tmp2, "LaBr energy : ppac time, PPAC %d", i);
        energy_time_ppac[i] = Mat(tmp, tmp2, 2000, 0, 10000, "LaBr energy [keV]", 2000, -100, 100, "t_{PPAC} - t_{LaBr} [ns]");
    }

    sprintf(tmp, "prompt_peak_movement");
    sprintf(tmp2, "Movement of prompt peak in energy spectrum");
    prompt_peak_movement = Mat(tmp, tmp2, 100, -50, 50, "t_{LaBr} - t_{dE ANY} [ns]", 100, 2900e9, 3200e9, "t_{Labr}");

    sprintf(tmp, "energy_time_e_de_all");
    sprintf(tmp2, "LaBr energy : e-de time diff, all");
    energy_time_e_de_all = Mat(tmp, tmp2, 5000, 0, 6000, "LaBr energy [keV]", 5000, 0, 300, "t_{DE} - t_{E} [ns]");

    sprintf(tmp, "energy_particle_time_e_de_all");
    sprintf(tmp2, "Particle energy : e-de time diff, all");
    energy_particle_time_e_de_all = Mat(tmp, tmp2, 5000, 0, 20000, "Particle energy [keV]", 5000, 0, 300, "t_{DE} - t_{E} [ns]");

    sprintf(tmp, "energy_E_particle_time_e_de_all");
    sprintf(tmp2, "Particle energy : e-de time diff, all");
    energy_E_particle_time_e_de_all = Mat(tmp, tmp2, 5000, 0, 20000, "Particle energy [keV]", 5000, 0, 300, "t_{DE} - t_{E} [ns]");


    sprintf(tmp, "energy_particle_time_e_de_all_gate");
    sprintf(tmp2, "Particle energy : e-de time diff, all, gated");
    energy_particle_time_e_de_all_gate = Mat(tmp, tmp2, 5000, 0, 20000, "Particle energy [keV]", 5000, 0, 300, "t_{DE} - t_{E} [ns]");

    sprintf(tmp, "filling_test");
    sprintf(tmp2, "Particle energy : e-de time diff, all, gated, test");
    filling_test = Mat(tmp, tmp2, 5000, 0, 20000, "Particle energy [keV]", 5000, 0, 300, "t_{DE} - t_{E} [ns]");


    //Dorthea attempt to create one spectrum with all labr
    sprintf(tmp, "energy_labr_all");
    energy_labr_all = Spec(tmp, tmp, 10000, 0, 10000, "Energy [keV]");

    // Time spectra (except those 'listed')
    sprintf(tmp, "de_align_time");
    sprintf(tmp2, "t_{dE} - t_{LaBr nr. 1}");
    de_align_time = Mat(tmp, tmp2, 5000, -1500, 1500, "t_{dE} - t_{LaBr nr. 1} [ns]", NUM_SI_DE_DET, 0, NUM_SI_DE_DET, "#Delta E detector id.");

    // Time spectra (except those 'listed')
    sprintf(tmp, "de_align_time_3");
    sprintf(tmp2, "t_{dE} - t_{LaBr nr. 3}");
    de_align_time_3 = Mat(tmp, tmp2, 5000, -1500, 1500, "t_{dE} - t_{LaBr nr. 3} [ns]", NUM_SI_DE_DET, 0, NUM_SI_DE_DET, "#Delta E detector id.");


    //Tried to write
    sprintf(tmp, "ppac_vs_dE_time");
    sprintf(tmp2, "t_{PPAC} - t_{dE ANY}");
    ppac_vs_dE_time = Mat(tmp, tmp2, 4000, -1500, 1500, "t_{PPAC 1} - t_{dE ANY} [ns]", NUM_PPAC, 0, NUM_PPAC, "PPAC id.");

    sprintf(tmp, "labr_align_time");
    sprintf(tmp2, "t_{LaBr} - t_{dE ANY}");
    labr_align_time = Mat(tmp, tmp2, 5000, -1500, 1500, "t_{LaBr} - t_{dE ANY} [ns]", NUM_LABR_DETECTORS, 0, NUM_LABR_DETECTORS, "LaBr detector id.");

    sprintf(tmp, "ppac_align_time");
    sprintf(tmp2, "t_{PPAC} - t_{LaBr nr. 1}");
    ppac_align_time = Mat(tmp, tmp2, 3100, -1500, 1500, "t_{PPAC} - t_{LaBr nr. 1} [ns]", NUM_PPAC, 0, NUM_PPAC, "PPAC id.");

    sprintf(tmp, "ppac_align_time_all");
    sprintf(tmp2, "t_{PPAC} - t_{LaBr ANY (?)}");
    ppac_align_time_all = Mat(tmp, tmp2, 3100, -1500, 1500, "t_{PPAC} - t_{LaBr} [ns]", NUM_PPAC, 0, NUM_PPAC, "PPAC id.");

    sprintf(tmp, "energy_time_labr_all");
    sprintf(tmp2, "E_{LaBr} : t_{LaBr} - t_{dE ANY}, all");
    energy_time_labr_all = Mat(tmp, tmp2, 5000, 0, 16000, "Energy LaBr [keV]", 2000, -50, 50, "t_{LaBr} - t_{DE} [ns]");

    sprintf(tmp, "ede_all");
    sprintf(tmp2, "E : DE, all");
    ede_all = Mat(tmp, tmp2, 10000, 0, 20000, "Back energy [keV]", 1000, 0, 5000, "Front energy [keV]");

    sprintf(tmp, "ede_all_doublepeak");
    sprintf(tmp2, "E : DE, all");
    ede_all_doublepeak = Mat(tmp, tmp2, 10000, 0, 20000, "Back energy [keV]", 1000, 0, 5000, "Front energy [keV]");

    sprintf(tmp, "ede_all_fission");
    sprintf(tmp2, "E : DE, all");
    ede_all_fission = Mat(tmp, tmp2, 10000, 0, 20000, "Back energy [keV]", 1000, 0, 5000, "Front energy [keV]");

    sprintf(tmp, "ede_all_nofission");
    sprintf(tmp2, "E : DE, all");
    ede_all_nofission = Mat(tmp, tmp2, 10000, 0, 20000, "Back energy [keV]", 1000, 0, 5000, "Front energy [keV]");


    sprintf(tmp, "ede_all_bg");
    sprintf(tmp2, "E : DE, all, background");
    ede_all_bg = Mat(tmp, tmp2, 10000, 0, 20000, "Back energy [keV]", 1000, 0, 5000, "Front energy [keV]");

    sprintf(tmp, "ede_all_except_pad7");
    sprintf(tmp2, "E : DE, all");
    ede_all_except_pad7 = Mat(tmp, tmp2, 10000, 0, 20000, "Back energy [keV]", 1000, 0, 5000, "Front energy [keV]");

    sprintf(tmp, "ede_gate");
    sprintf(tmp2, "E : DE, after particle gate");
    ede_gate = Mat(tmp, tmp2, 1000, 0, 20000, "Back energy [keV]", 250, 0, 5000, "Front energy [keV]");

    sprintf(tmp, "h_thick");
    sprintf(tmp2, "Apparent thickness of #Delta E");
    h_thick = Spec(tmp, tmp2, 3000, 0, 3000, "Apparent thickness [#mu m]");

    sprintf(tmp, "h_ede_all");
    sprintf(tmp2, "Total particle energy, all");
    h_ede_all = Spec(tmp, tmp2, 20000, 0, 20000, "Particle energy [keV]");

    sprintf(tmp, "h_ex_all");
    sprintf(tmp2, "Excitation energy, all");
    h_ex_all = Spec(tmp, tmp2, 20000, 0, 20000, "Excitation energy [keV]");

    sprintf(tmp, "exgam");
    exgam = Mat(tmp, tmp, 1500, 0, 15000, "LaBr [keV]", 1600, -1000, 15000, "Ex [keV]");

    sprintf(tmp, "exgam_bg");
    exgam_bg = Mat(tmp, tmp, 1500, 0, 15000, "LaBr [keV]", 1600, -1000, 15000, "Ex [keV]");

    sprintf(tmp, "exgam_ppac"); //alfna med fisjon
    exgam_ppac = Mat(tmp, tmp, 1500, 0, 15000, "LaBr [keV]", 1600, -1000, 15000, "Ex [keV]");

    sprintf(tmp, "exgam_ppac_bg");
    exgam_ppac_bg = Mat(tmp, tmp, 1500, 0, 15000, "LaBr [keV]", 1600, -1000, 15000, "Ex [keV]");

    sprintf(tmp, "exgam_veto_ppac"); //alfna uten fisjon
    exgam_veto_ppac = Mat(tmp, tmp, 1500, 0, 15000, "LaBr [keV]", 1600, -1000, 15000, "Ex [keV]");

    sprintf(tmp, "exgam_veto_ppac_bg");
    exgam_veto_ppac_bg = Mat(tmp, tmp, 1500, 0, 15000, "LaBr [keV]", 1600, -1000, 15000, "Ex [keV]");


    n_fail_e = 0;
    n_fail_de = 0;
    n_tot_e = 0;
    n_tot_de = 0;
    tot = 0;
}


bool UserSort::Sort(const Event &event) //det som sorterer
{
    int i, j;
    double energy;
    double tdiff;
    double tdiff_ede;

    n_tot_e += event.tot_Edet;
    n_tot_de += event.tot_dEdet;
    tot += 1;


    word_t de_words[256]; // List of dE hits from pads in front of the trigger E word.
    int n_de_words=0;

    // First fill some 'singles' spectra.
    for ( i = 0 ; i < NUM_LABR_DETECTORS ; ++i ){
        for ( j = 0 ; j < event.n_labr[i] ; ++j ){

            if(GATING==1){
                //not fill energy_labr_raw here
            }
            else if(GATING==0){
                energy_labr_raw[i]->Fill(event.w_labr[i][j].adcdata); //comment out this if gating on event with given energy
            }

            energy = CalibrateE(event.w_labr[i][j]);
            energy_labr[i]->Fill(energy);
            //Dorthea attempt at creating labr energy spectrum with all Labr
            energy_labr_all->Fill(energy);
        }
    }

    for ( i = 0 ; i < NUM_SI_DE_DET ; ++i ){
        for ( j = 0 ; j < event.n_dEdet[i] ; ++j ){
            energy_dE_raw[i]->Fill(event.w_dEdet[i][j].adcdata);
            energy = CalibrateE(event.w_dEdet[i][j]);
            energy_dE[i]->Fill(energy);
            if (event.w_dEdet[i][j].cfdfail > 0) // For 'statistical' purposes!
                ++n_fail_de;
        }
    }

    for ( i = 0 ; i < NUM_SI_E_DET ; ++i ){
        for ( j = 0 ; j < event.n_Edet[i] ; ++j ){
            energy_E_raw[i]->Fill(event.w_Edet[i][j].adcdata);
            energy = CalibrateE(event.w_Edet[i][j]);
            energy_E[i]->Fill(energy);
            if (event.w_Edet[i][j].cfdfail > 0) // For 'statistical' purposes!
                ++n_fail_e;
        }
    }

    // We know that DE addresses should be as following:
    // 0 - 7: With E address 0.
    // 8 - 15: With E address 1.
    // 16 - 23: With E address 2.
    // 24 - 31: With E address 3.
    // 32 - 39: With E address 4.
    // 40 - 47: With E address 5.
    // 48 - 55: With E address 6.
    // 56 - 63: With E address 7.

    for (i = 8*GetDetector(event.trigger.address).telNum ; i < 8*(GetDetector(event.trigger.address).telNum+1) ; ++i){
        for (j = 0 ; j < event.n_dEdet[i] ; ++j){

            if (n_de_words < 256)
                de_words[n_de_words++] = event.w_dEdet[i][j];
        }

    }


    // Check if only one dE detector of the trapezoidal has fired.
    if ( n_de_words == 1){

        word_t e_word = event.trigger;
        word_t de_word = de_words[0]; //noe skjer i dE-detector

        // The ring number and telescope number.
        int ring = GetDetector(de_word.address).detectorNum % 8; // Later we should define what we divide by somewhere else...
        int tel = GetDetector(e_word.address).telNum;

        tdiff = CalcTimediff(e_word, de_word);
        e_de_time[tel]->Fill(tdiff, ring);

        // Align the dE times...
        if(event.n_labr[2] == 1){
            //Check alignment against Labr03
            tdiff = CalcTimediff(event.w_labr[2][0], de_word);
            de_align_time_3->Fill(tdiff, GetDetector(de_word.address).detectorNum);
        }


        if ( event.n_labr[0] == 1){
            tdiff = CalcTimediff(event.w_labr[0][0], de_word);
            de_align_time->Fill(tdiff, GetDetector(de_word.address).detectorNum);

            for (int i = 0 ; i < NUM_PPAC ; ++i){
                for (int j = 0 ; j < event.n_ppac[i] ; ++j){
                    tdiff = CalcTimediff(event.w_labr[0][0], event.w_ppac[i][j]);
                    ppac_align_time->Fill(tdiff, i);
                    ppac_align_time_all->Fill(tdiff, i);
                }
            }
        }


        // Fill DE - E matrices.
        if(GATING==0){
            ede_raw[tel][ring]->Fill(e_word.adcdata, de_word.adcdata);
        }
        //If energy gate
        double E = e_word.adcdata;
        double DE = de_word.adcdata;
        double E1_E = 8150.0;
        double E2_E = 8300.0;
        double E1_DE = 1400.0;
        double E2_DE = 1900.0;
        if (E>E1_E && E<E2_E && DE>E1_DE &&DE<E2_DE && GATING==1){
            ede_raw[tel][ring]->Fill(E, DE);
        }

        double e_energy = CalibrateOnlyE(e_word, de_word);
        double de_energy = CalibrateE(de_word);

        ede[tel][ring]->Fill(e_energy, de_energy);

        // Seems like we may have some issues with the dE rings 6 & 7 (0-7). We will end our
        // sorting here if we have either 6 or 7.
        //if (ring == 6 || ring == 7)
        //    return true;

        if(tel!=7){
            ede_all_except_pad7->Fill(e_energy, de_energy);
        }

        //ede time cuts
        //Dorthea made
        tdiff_ede = CalcTimediff(e_word, de_word);

//        switch ( CheckTimeStatus(tdiff_ede, ede_time_cuts) ) {
//            //tdiff_ede: if prompt between e and de
//            case is_prompt : {
//                ede_all->Fill(e_energy, de_energy);

//                break;
//            }
//            case is_background : {
//                ede_all->Fill(e_energy, de_energy, -1);
//                break;
//            }
//            case ignore : {
//                break;
//            }
//        }



        ede_all->Fill(e_energy, de_energy);

        // Calculate 'apparent thickness'
        double thick = range.GetRange(e_energy + de_energy) - range.GetRange(e_energy);
        h_thick->Fill(thick);

        // Check if correct particle
        if ( thick >= thick_range[0] && thick <= thick_range[1] ){

            //ede_gate->Fill(e_energy, de_energy);

            // Calculate the particle energy.
            double e_tot = e_energy + de_energy;

            // Filling 'total particle energy' spectrum.
            h_ede[tel][ring]->Fill( e_tot );
            h_ede[tel][ring]->Fill( e_tot );
            h_ede_all->Fill( e_tot );

            //Dorthea made

            double y_upper = 0.0080361*e_tot + 175.7;
            //double y_lower = 0.01426418*e_tot + 37.7061778; //as in book page 150
            double y_lower_1 = 0.00826418*e_tot + 150.7061778; //very tight
            double y_lower_2 = -1.15834790e+03+2.80987862e-01*e_tot - 1.42244814e-05*e_tot*e_tot;


            if (tdiff_ede < y_upper && e_tot>6000 && tdiff_ede > 120 && GATING_EDE_TIME==1 ){
                if(e_tot >= 9500 && tdiff_ede > y_lower_1){
                    energy_particle_time_e_de_all_gate->Fill(e_tot, tdiff_ede);
                    ede_gate->Fill(e_energy, de_energy);
                }
                else if (e_tot < 9500 && tdiff_ede > y_lower_2){
                    energy_particle_time_e_de_all_gate->Fill(e_tot, tdiff_ede);
                    ede_gate->Fill(e_energy, de_energy);
                }

            }

            else if(GATING_EDE_TIME==0){
                energy_particle_time_e_de_all_gate->Fill(e_tot, tdiff_ede);
                ede_gate->Fill(e_energy, de_energy);
            }


            energy_particle_time_e_de_all->Fill(e_tot, tdiff_ede);
            energy_E_particle_time_e_de_all->Fill(e_energy, tdiff_ede);


            double ex = ex_from_ede[3*ring]; // Constant part.
            ex += ex_from_ede[3*ring + 1]*(e_tot*1e-3); // Linear part.
            ex += ex_from_ede[3*ring + 2]*( e_tot*1e-3 )*( e_tot*1e-3 ); // Quadratic term.
            ex *= 1000; // Back to keV units!


            h_ex[tel][ring]->Fill(ex);
            h_ex_all->Fill(ex);


            // Analyze gamma rays.
        #if FISSION
            AnalyzeGammaPPAC(de_word, e_word, ex, event);
        #else
            AnalyzeGamma(de_word, ex, event);
        #endif // FISSION

       }
    }

    return true;

}


void UserSort::AnalyzeGamma(const word_t &de_word, const double &excitation,const Event &event)
{

    // We will loop over all gamma-rays.
    for (int i = 0 ; i < NUM_LABR_DETECTORS ; ++i){
        for (int j = 0 ; j < event.n_labr[i] ; ++j){

            // Get energy and time of the gamma-ray.

            double energy = CalibrateE(event.w_labr[i][j]);
            double tdiff = CalcTimediff(de_word, event.w_labr[i][j]);

            // Fill time spectra.
            labr_align_time->Fill(tdiff, i);

            energy_time_labr[i]->Fill(energy, tdiff);
            energy_time_labr_all->Fill(energy, tdiff);

            if ( excitation > 8473.60 ) //Insert Sn for Pu!
                energy_time_labr_above[i]->Fill(energy, tdiff);


            // Check time gate.
            switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                case is_prompt : {
                    exgam->Fill(energy, excitation);

                    //Gate on event in e-de with given energy, only filling labr energy raw with these events
                    word_t e_word = event.trigger;
                    double E = e_word.adcdata;
                    double E1 = 8100.0;
                    double E2 = 8300.0;

                    if (E>E1 && E<E2 && GATING==1){
                        energy_labr_raw[i]->Fill(event.w_labr[i][j].adcdata);
                    }

                    break;
                }
                case is_background : {
                    exgam->Fill(energy, excitation, -1);
                    exgam_bg->Fill(energy, excitation);
                    word_t e_word = event.trigger;
                    double E = e_word.adcdata;
                    double E1 = 8100.0;
                    double E2 = 8300.0;
                    if (E>E1 && E<E2 &&GATING==1){
                        energy_labr_raw[i]->Fill(event.w_labr[i][j].adcdata, -1);
                    }
                    break;
                }
                case ignore : {
                    break;
                }
            }
        }
    }
}

void UserSort::AnalyzeGammaPPAC(const word_t &de_word, const word_t &e_word, const double &excitation, const Event &event)
{


    // Things with PPAC
    for (int i = 0 ; i < NUM_PPAC ; ++i){
        for (int j = 0 ; j < event.n_ppac[i] ; ++j){

            double tdiff = CalcTimediff(de_word, event.w_ppac[i][j]);
            excitation_time_ppac[i]->Fill(excitation, tdiff);
            //Dortheas attempt
            ppac_vs_dE_time->Fill(tdiff,i);

        }
    }

    // Things with gamma
    for (int i = 0 ; i < NUM_LABR_DETECTORS ; ++i){
        for (int j = 0 ; j < event.n_labr[i] ; ++j){

            // Get energy and time of the gamma-ray.

            double energy = CalibrateE(event.w_labr[i][j]);
            double tdiff = CalcTimediff(de_word, event.w_labr[i][j]);
            double tabs = CalcAbsTime(event.w_labr[i][j]);
            //std::cout << tabs << std::endl;

            //For one labr, check movement of peak
            if(i ==3) {
                prompt_peak_movement->Fill(tdiff, tabs);

            }

            // Fill time spectra.
            labr_align_time->Fill(tdiff, i);
            energy_time_labr[i]->Fill(energy, tdiff);
            energy_time_labr_all->Fill(energy, tdiff);


            word_t e_word = event.trigger;
            double tdiff_ede;
            tdiff_ede = CalcTimediff(e_word, de_word);
            energy_time_e_de_all->Fill(energy, tdiff_ede);

            bool ppac_prompt =  false;
            //bool labr_prompt = false;

            //Is this correct? Doesnt it loop through all events, and changes ppac_prompt between true and false?
            //
            for (int n = 0 ; n < NUM_PPAC ; ++n){
                //std::cout << "m: " << event.n_ppac[n] << std::endl;
                //Number of PPAC
                for (int m = 0 ; m < event.n_ppac[n] ; ++m){
                    //All events in ppacs

                    //double tdiff_ppac = CalcTimediff(event.w_labr[0][0], event.w_ppac[n][m]);
                    double tdiff_ppac = CalcTimediff(event.w_labr[i][j], event.w_ppac[n][m]);

                    ppac_align_time_all->Fill(tdiff_ppac, n);
                    energy_time_ppac[n]->Fill(energy, tdiff_ppac); //if aligned, can use this for PFG/PFN separations


                    switch ( CheckTimeStatus(tdiff_ppac, ppac_time_cuts) ) {
//                        tdiff_ppac: time diff between ppac and Labr3, definere fisjon
                        case is_prompt : {
                            //std::cout << "true" << std::endl;
                            ppac_prompt = true;
                            break;
                        }
                        case is_background : {
                            //std::cout << "false" << std::endl;
                            ppac_prompt = false;
                            break;
                        }
                        case ignore : {
                            //std::cout << "ignore" << std::endl;
                            break;
                        }
                    }
                //std::cout << "ppac_prompt " << ppac_prompt << std::endl;
                }

            }
            //std::cout << "---" << std::endl;

            // Check time gate.
            switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                //tdiff, time diff between de and labr
                case is_prompt : {

                    //Gate on event in e-de with given energy, only filling labr energy raw with these events
                    word_t e_word = event.trigger;
                    double E = e_word.adcdata;
                    double E1 = 8150.0;
                    double E2 = 8300.0;
                    double DE = de_word.adcdata;
                    double DE1 = 1400.0;
                    double DE2 = 1900.0;
                    if (E>E1 && E<E2 && DE>DE1 &&DE<DE2 && GATING==1){
                        energy_labr_raw[i]->Fill(event.w_labr[i][j].adcdata);
                    }
                    double e_energy = CalibrateOnlyE(e_word, de_word);
                    double de_energy = CalibrateE(de_word);
                    if(i==0||i==1||i==2||i==3){
                        ede_all_doublepeak->Fill(e_energy, de_energy);
                    }
                    double e_tot = e_energy + de_energy;
                    double tdiff_ede = CalcTimediff(e_word, de_word);
                    //Cuts in energy_particle_time -> Get rid of background
                    double y_upper = 0.0080361*e_tot + 175.7;
                    double y_lower_1 = 0.00826418*e_tot + 150.7061778;
                    double y_lower_2 = -1.15834790e+03+2.80987862e-01*e_tot - 1.42244814e-05*e_tot*e_tot;

                    //Cut in energy_particle_time
                    if (tdiff_ede < y_upper && e_tot>6000 && tdiff_ede > 120 && GATING_EDE_TIME==1){
                        if(e_tot >= 9500 && tdiff_ede > y_lower_1){
                            exgam->Fill(energy, excitation);
                            if (ppac_prompt)
                                //de, labr and ppac are prompt->Fission
                                exgam_ppac->Fill(energy, excitation);
                            else
                                //de, labr are prompt, but not ppac -> not fission
                                exgam_veto_ppac->Fill(energy, excitation);
                            break;

                        }
                        else if (e_tot < 9500 && tdiff_ede > y_lower_2){
                            exgam->Fill(energy, excitation);
                            if (ppac_prompt)
                                //de, labr and ppac are prompt->Fission
                                exgam_ppac->Fill(energy, excitation);
                            else
                                //de, labr are prompt, but not ppac -> not fission
                                exgam_veto_ppac->Fill(energy, excitation);
                            break;

                        }

                    }

                    else if(GATING_EDE_TIME==0){
                        if (ppac_prompt){
                            //de, labr and ppac are prompt->Fission
                            ede_all_fission->Fill(e_energy, de_energy);
                            exgam_ppac->Fill(energy, excitation);
                        }
                        else{
                            //de, labr are prompt, but not ppac -> not fission
                            ede_all_nofission->Fill(e_energy, de_energy);
                            exgam_veto_ppac->Fill(energy, excitation);
                        }
                            break;

                    }

                }
                case is_background : {
                word_t e_word = event.trigger;
                double E = e_word.adcdata;
                double E1 = 8150.0;
                double E2 = 8300.0;
                double DE = de_word.adcdata;
                double DE1 = 1400.0;
                double DE2 = 1900.0;
                if (E>E1 && E<E2 && DE>DE1 &&DE<DE2 && GATING==1){
                    energy_labr_raw[i]->Fill(event.w_labr[i][j].adcdata,-1);
                }
                double e_energy = CalibrateOnlyE(e_word, de_word);
                double de_energy = CalibrateE(de_word);

                if(i==0||i==1||i==2||i==3){
                    ede_all_doublepeak->Fill(e_energy, de_energy, -1);
                }

                double e_tot = e_energy + de_energy;
                double tdiff_ede = CalcTimediff(e_word, de_word);
                //Cuts in energy_particle_time -> Get rid of background
                double y_upper = 0.0080361*e_tot + 175.7;
                double y_lower_1 = 0.00826418*e_tot + 150.7061778;
                double y_lower_2 = -1.15834790e+03+2.80987862e-01*e_tot - 1.42244814e-05*e_tot*e_tot;

                if (tdiff_ede < y_upper && e_tot>6000 && tdiff_ede > 120 && GATING_EDE_TIME==1){
                    if(e_tot >= 9500 && tdiff_ede > y_lower_1){
                        exgam->Fill(energy, excitation, -1);
                        exgam_bg->Fill(energy, excitation);
                        if (ppac_prompt){
                            exgam_ppac->Fill(energy, excitation, -1);
                            exgam_ppac_bg->Fill(energy, excitation);
                        } else {
                            exgam_veto_ppac->Fill(energy, excitation, -1);
                            exgam_veto_ppac_bg->Fill(energy, excitation);
                        }
                        break;

                    }
                    else if (e_tot < 9500 && tdiff_ede > y_lower_2){
                        exgam->Fill(energy, excitation, -1);
                        exgam_bg->Fill(energy, excitation);
                        if (ppac_prompt){
                            exgam_ppac->Fill(energy, excitation, -1);
                            exgam_ppac_bg->Fill(energy, excitation);
                        } else {
                            exgam_veto_ppac->Fill(energy, excitation, -1);
                            exgam_veto_ppac_bg->Fill(energy, excitation);
                        }
                        break;

                    }

                }

                else if(GATING_EDE_TIME==0){
                    if (ppac_prompt){
                        exgam_ppac->Fill(energy, excitation, -1);
                        ede_all_fission->Fill(e_energy, de_energy, -1);
                        exgam_ppac_bg->Fill(energy, excitation);
                    } else {
                        exgam_veto_ppac->Fill(energy, excitation, -1);
                        ede_all_nofission->Fill(e_energy, de_energy, -1);
                        exgam_veto_ppac_bg->Fill(energy, excitation);
                    }
                    break;

                }

                }
                case ignore : {
                    break;
                }
            }
        }
    }
}




UserSort::prompt_status_t UserSort::CheckTimeStatus(const double &time, const Parameter &parameter) const
{
    if ( time > parameter[0] && time < parameter[1])
        return is_prompt;
    else if (time > parameter[2] && time < parameter[3])
        return is_background;
    else
        return ignore;
}

bool UserSort::End()
{
    std::cout << "Stats info: " << std::endl;
    std::cout << "CFD fails in E - detectors: " << n_fail_e << std::endl;
    std::cout << "CFD fails in dE - detectors: " << n_fail_de << std::endl;
    std::cout << "Average number of dE words: " << n_tot_de/double(tot) << std::endl;
    std::cout << "Average number of E words: " << n_tot_e/double(tot) << std::endl;
    return true;
}
