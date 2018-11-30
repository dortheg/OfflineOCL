
void Plot_EDE(){
	TFile *file = TFile::Open("Pu_all_newbg_Ecorr_noexcorr_looseEDEtimegate_noparticlegate_20nov2018.root");

	TH2 *ede_gate = (TH2 *)file->Get("ede_all_fission");

	ede_gate->Draw();
	gPad->SetLogz();
	//ede_gate->GetXaxis()->SetTitle("Photon multiplicity PM");
	ede_gate->GetXaxis()->SetLabelSize(0.037);
	ede_gate->GetYaxis()->SetLabelSize(0.037);
	ede_gate->GetYaxis()->SetTitleSize(0.040);
	ede_gate->GetXaxis()->SetTitleSize(0.040);
	ede_gate->GetZaxis()->SetLabelSize(0.037);
	gStyle->SetOptStat(0);
	ede_gate->SetTitle(" ");
	ede_gate->GetXaxis()->SetTitle("E [keV]");
	ede_gate->GetYaxis()->SetTitle("#Delta E [keV]");
	ede_gate->GetYaxis()->SetRangeUser(500,5000);
	//ede_gate->GetXaxis()->SetRangeUser(10000,17500);

	// TH2 *ede_gate = (TH2 *)file->Get("ede_raw_b0_f0");

	// ede_gate->Draw();
	// gPad->SetLogz();
	// //ede_gate->GetXaxis()->SetTitle("Photon multiplicity PM");
	// ede_gate->GetXaxis()->SetLabelSize(0.037);
	// ede_gate->GetYaxis()->SetLabelSize(0.037);
	// ede_gate->GetYaxis()->SetTitleSize(0.040);
	// ede_gate->GetXaxis()->SetTitleSize(0.040);
	// ede_gate->GetZaxis()->SetLabelSize(0.037);
	// gStyle->SetOptStat(0);
	// ede_gate->SetTitle(" ");
	// ede_gate->GetXaxis()->SetTitle("E [channel]");
	// ede_gate->GetYaxis()->SetTitle("#Delta E [channel]");
	// ede_gate->GetYaxis()->SetRangeUser(1000,9000);
	// ede_gate->GetXaxis()->SetRangeUser(0,10000);




}
