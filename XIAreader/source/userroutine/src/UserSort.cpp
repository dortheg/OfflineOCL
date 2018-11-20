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
const int GATING_EDE_TIME = 1;

//If line up E-detectors better
const int E_LINEUP = 1;

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
        return gain_labr[info.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_labr[info.detectorNum];
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
    //change b1f1
//    if(info_de.detectorNum==9){
//        return 1.04783577*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) - 648.80515;
//    }

    if(info_de.detectorNum==0 && E_LINEUP==1){
        return 1.00852659251*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) - 126.310420215;
    }

    if(info_de.detectorNum==1 && E_LINEUP==1){
        return 1.01054405216*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) - 158.366926658;
    }

    if(info_de.detectorNum==2 && E_LINEUP==1){
        return 1.00801130021*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) -128.664260047;
    }

    if(info_de.detectorNum==3 && E_LINEUP==1){
        return 1.00758122521*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) -125.873456448;
    }

    if(info_de.detectorNum==4 && E_LINEUP==1){
        return 1.00229000251*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) -55.5632838676;
    }

    if(info_de.detectorNum==5 && E_LINEUP==1){
        return 0.997998582917*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) -30.4900005565;
    }

    if(info_de.detectorNum==6  && E_LINEUP==1){
        return 1.00839105694*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) -152.122119317;
    }

    if(info_de.detectorNum==7  && E_LINEUP==1){
        return 1.01503221179*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) -235.394487516;
    }

    if(info_de.detectorNum==8 && E_LINEUP==1){
        return 1.00178957408*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) - 29.2556309974;
    }

    if(info_de.detectorNum==9 && E_LINEUP==1){
        return 1.0321774536*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) - 403.996969179;
    }

    if(info_de.detectorNum==10 && E_LINEUP==1){
        return 1.00193207639*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) -54.8228538909;
    }

    if(info_de.detectorNum==11 && E_LINEUP==1){
        return 1.00809055478*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) -120.242054039;
    }

    if(info_de.detectorNum==12 && E_LINEUP==1){
        return 1.00375191173*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) -82.0253833215;
    }

    if(info_de.detectorNum==13 && E_LINEUP==1){
        return 0.987809637357*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) + 92.6423012963;
    }

    if(info_de.detectorNum==14 && E_LINEUP==1){
        return 0.99559803607*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) + 43.6162924804;
    }

    if(info_de.detectorNum==15 && E_LINEUP==1){
        return 0.998454893818*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) -18.1451041661;
    }

    if(info_de.detectorNum==16 && E_LINEUP==1){
        return 0.996832946174*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) + 31.68451201;
    }

    if(info_de.detectorNum==17 && E_LINEUP==1){
        return 0.998898923042*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) - 12.6530158614;
    }

    if(info_de.detectorNum==18 && E_LINEUP==1){
        return 0.995652174499*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) + 27.1425106722;
    }

    if(info_de.detectorNum==19 && E_LINEUP==1){
        return 0.999551482345*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) -4.3686676003;
    }

    if(info_de.detectorNum==20 && E_LINEUP==1){
        return 0.99897872031*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) -12.8217950663;
    }

    if(info_de.detectorNum==21 && E_LINEUP==1){
        return 0.981477213244*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) + 184.068164129;
    }

    if(info_de.detectorNum==22 && E_LINEUP==1){
        return 1.00933198119*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) -121.398336087;
    }

    if(info_de.detectorNum==23 && E_LINEUP==1){
        return 0.993458785702*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) + 48.4303638799;
    }

    if(info_de.detectorNum==24 && E_LINEUP==1){
        return 1.00403364728*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) - 53.4903885192;
    }

    if(info_de.detectorNum==25 && E_LINEUP==1){
        return 0.990438093594*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) + 74.3410216251;
    }

    if(info_de.detectorNum==26 && E_LINEUP==1){
        return 0.987759178021*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) + 126.427018636;
    }

    if(info_de.detectorNum==27 && E_LINEUP==1){
        return 0.995683828449*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) + 13.2894955223 ;
    }

    if(info_de.detectorNum==28 && E_LINEUP==1){
        return 1.00413068604*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) -72.4761800398 ;
    }

    if(info_de.detectorNum==29 && E_LINEUP==1){
        return 1.00358799658 *(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) -75.6579320917;
    }

    if(info_de.detectorNum==30 && E_LINEUP==1){
        return 0.992315003843 *(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) + 68.1170954517;
    }

    if(info_de.detectorNum==31 && E_LINEUP==1){
        return 0.981472706182*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) + 175.58645599;
    }

    if(info_de.detectorNum==32 && E_LINEUP==1){
        return 0.986040657504*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) + 150.77083862;
    }

    if(info_de.detectorNum==33 && E_LINEUP==1){
        return 0.993448618731*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) + 42.0272218719;
    }

    if(info_de.detectorNum==34 && E_LINEUP==1){
        return 1.01333994917*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) -175.290543003;
    }

    if(info_de.detectorNum==35 && E_LINEUP==1){
        return 0.997327035407*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) + 5.91402582213;
    }

    if(info_de.detectorNum==36 && E_LINEUP==1){
        return 0.993533000697*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) + 50.5610921715;
    }

    if(info_de.detectorNum==37 && E_LINEUP==1){
        return 0.981814822113*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) + 186.315211596;
    }

    if(info_de.detectorNum==38 && E_LINEUP==1){
        return 0.992955095889*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) + 65.9950278342;
    }

    if(info_de.detectorNum==39 && E_LINEUP==1){
        return 1.00768707053*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) -94.8285222686;
    }

    if(info_de.detectorNum==40 && E_LINEUP==1){
        return 0.991999125656*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) + 54.0064952741;
    }

    if(info_de.detectorNum==41 && E_LINEUP==1){
        return 0.993399118314*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) + 30.0596248577;
    }

    if(info_de.detectorNum==42 && E_LINEUP==1){
        return 0.99431139601*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) + 9.89494971722;
    }

    if(info_de.detectorNum==43 && E_LINEUP==1){
        return 0.990958851152*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) + 50.2971578498;
    }

    if(info_de.detectorNum==44 && E_LINEUP==1){
        return 0.994201179914*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) +29.2330367709;
    }

    if(info_de.detectorNum==45 && E_LINEUP==1){
        return 0.991190952747*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) +54.1993695047 ;
    }

    if(info_de.detectorNum==46 && E_LINEUP==1){
        return 0.989983205946*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) +85.8159667554;
    }

    if(info_de.detectorNum==47 && E_LINEUP==1){
        return 0.988122741765*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) +84.4621694597;
    }

    if(info_de.detectorNum==48 && E_LINEUP==1){
        return 0.995241276383*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) + 39.1397838828;
    }

    if(info_de.detectorNum==49 && E_LINEUP==1){
        return 1.00944399408*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) -137.165520061;
    }

    if(info_de.detectorNum==50 && E_LINEUP==1){
        return 1.00887849447*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) -129.318208685;
    }

    if(info_de.detectorNum==51 && E_LINEUP==1){
        return 1.00022827766*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) -27.8703999905;
    }

    if(info_de.detectorNum==52 && E_LINEUP==1){
        return 0.991952750385*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) + 57.3709240994;
    }

    if(info_de.detectorNum==53 && E_LINEUP==1){
        return 0.98971797548*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) + 77.4867227562;
    }

    if(info_de.detectorNum==54 && E_LINEUP==1){
        return 1.01144910412*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) -162.844397235;
    }

    if(info_de.detectorNum==55 && E_LINEUP==1){
        return 1.0159836277*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) -203.498124961;
    }

    if(info_de.detectorNum==56 && E_LINEUP==1){
        return 0.996359888104*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) + 33.9283061123;
    }

    if(info_de.detectorNum==57 && E_LINEUP==1){
        return 1.00087972113*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) - 35.2200002207;
    }

    if(info_de.detectorNum==58 && E_LINEUP==1){
        return 0.999245325548*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) -7.20824790206;
    }

    if(info_de.detectorNum==59 && E_LINEUP==1){
        return 0.999977505892*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) -12.3884230434;
    }

    if(info_de.detectorNum==60 && E_LINEUP==1){
        return 0.998104395349*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) + 1.26761444883;
    }

    if(info_de.detectorNum==61 && E_LINEUP==1){
        return 1.01402956987*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) -161.2242491;
    }

    if(info_de.detectorNum==62 && E_LINEUP==1){
        return 1.00275702277*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) -32.2508546044;
    }

    if(info_de.detectorNum==63 && E_LINEUP==1){
        return 1.00872623673*(gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum]) -111.761651903;
    }

    else{
        return gain_E[info_de.detectorNum]*(w.adcdata + drand48() - 0.5) + shift_E[info_de.detectorNum];
    }
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

    sprintf(tmp, "number_of_fissions");
    number_of_fissions = Spec(tmp, tmp, 15, 0, 15000, "Ex energy [MeV]");

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

    sprintf(tmp, "energy_labr_all_separate");
    energy_labr_all_separate = Spec(tmp, tmp, 10000, 0, 10000, "Energy [keV]");

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

    sprintf(tmp, "ede_all_fission_nobgsub");
    sprintf(tmp2, "E : DE, all");
    ede_all_fission_nobgsub = Mat(tmp, tmp2, 10000, 0, 20000, "Back energy [keV]", 1000, 0, 5000, "Front energy [keV]");

    sprintf(tmp, "ede_all_fission_bg");
    sprintf(tmp2, "E : DE, all");
    ede_all_fission_bg = Mat(tmp, tmp2, 10000, 0, 20000, "Back energy [keV]", 1000, 0, 5000, "Front energy [keV]");

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
        double E1_E = 8400.0;
        double E2_E = 9000.0;
        double E1_DE = 100.0;
        double E2_DE = 4000.0;
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
            double y_upper = 0.0062125*e_tot + 205.926;
            double y_lower = 0.0058352*e_tot + 60;

            if (tdiff_ede < y_upper && tdiff_ede > y_lower && GATING_EDE_TIME==1 ){
                 energy_particle_time_e_de_all_gate->Fill(e_tot, tdiff_ede);
                 ede_gate->Fill(e_energy, de_energy);
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

            //Here change ex to ex_new! In order to make the known peaks fit ex-energies
            //ex = 1.0458313*ex + 2.88477418; //low
            //ex = 1.08774584*ex + 20.31316188; //high


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


    float bg_param = -0.94;

    // Things with PPAC
    for (int i = 0 ; i < NUM_PPAC ; ++i){
        for (int j = 0 ; j < event.n_ppac[i] ; ++j){

            double tdiff = CalcTimediff(de_word, event.w_ppac[i][j]);
            excitation_time_ppac[i]->Fill(excitation, tdiff);
            //Dortheas attempt
            ppac_vs_dE_time->Fill(tdiff,i);

        }
    }

    //fission counting
    for (int n = 0 ; n < NUM_PPAC ; ++n){
        for (int m = 0 ; m < event.n_ppac[n] ; ++m){
            //All events in ppacs

            double tdiff_ppac = CalcTimediff(de_word, event.w_ppac[n][m]);

            switch ( CheckTimeStatus(tdiff_ppac, ppac_time_cuts) ) {
                case is_prompt : {
                    number_of_fissions->Fill(excitation);
                    break;
                }
                case is_background : {
                    number_of_fissions->Fill(excitation, -1);
                    break;
                }
                case ignore : {
                    break;
                }
            }
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

            int ppac_prompt = 0;

            //Is this correct? Doesnt it loop through all events, and changes ppac_prompt between true and false?
            //
            for (int n = 0 ; n < NUM_PPAC ; ++n){
                //std::cout << "m: " << event.n_ppac[n] << std::endl;
                //Number of PPAC
                for (int m = 0 ; m < event.n_ppac[n] ; ++m){
                    //All events in ppacs

                    //double tdiff_ppac = CalcTimediff(event.w_labr[0][0], event.w_ppac[n][m]);
                    //double tdiff_ppac = CalcTimediff(event.w_labr[i][j], event.w_ppac[n][m]);
                    double tdiff_ppac = CalcTimediff(de_word, event.w_ppac[n][m]);

                    ppac_align_time_all->Fill(tdiff_ppac, n);
                    energy_time_ppac[n]->Fill(energy, tdiff_ppac); //if aligned, can use this for PFG/PFN separations


                    switch ( CheckTimeStatus(tdiff_ppac, ppac_time_cuts) ) {
//                        tdiff_ppac: time diff between ppac and Labr3, definere fisjon
                        case is_prompt : {
                            //std::cout << "true" << std::endl;
                            ppac_prompt = 1;
                            break;
                        }
                        case is_background : {
                            //std::cout << "false" << std::endl;
                            ppac_prompt = 2;
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
                    double E1 = 8400.0;
                    double E2 = 9000.0;
                    double DE = de_word.adcdata;
                    double DE1 = 100.0;
                    double DE2 = 4000.0;
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
                    double y_upper = 0.0062125*e_tot + 205.926;
                    double y_lower = 0.0058352*e_tot + 60;

                    //Cut in energy_particle_time
                    if (tdiff_ede < y_upper && e_tot>4000 && tdiff_ede > y_lower && GATING_EDE_TIME==1){
                        exgam->Fill(energy, excitation);
                        if (ppac_prompt==1){
                            //de, labr and ppac are prompt->Fission
                            ede_all_fission->Fill(e_energy, de_energy);
                            ede_all_fission_nobgsub->Fill(e_energy, de_energy);
                            exgam_ppac->Fill(energy, excitation);
                        }
                        else{
                            //de, labr are prompt, but not ppac -> not fission
                            exgam_veto_ppac->Fill(energy, excitation);

                            if(ppac_prompt==2){
                                ede_all_fission->Fill(e_energy, de_energy, bg_param);
                                ede_all_fission_bg->Fill(e_energy, de_energy);
                                exgam_ppac->Fill(energy, excitation, bg_param);
                                exgam_ppac_bg->Fill(energy, excitation);
                            }
                        }
                        break;

                    }

                    else if(GATING_EDE_TIME==0){
                        exgam->Fill(energy, excitation);
                        if (ppac_prompt==1){
                            //de, labr and ppac are prompt->Fission
                            ede_all_fission->Fill(e_energy, de_energy);
                            ede_all_fission_nobgsub->Fill(e_energy, de_energy);
                            exgam_ppac->Fill(energy, excitation);
                        }
                        else{
                            //de, labr are prompt, but not ppac -> not fission
                            ede_all_nofission->Fill(e_energy, de_energy);
                            exgam_veto_ppac->Fill(energy, excitation);

                            if(ppac_prompt==2){
                                ede_all_fission->Fill(e_energy, de_energy, bg_param);
                                ede_all_fission_bg->Fill(e_energy, de_energy);
                                exgam_ppac->Fill(energy, excitation, bg_param);
                                exgam_ppac_bg->Fill(energy, excitation);
                            }
                        }
                        break;

                    }

                }
                case is_background : {
                word_t e_word = event.trigger;
                double E = e_word.adcdata;
                double E1 = 8400.0;
                double E2 = 9000.0;
                double DE = de_word.adcdata;
                double DE1 = 100.0;
                double DE2 = 4000.0;
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
                double y_upper = 0.0062125*e_tot + 205.926;
                double y_lower = 0.0058352*e_tot + 60;

                if (tdiff_ede < y_upper && e_tot>4000 && tdiff_ede > y_lower && GATING_EDE_TIME==1){
                    exgam->Fill(energy, excitation, -1);
                    exgam_bg->Fill(energy, excitation);

                    if (ppac_prompt==1){
                        exgam_ppac->Fill(energy, excitation, bg_param);
                        exgam_ppac_bg->Fill(energy, excitation);
                        ede_all_fission->Fill(e_energy, de_energy, bg_param);
                        ede_all_fission_bg->Fill(e_energy, de_energy);
                    }

                    else {
                        if(ppac_prompt==2){
                            ede_all_fission->Fill(e_energy, de_energy, bg_param);
                            ede_all_fission_bg->Fill(e_energy, de_energy);
                            exgam_ppac->Fill(energy, excitation, bg_param);
                            exgam_ppac_bg->Fill(energy, excitation);
                        }

                        exgam_veto_ppac->Fill(energy, excitation, -1);
                        exgam_veto_ppac_bg->Fill(energy, excitation);
                    }
                    break;
                }

                else if(GATING_EDE_TIME==0){
                    exgam->Fill(energy, excitation, -1);
                    exgam_bg->Fill(energy, excitation);
                    if (ppac_prompt==1){
                        exgam_ppac->Fill(energy, excitation, bg_param);
                        exgam_ppac_bg->Fill(energy, excitation);
                        ede_all_fission->Fill(e_energy, de_energy, bg_param);
                        ede_all_fission_bg->Fill(e_energy, de_energy);
                    }

                    else {

                        if(ppac_prompt==2){
                            ede_all_fission->Fill(e_energy, de_energy, bg_param);
                            ede_all_fission_bg->Fill(e_energy, de_energy);
                            exgam_ppac->Fill(energy, excitation, bg_param);
                            exgam_ppac_bg->Fill(energy, excitation);
                        }

                        exgam_veto_ppac->Fill(energy, excitation, -1);
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
