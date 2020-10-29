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
#define FISSION 1 //should be 1 in fission sorting

//If gating on specified energies in ede
const int GATING = 1;

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
    bool p0 = set_par(parameters, ipar, "a_labr",    NUM_LABR_DETECTORS );
    bool p1 = set_par(parameters, ipar, "b_labr",   NUM_LABR_DETECTORS );
    bool p2 = set_par(parameters, ipar, "c_labr",   NUM_LABR_DETECTORS );
    bool p3 = set_par(parameters, ipar, "gain_de", NUM_SI_DE_DET );
    bool p4 = set_par(parameters, ipar, "shift_de", NUM_SI_DE_DET );
    bool p5 = set_par(parameters, ipar, "gain_e", NUM_SI_DE_DET ); //Changed to DE, to get 64 coeff
    bool p6 = set_par(parameters, ipar, "shift_e", NUM_SI_DE_DET ); //Changed to DE, to get 64 coeff
    bool p7 = set_par(parameters, ipar, "shift_time_labr", NUM_LABR_DETECTORS );
    bool p8 = set_par(parameters, ipar, "shift_time_de", NUM_SI_DE_DET );
    bool p9 = set_par(parameters, ipar, "shift_time_e", NUM_SI_E_DET );
    bool p10 = set_par(parameters, ipar, "shift_time_ppac", NUM_PPAC );
    return (p0 && p1 && p2 && p3 && p4 && p5 && p6 && p7 && p8 && p9 && p10);
}

UserSort::UserSort()
    : a_labr( GetParameters(), "a_labr", NUM_LABR_DETECTORS, 2) //Unsure what the last number means?
    , b_labr( GetParameters(), "b_labr", NUM_LABR_DETECTORS, 1)
    , c_labr( GetParameters(), "c_labr", NUM_LABR_DETECTORS, 0)
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
    , neutron_time_cuts ( GetParameters(), "neutron_time_cuts", 2*2 )
    , ede_time_cuts ( GetParameters(), "ede_time_cuts", 2*2 )
{
}


