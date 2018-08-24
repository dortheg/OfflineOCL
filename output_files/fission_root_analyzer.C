#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <TROOT.h>
#include <iostream>
#include <cstdio>
#include "TFile.h"
#include "TChain.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>

///////////////
// To be used in Dorthea's master, to analyze the data output from the experiment
// based on freya_root_analyzer.C, used to analyze FREYA output files
//
// Includes:
// - Mean gamma multiplicity as function of excitation energy
// - Total gamma energy as function of excitation energy
// - Mean gamma energy as function of excitation energy
//
//////////////////////////


TFile *vetsex = new TFile("Pu_test.root", "READ");
TTree *mytree = (TTree *) gROOT->FindObject("FreyaTree");

//
Double_t pi = 3.14159265359;

//Photon emission multiplicities
TH1D *hframe_p_mult_0;
TH1D *hframe_p_mult_1;
TH1D *hframe_p_mult_2;
TH2D *hframe_p_multi;
TH3F *hframe_p_multi3D;
TH1D *h_p_mult_total;
int nbins_h_p_mult_total  = 20;

TH1D *hframe_ph_E_0;
TH1D *hframe_ph_E_1;
TH1D *hframe_ph_E_2;
TH1D *h_ph_E_total;
// TH1D *h_n_E_Boltzmann;
int nbins_h_ph_E_total  = 500;
int max_h_ph_E  = 7; // in MeV

void create_frames();

void fission_root_analyzer() {

	create_frames();

	TCanvas *c1 = new TCanvas("c1","Fragment Yield",150,10,990,660);
	

}


void create_frames() {
gStyle->SetOptStat(0);
int nbins;
int maxbin;


nbins = nbins_h_p_mult_total;
maxbin = nbins;
hframe_p_mult_0 = new TH1D("hframe_p_mult_0","",nbins,-0.5,maxbin-.5);
hframe_p_mult_1 = new TH1D("hframe_p_mult_1","",nbins,-0.5,maxbin-.5);
hframe_p_mult_2 = new TH1D("hframe_p_mult_2","",nbins,-0.5,maxbin-0.5);
hframe_p_multi  = new TH2D("hframe_p_multi","",nbins,-0.5,maxbin-0.5,nbins,-0.5,maxbin-0.5);
hframe_p_multi3D  = new TH3F("hframe_p_multi3D","",nbins,-0.5,maxbin-0.5,nbins,-0.5,maxbin-0.5,nbins,-0.5,maxbin-0.5);
h_p_mult_total = new TH1D("h_p_mult_total","",nbins,-0.5,maxbin-.5);

nbins = nbins_h_ph_E_total;
maxbin = max_h_ph_E;
hframe_ph_E_0 = new TH1D("hframe_ph_E_0","",nbins,0,maxbin);
hframe_ph_E_1 = new TH1D("hframe_ph_E_1","",nbins,0,maxbin);
hframe_ph_E_2 = new TH1D("hframe_ph_E_2","",nbins,0,maxbin);
h_ph_E_total = new TH1D("h_ph_E_total","",nbins,0,maxbin);

}
