
void Plot_EDE(){
	TFile *file = TFile::Open("Pu_3nov_noparticlegate_loose_E_DE_time_gate.root");

	TH2 *ede_gate = (TH2 *)file->Get("ede_gate");

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
	ede_gate->GetYaxis()->SetRangeUser(500,2000);
	ede_gate->GetXaxis()->SetRangeUser(10000,17500);
}