double UserSort::CalibrateE(const word_t &w) const
{
    DetectorInfo_t info = GetDetector(w.address);
    switch ( info.type ) {

    case labr : {
        return a_labr[info.detectorNum]*(w.adcdata + drand48() - 0.5)*(w.adcdata + drand48() - 0.5) + b_labr[info.detectorNum]*(w.adcdata + drand48() - 0.5) + c_labr[info.detectorNum];
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


    sprintf(tmp, "energy_labr_gated_all");
    energy_labr_gated_all = Spec(tmp, tmp, 10000, 0, 10000, "Energy [keV]");


    // Allocating the LaBr 'singles' spectra
    for (int i = 0 ; i < NUM_LABR_DETECTORS ; ++i){

        // Create energy spectra
        sprintf(tmp, "energy_raw_labr_%02d", i+1);
        energy_labr_raw[i] = Spec(tmp, tmp, 32768, 0, 32768, "Energy [ch]");

        sprintf(tmp, "energy_raw_gated_labr_%02d", i+1);
        energy_labr_raw_gated[i] = Spec(tmp, tmp, 32768, 0, 32768, "Energy [ch]");

        sprintf(tmp, "energy_raw_particle_labr_%02d", i+1);
        energy_labr_raw_particle[i] = Spec(tmp, tmp, 32768, 0, 32768, "Energy [ch]");

        sprintf(tmp, "energy_labr_%02d", i+1);
        energy_labr[i] = Spec(tmp, tmp, 10000, 0, 10000, "Energy [keV]");

        sprintf(tmp, "energy_time_labr_aboveSn_%02d", i+1);
        //Mat(tmp,tmp,nrbins,start,stop, name of xaxis, and then if 2D, again)
        energy_time_labr_above[i] = Mat(tmp, tmp, 1000, 0, 16000, "LaBr energy [keV]", 2000, -100, 100, "Time difference [ns]");
    }

    sprintf(tmp, "energy_labr_crosstalk_1_2");
    energy_labr_crosstalk_1_2 = Spec(tmp, tmp, 500, 0, 10000, "Energy [keV]");

    sprintf(tmp, "energy_labr_crosstalk_1_5");
    energy_labr_crosstalk_1_5 = Spec(tmp, tmp, 500, 0, 10000, "Energy [keV]");

    sprintf(tmp, "energy_labr_crosstalk_1_3");
    energy_labr_crosstalk_1_3 = Spec(tmp, tmp, 500, 0, 10000, "Energy [keV]");

    sprintf(tmp, "energy_labr_crosstalk_1_4");
    energy_labr_crosstalk_1_4 = Spec(tmp, tmp, 500, 0, 10000, "Energy [keV]");

    //Needed to fill in root
    sprintf(tmp, "energy_labr_crosstalk_A");
    energy_labr_crosstalk_A = Spec(tmp, tmp, 500, 0, 10000, "Energy [keV]");

    sprintf(tmp, "energy_labr_crosstalk_B");
    energy_labr_crosstalk_B = Spec(tmp, tmp, 500, 0, 10000, "Energy [keV]");

    sprintf(tmp, "energy_labr_crosstalk_C");
    energy_labr_crosstalk_C = Spec(tmp, tmp, 500, 0, 10000, "Energy [keV]");

    sprintf(tmp, "energy_labr_crosstalk_D");
    energy_labr_crosstalk_D = Spec(tmp, tmp, 500, 0, 10000, "Energy [keV]");


    sprintf(tmp, "number_of_fissions");
    number_of_fissions = Spec(tmp, tmp, 1500, 0, 15000, "Ex energy [MeV]");
    sprintf(tmp, "number_of_fissions_all");
    number_of_fissions_all = Spec(tmp, tmp, 1500, 0, 15000, "Ex energy [MeV]"); //before bg-sub
    sprintf(tmp, "number_of_fissions_bg");
    number_of_fissions_bg = Spec(tmp, tmp, 1500, 0, 15000, "Ex energy [MeV]"); //bg in number of fissions

    sprintf(tmp, "PFG_mult");
    sprintf(tmp2, "PFG multiplicity");
    PFG_mult = Mat(tmp, tmp2, 16, 0, 15, "PFG multiplicity", 16, 0, 15000, "Ex energy [keV]");

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

            sprintf(tmp, "ede_raw_gated_b%d_f%d", i, j);
            sprintf(tmp2, "E : DE raw, pad %d, ring %d", i, j);
            ede_raw_gated[i][j] = Mat(tmp, tmp2, 2048, 0, 32768, "Back energy [ch]", 2048, 0, 32768, "Front energy [ch]");

            // Make 'calibrated' ede spectrum.
            sprintf(tmp, "ede_b%d_f%d", i, j);
            sprintf(tmp2, "E : DE calibrated, pad %d, ring %d", i, j);
            ede[i][j] = Mat(tmp, tmp2, 2000, 0, 20000, "Back energy [keV]", 500, 0, 5000, "Front energy [keV]");

            sprintf(tmp, "ede_gated_b%d_f%d", i, j);
            sprintf(tmp2, "E : DE calibrated, pad %d, ring %d", i, j);
            ede_gated[i][j] = Mat(tmp, tmp2, 2000, 0, 20000, "Back energy [keV]", 500, 0, 5000, "Front energy [keV]");

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

    sprintf(tmp, "energy_labr_time_e_de");
    sprintf(tmp2, "LaBr energy : e-de time diff");
    energy_labr_time_e_de = Mat(tmp, tmp2, 5000, 0, 6000, "LaBr energy [keV]", 5000, 0, 300, "t_{DE} - t_{E} [ns]");

    sprintf(tmp, "energy_E_particle_time_e_de");
    sprintf(tmp2, "E energy : e-de time diff");
    energy_E_particle_time_e_de = Mat(tmp, tmp2, 5000, 0, 20000, "E energy [keV]", 5000, 0, 300, "t_{DE} - t_{E} [ns]");

    sprintf(tmp, "excitation_vs_labr_time");
    sprintf(tmp2, "Excitation : t_{LaBr} - t_{dE ANY}");
    excitation_vs_labr_time = Mat(tmp, tmp2, 2000, 0, 10000, "Excitation energy [keV]", 5000, -1500, 1500, "t_{LaBr} - t_{DE} [ns]");

    sprintf(tmp, "E_energy_vs_labr_time");
    sprintf(tmp2, "E energy : t_{LaBr} - t_{dE ANY}");
    E_energy_vs_labr_time = Mat(tmp, tmp2, 2000, 0, 10000, "Excitation energy [keV]", 5000, -1500, 1500, "t_{LaBr} - t_{DE} [ns]");


    sprintf(tmp, "energy_labr_all");
    energy_labr_all = Spec(tmp, tmp, 10000, 0, 10000, "Energy [keV]");

    sprintf(tmp, "de_align_time");
    sprintf(tmp2, "t_{dE} - t_{LaBr nr. 1}");
    de_align_time = Mat(tmp, tmp2, 5000, -1500, 1500, "t_{dE} - t_{LaBr nr. 1} [ns]", NUM_SI_DE_DET, 0, NUM_SI_DE_DET, "#Delta E detector id.");

    sprintf(tmp, "labr_vs_ppac_time");
    labr_vs_ppac_time = Mat(tmp, tmp, 5000, -1500, 1500, "t_{LaBr} - t_{DE} [ns]", 5000, -1500, 1500, "t_{PPAC} - t_{DE} [ns]");

    sprintf(tmp, "ppac_vs_dE_time");
    sprintf(tmp2, "t_{PPAC} - t_{dE ANY}");
    ppac_vs_dE_time = Mat(tmp, tmp2, 4000, -1500, 1500, "t_{PPAC 1} - t_{dE ANY} [ns]", NUM_PPAC, 0, NUM_PPAC, "PPAC id.");

    sprintf(tmp, "labr_align_time");
    sprintf(tmp2, "t_{LaBr} - t_{dE ANY}");
    labr_align_time = Mat(tmp, tmp2, 5000, -1500, 1500, "t_{LaBr} - t_{dE ANY} [ns]", NUM_LABR_DETECTORS, 0, NUM_LABR_DETECTORS, "LaBr detector id.");

    sprintf(tmp, "ppac_align_time");
    sprintf(tmp2, "t_{PPAC} - t_{dE ANY}");
    ppac_align_time = Mat(tmp, tmp2, 3100, -1500, 1500, "t_{PPAC} - t_{dE ANY} [ns]", NUM_PPAC, 0, NUM_PPAC, "PPAC id.");

    sprintf(tmp, "time_energy_labr");
    sprintf(tmp2, "t_{LaBr} - t_{dE ANY} : E_{LaBr}");
    time_energy_labr = Mat(tmp, tmp2, 2000, -50, 50, "t_{LaBr} - t_{DE} [ns]", 5000, 0, 10000, "Energy LaBr [keV]");

    sprintf(tmp, "time_energy_labr_fission");
    sprintf(tmp2, "t_{LaBr} - t_{dE ANY} : E_{LaBr}, fission");
    time_energy_labr_fission = Mat(tmp, tmp2, 2000, -50, 50, "t_{LaBr} - t_{DE} [ns]", 5000, 0, 10000, "Energy LaBr [keV]");

    sprintf(tmp, "ede_all");
    sprintf(tmp2, "E : DE, all");
    ede_all = Mat(tmp, tmp2, 10000, 0, 20000, "Back energy [keV]", 1000, 0, 5000, "Front energy [keV]");

    //Checking where the particles in correlation with fission comes from
    sprintf(tmp, "ede_fission");
    sprintf(tmp2, "E : DE, all");
    ede_fission = Mat(tmp, tmp2, 10000, 0, 20000, "Back energy [keV]", 1000, 0, 5000, "Front energy [keV]");

    sprintf(tmp, "ede_fission_nobgsub");
    sprintf(tmp2, "E : DE, all");
    ede_fission_nobgsub = Mat(tmp, tmp2, 10000, 0, 20000, "Back energy [keV]", 1000, 0, 5000, "Front energy [keV]");

    sprintf(tmp, "ede_fission_bg");
    sprintf(tmp2, "E : DE, all");
    ede_fission_bg = Mat(tmp, tmp2, 10000, 0, 20000, "Back energy [keV]", 1000, 0, 5000, "Front energy [keV]");

    sprintf(tmp, "ede_all_nofission");
    sprintf(tmp2, "E : DE, all");
    ede_all_nofission = Mat(tmp, tmp2, 10000, 0, 20000, "Back energy [keV]", 1000, 0, 5000, "Front energy [keV]");

    //Checking where the gammas in coincidence with a particle and fission come from
    sprintf(tmp, "ede_gamma_fission");
    sprintf(tmp2, "E : DE, all");
    ede_gamma_fission = Mat(tmp, tmp2, 10000, 0, 20000, "Back energy [keV]", 1000, 0, 5000, "Front energy [keV]");

    sprintf(tmp, "ede_gamma_fission_nobgsub");
    sprintf(tmp2, "E : DE, all");
    ede_gamma_fission_nobgsub = Mat(tmp, tmp2, 10000, 0, 20000, "Back energy [keV]", 1000, 0, 5000, "Front energy [keV]");

    sprintf(tmp, "ede_gamma_fission_bg");
    sprintf(tmp2, "E : DE, all");
    ede_gamma_fission_bg = Mat(tmp, tmp2, 10000, 0, 20000, "Back energy [keV]", 1000, 0, 5000, "Front energy [keV]");


    sprintf(tmp, "ede_all_bg");
    sprintf(tmp2, "E : DE, all, background");
    ede_all_bg = Mat(tmp, tmp2, 10000, 0, 20000, "Back energy [keV]", 1000, 0, 5000, "Front energy [keV]");

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
    exgam_ppac = Mat(tmp, tmp, 1500, 0, 15000, "LaBr [keV]", 1500, 0, 15000, "Ex [keV]");

    sprintf(tmp, "exgam_ppac_all"); //alfna med fisjon, ingen bg-sub
    exgam_ppac_all = Mat(tmp, tmp, 1500, 0, 15000, "LaBr [keV]", 1500, 0, 15000, "Ex [keV]");

    sprintf(tmp, "exgam_ppac_bg"); //the bg subtratcted from exgam_ppac
    exgam_ppac_bg = Mat(tmp, tmp, 1500, 0, 15000, "LaBr [keV]", 1500, 0, 15000, "Ex [keV]");

    sprintf(tmp, "exgam_veto_ppac"); //alfna uten fisjon
    exgam_veto_ppac = Mat(tmp, tmp, 1500, 0, 15000, "LaBr [keV]", 1500, 0, 15000, "Ex [keV]");

    sprintf(tmp, "exgam_veto_ppac_bg");
    exgam_veto_ppac_bg = Mat(tmp, tmp, 1500, 0, 15000, "LaBr [keV]", 1500, 0, 15000, "Ex [keV]");

    sprintf(tmp, "exgam_ppac_neutron");
    exgam_ppac_neutron = Mat(tmp, tmp, 1500, 0, 15000, "LaBr [keV]", 1500, 0, 15000, "Ex [keV]");

    sprintf(tmp, "exgam_ppac_neutron_all");
    exgam_ppac_neutron_all = Mat(tmp, tmp, 1500, 0, 15000, "LaBr [keV]", 1500, 0, 15000, "Ex [keV]");

    sprintf(tmp, "exgam_ppac_neutron_bg");
    exgam_ppac_neutron_bg = Mat(tmp, tmp, 1500, 0, 15000, "LaBr [keV]", 1500, 0, 15000, "Ex [keV]");

    sprintf(tmp, "multiple_fission_events");
    multiple_fission_events = Spec(tmp, tmp, 20, 0, 20, "Counts");

    sprintf(tmp, "multiple_fission_events_prompt_bg");
    multiple_fission_events_prompt_bg = Mat(tmp, tmp, 20, 0, 20, "prompt fission fragment", 20, 0, 20, "bg fission fragment");



    n_fail_e = 0;
    n_fail_de = 0;
    n_tot_e = 0;
    n_tot_de = 0;
    tot = 0;
}


bool UserSort::Sort(const Event &event)
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
            energy_labr_raw[i]->Fill(event.w_labr[i][j].adcdata);

            energy = CalibrateE(event.w_labr[i][j]);
            energy_labr[i]->Fill(energy);
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
            //singles spectra doesn't use correct calibration
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

        if ( event.n_labr[0] == 1){
            tdiff = CalcTimediff(event.w_labr[0][0], de_word);
            de_align_time->Fill(tdiff, GetDetector(de_word.address).detectorNum);
        }

        for (int i = 0 ; i < NUM_PPAC ; ++i){
            for (int j = 0 ; j < event.n_ppac[i] ; ++j){
                tdiff = CalcTimediff(de_word, event.w_ppac[i][j]);
                ppac_align_time->Fill(tdiff, i);
            }
        }


        // Fill DE - E matrices.
        ede_raw[tel][ring]->Fill(e_word.adcdata, de_word.adcdata);

        //If energy gate
        double E = e_word.adcdata;
        double DE = de_word.adcdata;
        double E1_E = 11500.0;
        double E2_E = 11800.0;
        double E1_DE = 830.0;
        double E2_DE = 1050.0;
        if (E>E1_E && E<E2_E && DE>E1_DE &&DE<E2_DE && GATING==1){
            ede_raw_gated[tel][ring]->Fill(E, DE);
        }

        double e_energy = CalibrateOnlyE(e_word, de_word);
        double de_energy = CalibrateE(de_word);

        ede[tel][ring]->Fill(e_energy, de_energy);

        double E1 = 7700.0;
        double E2 = 8000.0;
        double DE1 = 1150.0;
        double DE2 = 1500.0;
        if (e_energy>E1 && e_energy<E2 && de_energy>DE1 && de_energy<DE2 && GATING==1){
            ede_gated[tel][ring]->Fill(e_energy, de_energy);
        }

        //EDE time gate
        tdiff_ede = CalcTimediff(e_word, de_word);
        ede_all->Fill(e_energy, de_energy);

        // Calculate 'apparent thickness'
        double thick = range.GetRange(e_energy + de_energy) - range.GetRange(e_energy);
        h_thick->Fill(thick);


        // Calculate the particle energy.
        double e_tot = e_energy + de_energy;


        // Check if correct particle
        if ( thick >= thick_range[0] && thick <= thick_range[1] ){

            // Filling 'total particle energy' spectrum.
            h_ede[tel][ring]->Fill( e_tot );
            h_ede[tel][ring]->Fill( e_tot );
            h_ede_all->Fill( e_tot );

            ede_gate->Fill(e_energy, de_energy);

            energy_E_particle_time_e_de->Fill(e_energy, tdiff_ede);


            double ex = ex_from_ede[3*ring]; // Constant part.
            ex += ex_from_ede[3*ring + 1]*(e_tot*1e-3); // Linear part.
            ex += ex_from_ede[3*ring + 2]*( e_tot*1e-3 )*( e_tot*1e-3 ); // Quadratic term.
            ex *= 1000; // Back to keV units!

            h_ex[tel][ring]->Fill(ex);
            h_ex_all->Fill(ex);


            // Analyze gamma rays.
        #if FISSION
            word_t e_word = event.trigger;
            AnalyzeGammaPPAC(de_word, e_word, ex, event);
        #else
            AnalyzeGamma(de_word, ex, event);
        #endif

       }
    }

    return true;

}


void UserSort::AnalyzeGamma(const word_t &de_word, const double &excitation,const Event &event)
{

    //Checking crosstalk 1&2
    if(event.n_labr[0] > 0 && event.n_labr[1] > 0){
        int prompt_0 = 0;
        int prompt_1 = 0;

        for (int i = 0 ; i < event.n_labr[0] ; ++i){
            double tdiff= CalcTimediff(de_word, event.w_labr[0][i]);
            switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                case is_prompt : {
                    prompt_0 = 1;
                }
            }
        }
        for (int i = 0 ; i < event.n_labr[1] ; ++i){
            double tdiff= CalcTimediff(de_word, event.w_labr[1][i]);
            switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                case is_prompt : {
                    prompt_1 = 1;
                }
            }
        }
        if(prompt_0==1 && prompt_1==1){
            for (int i = 0 ; i < event.n_labr[0] ; ++i){
                double tdiff= CalcTimediff(de_word, event.w_labr[0][i]);
                switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                    case is_prompt : {
                        double energy = CalibrateE(event.w_labr[0][i]);
                        energy_labr_crosstalk_1_2->Fill(energy);
                    }
                }
            }
            for (int i = 0 ; i < event.n_labr[1] ; ++i){
                double tdiff= CalcTimediff(de_word, event.w_labr[1][i]);
                switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                    case is_prompt : {
                        double energy = CalibrateE(event.w_labr[1][i]);
                        energy_labr_crosstalk_1_2->Fill(energy);
                    }
                }
            }
        }
    }

    //Checking crosstalk 1&5
    if(event.n_labr[0] > 0 && event.n_labr[4] > 0){
        int prompt_0 = 0;
        int prompt_1 = 0;

        for (int i = 0 ; i < event.n_labr[0] ; ++i){
            double tdiff= CalcTimediff(de_word, event.w_labr[0][i]);
            switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                case is_prompt : {
                    prompt_0 = 1;
                }
            }
        }
        for (int i = 0 ; i < event.n_labr[4] ; ++i){
            double tdiff= CalcTimediff(de_word, event.w_labr[4][i]);
            switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                case is_prompt : {
                    prompt_1 = 1;
                }
            }
        }
        if(prompt_0==1 && prompt_1==1){
            for (int i = 0 ; i < event.n_labr[0] ; ++i){
                double tdiff= CalcTimediff(de_word, event.w_labr[0][i]);
                switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                    case is_prompt : {
                        double energy = CalibrateE(event.w_labr[0][i]);
                        energy_labr_crosstalk_1_5->Fill(energy);
                    }
                }
            }
            for (int i = 0 ; i < event.n_labr[4] ; ++i){
                double tdiff= CalcTimediff(de_word, event.w_labr[4][i]);
                switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                    case is_prompt : {
                        double energy = CalibrateE(event.w_labr[4][i]);
                        energy_labr_crosstalk_1_5->Fill(energy);
                    }
                }
            }
        }
    }


    //Checking crosstalk 1&3
    if(event.n_labr[0] > 0 && event.n_labr[2] > 0){
        int prompt_0 = 0;
        int prompt_1 = 0;

        for (int i = 0 ; i < event.n_labr[0] ; ++i){
            double tdiff= CalcTimediff(de_word, event.w_labr[0][i]);
            switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                case is_prompt : {
                    prompt_0 = 1;
                }
            }
        }
        for (int i = 0 ; i < event.n_labr[2] ; ++i){
            double tdiff= CalcTimediff(de_word, event.w_labr[2][i]);
            switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                case is_prompt : {
                    prompt_1 = 1;
                }
            }
        }
        if(prompt_0==1 && prompt_1==1){
            for (int i = 0 ; i < event.n_labr[0] ; ++i){
                double tdiff= CalcTimediff(de_word, event.w_labr[0][i]);
                switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                    case is_prompt : {
                        double energy = CalibrateE(event.w_labr[0][i]);
                        energy_labr_crosstalk_1_3->Fill(energy);
                    }
                }
            }
            for (int i = 0 ; i < event.n_labr[2] ; ++i){
                double tdiff= CalcTimediff(de_word, event.w_labr[2][i]);
                switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                    case is_prompt : {
                        double energy = CalibrateE(event.w_labr[2][i]);
                        energy_labr_crosstalk_1_3->Fill(energy);
                    }
                }
            }
        }
    }

    //Checking crosstalk 1&4
    if(event.n_labr[0] > 0 && event.n_labr[3] > 0){
        int prompt_0 = 0;
        int prompt_1 = 0;

        for (int i = 0 ; i < event.n_labr[0] ; ++i){
            double tdiff= CalcTimediff(de_word, event.w_labr[0][i]);
            switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                case is_prompt : {
                    prompt_0 = 1;
                }
            }
        }
        for (int i = 0 ; i < event.n_labr[3] ; ++i){
            double tdiff= CalcTimediff(de_word, event.w_labr[3][i]);
            switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                case is_prompt : {
                    prompt_1 = 1;
                }
            }
        }
        if(prompt_0==1 && prompt_1==1){
            for (int i = 0 ; i < event.n_labr[0] ; ++i){
                double tdiff= CalcTimediff(de_word, event.w_labr[0][i]);
                switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                    case is_prompt : {
                        double energy = CalibrateE(event.w_labr[0][i]);
                        energy_labr_crosstalk_1_4->Fill(energy);
                    }
                }
            }
            for (int i = 0 ; i < event.n_labr[3] ; ++i){
                double tdiff= CalcTimediff(de_word, event.w_labr[3][i]);
                switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                    case is_prompt : {
                        double energy = CalibrateE(event.w_labr[3][i]);
                        energy_labr_crosstalk_1_4->Fill(energy);
                    }
                }
            }
        }
    }

    // We will loop over all gamma-rays.
    for (int i = 0 ; i < NUM_LABR_DETECTORS ; ++i){
        for (int j = 0 ; j < event.n_labr[i] ; ++j){

            // Get energy and time of the gamma-ray.

            double energy = CalibrateE(event.w_labr[i][j]);
            double tdiff = CalcTimediff(de_word, event.w_labr[i][j]);

            // Fill time spectra.
            labr_align_time->Fill(tdiff, i);
            time_energy_labr->Fill(tdiff, energy);

            if ( excitation > 8473.60 ) //Insert Sn for Pu!
                energy_time_labr_above[i]->Fill(energy, tdiff);


            // Check time gate.
            switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                case is_prompt : {
                    exgam->Fill(energy, excitation);
                    energy_labr_raw_particle[i]->Fill(event.w_labr[i][j].adcdata);
                    //Gate on event in e-de with given energy, only filling labr energy raw with these events
                    word_t e_word = event.trigger;
                    double E = CalibrateOnlyE(e_word, de_word);
                    double DE = CalibrateE(de_word);
                    //E and DE are calibrated, give energy!
                    double E1 = 12000.0;
                    double E2 = 17000.0;
                    double DE1 = 600.0;
                    double DE2 = 1300.0;
                    if (E>E1 && E<E2 && DE>DE1 && DE<DE2 && GATING==1){
                        energy_labr_raw_gated[i]->Fill(event.w_labr[i][j].adcdata);
                        energy_labr_gated_all->Fill(energy);
                    }

                    break;
                }
                case is_background : {
                    exgam->Fill(energy, excitation, -1);
                    exgam_bg->Fill(energy, excitation);
                    energy_labr_raw_particle[i]->Fill(event.w_labr[i][j].adcdata, -1);

                    word_t e_word = event.trigger;
                    double E = CalibrateOnlyE(e_word, de_word);
                    double DE = CalibrateE(de_word);
                    double E1 = 12000.0;
                    double E2 = 17000.0;
                    double DE1 = 600.0;
                    double DE2 = 1300.0;
                    if (E>E1 && E<E2 && DE>DE1 && DE<DE2 && GATING==1){
                        energy_labr_raw_gated[i]->Fill(event.w_labr[i][j].adcdata, -1);
                        energy_labr_gated_all->Fill(energy, -1);
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
    float bg_param = -1.0;

    // Things with PPAC
    for (int i = 0 ; i < NUM_PPAC ; ++i){
        for (int j = 0 ; j < event.n_ppac[i] ; ++j){

            double tdiff_ppac = CalcTimediff(de_word, event.w_ppac[i][j]);
            excitation_time_ppac[i]->Fill(excitation, tdiff_ppac);
            ppac_vs_dE_time->Fill(tdiff_ppac,i);
        }
    }

    //Things with gamma
    for (int i = 0 ; i < NUM_LABR_DETECTORS ; ++i){
        for (int j = 0 ; j < event.n_labr[i] ; ++j){

            double energy = CalibrateE(event.w_labr[i][j]);
            double tdiff = CalcTimediff(de_word, event.w_labr[i][j]);

            labr_align_time->Fill(tdiff, i);
            time_energy_labr->Fill(tdiff, energy);

            word_t e_word = event.trigger;
            double tdiff_ede;
            double e_energy = CalibrateOnlyE(e_word, de_word);
            tdiff_ede = CalcTimediff(e_word, de_word);
            energy_labr_time_e_de->Fill(energy, tdiff_ede);

            excitation_vs_labr_time->Fill(excitation,tdiff);
            E_energy_vs_labr_time->Fill(e_energy,tdiff);

            switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                case is_prompt : {
                    exgam->Fill(energy, excitation);
                    break;
                }
                case is_background : {
                    exgam->Fill(energy, excitation, bg_param);
                    exgam_bg->Fill(energy, excitation, -bg_param);
                    break;
                }
                case ignore : {
                    break;
                }
            }

        }
    }

    //Checking crosstalk 1&2
    if(event.n_labr[0] > 0 && event.n_labr[1] > 0){
        int prompt_0 = 0;
        int prompt_1 = 0;

        for (int i = 0 ; i < event.n_labr[0] ; ++i){
            double tdiff= CalcTimediff(de_word, event.w_labr[0][i]);
            switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                case is_prompt : {
                    prompt_0 = 1;
                }
            }
        }
        for (int i = 0 ; i < event.n_labr[1] ; ++i){
            double tdiff= CalcTimediff(de_word, event.w_labr[1][i]);
            switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                case is_prompt : {
                    prompt_1 = 1;
                }
            }
        }
        if(prompt_0==1 && prompt_1==1){
            for (int i = 0 ; i < event.n_labr[0] ; ++i){
                double tdiff= CalcTimediff(de_word, event.w_labr[0][i]);
                switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                    case is_prompt : {
                        double energy = CalibrateE(event.w_labr[0][i]);
                        energy_labr_crosstalk_1_2->Fill(energy);
                    }
                }
            }
            for (int i = 0 ; i < event.n_labr[1] ; ++i){
                double tdiff= CalcTimediff(de_word, event.w_labr[1][i]);
                switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                    case is_prompt : {
                        double energy = CalibrateE(event.w_labr[1][i]);
                        energy_labr_crosstalk_1_2->Fill(energy);
                    }
                }
            }
        }
    }

    //Checking crosstalk 1&5
    if(event.n_labr[0] > 0 && event.n_labr[4] > 0){
        int prompt_0 = 0;
        int prompt_1 = 0;

        for (int i = 0 ; i < event.n_labr[0] ; ++i){
            double tdiff= CalcTimediff(de_word, event.w_labr[0][i]);
            switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                case is_prompt : {
                    prompt_0 = 1;
                }
            }
        }
        for (int i = 0 ; i < event.n_labr[4] ; ++i){
            double tdiff= CalcTimediff(de_word, event.w_labr[4][i]);
            switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                case is_prompt : {
                    prompt_1 = 1;
                }
            }
        }
        if(prompt_0==1 && prompt_1==1){
            for (int i = 0 ; i < event.n_labr[0] ; ++i){
                double tdiff= CalcTimediff(de_word, event.w_labr[0][i]);
                switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                    case is_prompt : {
                        double energy = CalibrateE(event.w_labr[0][i]);
                        energy_labr_crosstalk_1_5->Fill(energy);
                    }
                }
            }
            for (int i = 0 ; i < event.n_labr[4] ; ++i){
                double tdiff= CalcTimediff(de_word, event.w_labr[4][i]);
                switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                    case is_prompt : {
                        double energy = CalibrateE(event.w_labr[4][i]);
                        energy_labr_crosstalk_1_5->Fill(energy);
                    }
                }
            }
        }
    }


    //Checking crosstalk 1&3
    if(event.n_labr[0] > 0 && event.n_labr[2] > 0){
        int prompt_0 = 0;
        int prompt_1 = 0;

        for (int i = 0 ; i < event.n_labr[0] ; ++i){
            double tdiff= CalcTimediff(de_word, event.w_labr[0][i]);
            switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                case is_prompt : {
                    prompt_0 = 1;
                }
            }
        }
        for (int i = 0 ; i < event.n_labr[2] ; ++i){
            double tdiff= CalcTimediff(de_word, event.w_labr[2][i]);
            switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                case is_prompt : {
                    prompt_1 = 1;
                }
            }
        }
        if(prompt_0==1 && prompt_1==1){
            for (int i = 0 ; i < event.n_labr[0] ; ++i){
                double tdiff= CalcTimediff(de_word, event.w_labr[0][i]);
                switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                    case is_prompt : {
                        double energy = CalibrateE(event.w_labr[0][i]);
                        energy_labr_crosstalk_1_3->Fill(energy);
                    }
                }
            }
            for (int i = 0 ; i < event.n_labr[2] ; ++i){
                double tdiff= CalcTimediff(de_word, event.w_labr[2][i]);
                switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                    case is_prompt : {
                        double energy = CalibrateE(event.w_labr[2][i]);
                        energy_labr_crosstalk_1_3->Fill(energy);
                    }
                }
            }
        }
    }

    //Checking crosstalk 1&4
    if(event.n_labr[0] > 0 && event.n_labr[3] > 0){
        int prompt_0 = 0;
        int prompt_1 = 0;

        for (int i = 0 ; i < event.n_labr[0] ; ++i){
            double tdiff= CalcTimediff(de_word, event.w_labr[0][i]);
            switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                case is_prompt : {
                    prompt_0 = 1;
                }
            }
        }
        for (int i = 0 ; i < event.n_labr[3] ; ++i){
            double tdiff= CalcTimediff(de_word, event.w_labr[3][i]);
            switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                case is_prompt : {
                    prompt_1 = 1;
                }
            }
        }
        if(prompt_0==1 && prompt_1==1){
            for (int i = 0 ; i < event.n_labr[0] ; ++i){
                double tdiff= CalcTimediff(de_word, event.w_labr[0][i]);
                switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                    case is_prompt : {
                        double energy = CalibrateE(event.w_labr[0][i]);
                        energy_labr_crosstalk_1_4->Fill(energy);
                    }
                }
            }
            for (int i = 0 ; i < event.n_labr[3] ; ++i){
                double tdiff= CalcTimediff(de_word, event.w_labr[3][i]);
                switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                    case is_prompt : {
                        double energy = CalibrateE(event.w_labr[3][i]);
                        energy_labr_crosstalk_1_4->Fill(energy);
                    }
                }
            }
        }
    }


    //Creating fission spectra
    int multiple_fission_counter = 0;

    int prompt_ff = 0;
    int bg_ff = 0;

    for (int n = 0 ; n < NUM_PPAC ; ++n){
        for (int m = 0 ; m < event.n_ppac[n] ; ++m){
            multiple_fission_counter += 1;
            //All events in ppacs
            word_t e_word = event.trigger;
            double e_energy = CalibrateOnlyE(e_word, de_word);
            double de_energy = CalibrateE(de_word);
            double tdiff_ppac = CalcTimediff(de_word, event.w_ppac[n][m]);

            switch ( CheckTimeStatus(tdiff_ppac, ppac_time_cuts) ) {
                case is_prompt : {

                    prompt_ff += 1;
                    number_of_fissions->Fill(excitation);
                    number_of_fissions_all->Fill(excitation);
                    ede_fission->Fill(e_energy, de_energy);
                    ede_fission_nobgsub->Fill(e_energy, de_energy);


                    for (int i = 0 ; i < NUM_LABR_DETECTORS ; ++i){
                        for (int j = 0 ; j < event.n_labr[i] ; ++j){
                            double energy = CalibrateE(event.w_labr[i][j]);
                            double tdiff = CalcTimediff(de_word, event.w_labr[i][j]);

                            time_energy_labr_fission->Fill(tdiff, energy);
                            labr_vs_ppac_time->Fill(tdiff, tdiff_ppac);

                            switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                                case is_prompt : {
                                    //Fill
                                    exgam_ppac->Fill(energy, excitation);
                                    exgam_ppac_all->Fill(energy, excitation);

                                    ede_gamma_fission->Fill(e_energy, de_energy);
                                    ede_gamma_fission_nobgsub->Fill(e_energy, de_energy);
                                    break;
                                }

                                case is_background : {
                                    //Subtract
                                    exgam_ppac->Fill(energy, excitation, bg_param);
                                    exgam_ppac_bg->Fill(energy, excitation, -bg_param);

                                    ede_gamma_fission->Fill(e_energy, de_energy, bg_param);
                                    ede_gamma_fission_bg->Fill(e_energy, de_energy, -bg_param);

                                    break;
                                }

                                case ignore :{
                                    break;
                                }
                            }
                        }
                    }

                    break;

                }
                case is_background : {

                    bg_ff += 1;

                    number_of_fissions->Fill(excitation, bg_param);
                    number_of_fissions_bg->Fill(excitation);
                    ede_fission->Fill(e_energy, de_energy, bg_param);
                    ede_fission_bg->Fill(e_energy, de_energy, -bg_param);

                    for (int i = 0 ; i < NUM_LABR_DETECTORS ; ++i){
                        for (int j = 0 ; j < event.n_labr[i] ; ++j){
                            double energy = CalibrateE(event.w_labr[i][j]);
                            double tdiff = CalcTimediff(de_word, event.w_labr[i][j]);
                            labr_vs_ppac_time->Fill(tdiff, tdiff_ppac);


                            switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                                case is_prompt : {
                                    //Subtract
                                    exgam_ppac->Fill(energy, excitation, bg_param);
                                    exgam_ppac_bg->Fill(energy, excitation, -bg_param);

                                    ede_gamma_fission->Fill(e_energy, de_energy, bg_param);
                                    ede_gamma_fission_bg->Fill(e_energy, de_energy, -bg_param);
                                    break;
                                }

                                case is_background : {
                                    //Subtract
                                    exgam_ppac->Fill(energy, excitation, bg_param);
                                    exgam_ppac_bg->Fill(energy, excitation, -bg_param);

                                    ede_gamma_fission->Fill(e_energy, de_energy, bg_param);
                                    ede_gamma_fission_bg->Fill(e_energy, de_energy, -bg_param);

                                    break;

                                }

                                case ignore :{
                                    break;
                                }
                            }
                        }
                    }

                    break;
                }
                case ignore : {
                    for (int i = 0 ; i < NUM_LABR_DETECTORS ; ++i){
                        for (int j = 0 ; j < event.n_labr[i] ; ++j){
                            double tdiff = CalcTimediff(de_word, event.w_labr[i][j]);
                            labr_vs_ppac_time->Fill(tdiff, tdiff_ppac);

                        }
                    }
                }
            }

        }

    }
    if(multiple_fission_counter > 0){
        multiple_fission_events->Fill(multiple_fission_counter);
    }

    if(prompt_ff > 0 || bg_ff > 0){
        multiple_fission_events_prompt_bg->Fill(prompt_ff,bg_ff);
    }


    //Creating neutron spectrum
    for (int n = 0 ; n < NUM_PPAC ; ++n){
        for (int m = 0 ; m < event.n_ppac[n] ; ++m){
            word_t e_word = event.trigger;
            double tdiff_ppac = CalcTimediff(de_word, event.w_ppac[n][m]);

            switch ( CheckTimeStatus(tdiff_ppac, ppac_time_cuts) ) {
                case is_prompt : {

                    for (int i = 0 ; i < NUM_LABR_DETECTORS ; ++i){
                        for (int j = 0 ; j < event.n_labr[i] ; ++j){
                            double tdiff = CalcTimediff(de_word, event.w_labr[i][j]);
                            double energy = CalibrateE(event.w_labr[i][j]);

                            switch ( CheckTimeStatus(tdiff, neutron_time_cuts) ) {
                                case is_prompt : {
                                    exgam_ppac_neutron->Fill(energy, excitation);
                                    exgam_ppac_neutron_all->Fill(energy, excitation);
                                    break;
                                }

                                case is_background : {
                                    exgam_ppac_neutron->Fill(energy, excitation,bg_param);
                                    exgam_ppac_neutron_bg->Fill(energy, excitation,-bg_param);
                                    break;
                                }

                                case ignore :{
                                    break;
                                }
                            }
                        }
                    }

                    break;
                }
                case is_background : {
                    for (int i = 0 ; i < NUM_LABR_DETECTORS ; ++i){
                        for (int j = 0 ; j < event.n_labr[i] ; ++j){
                            double energy = CalibrateE(event.w_labr[i][j]);
                            double tdiff = CalcTimediff(de_word, event.w_labr[i][j]);


                            switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                                case is_prompt : {
                                    //Subtract
                                    exgam_ppac_neutron->Fill(energy, excitation,bg_param);
                                    exgam_ppac_neutron_bg->Fill(energy, excitation,-bg_param);
                                    break;
                                }

                                case is_background : {
                                    //Subtract
                                    exgam_ppac_neutron->Fill(energy, excitation,bg_param);
                                    exgam_ppac_neutron_bg->Fill(energy, excitation,-bg_param);
                                    break;
                                }

                                case ignore :{
                                    break;
                                }
                            }
                        }
                    }
                    break;
                }

                case ignore : {
                break;
                }
            }

        }

    }




    // Creating fission veto spectrum
    for (int i = 0 ; i < NUM_LABR_DETECTORS ; ++i){
        for (int j = 0 ; j < event.n_labr[i] ; ++j){

            double energy = CalibrateE(event.w_labr[i][j]);
            double tdiff = CalcTimediff(de_word, event.w_labr[i][j]);
            bool ppac_prompt =  false;

            for (int n = 0 ; n < NUM_PPAC ; ++n){
                for (int m = 0 ; m < event.n_ppac[n] ; ++m){

                    double tdiff_ppac = CalcTimediff(de_word, event.w_ppac[n][m]);
                    energy_time_ppac[n]->Fill(energy, tdiff_ppac);

                    switch ( CheckTimeStatus(tdiff_ppac, ppac_time_cuts) ) {
                        case is_prompt : {
                            ppac_prompt = true;
                            break;
                        }
                            case is_background : {
                            break;
                        }
                        case ignore : {
                            break;
                        }
                    }
                }
            }

            switch ( CheckTimeStatus(tdiff, labr_time_cuts) ) {
                case is_prompt : {
                    if (ppac_prompt)
                        break;
                    else
                        exgam_veto_ppac->Fill(energy, excitation);
                    break;
                }
                case is_background : {
                    if (ppac_prompt){
                        break;
                    }
                    else {
                        exgam_veto_ppac->Fill(energy, excitation, bg_param);
                        exgam_veto_ppac_bg->Fill(energy, excitation);
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
