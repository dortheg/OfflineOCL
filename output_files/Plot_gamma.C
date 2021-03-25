
void Plot_gamma(){
	TFile *file = TFile::Open("Si_all.root");

	TH2 *energy_labr_raw = (TH2 *)file->Get("energy_raw_labr_01");

	energy_labr_raw->Draw();
	//gPad->SetLogy();
	//energy_labr_raw->GetXaxis()->SetTitle("Photon multiplicity PM");
	energy_labr_raw->GetXaxis()->SetLabelSize(0.037);
	energy_labr_raw->GetYaxis()->SetLabelSize(0.037);
	energy_labr_raw->GetYaxis()->SetTitleSize(0.040);
	energy_labr_raw->GetXaxis()->SetTitleSize(0.040);
	energy_labr_raw->GetZaxis()->SetLabelSize(0.037);
	gStyle->SetOptStat(0);
	energy_labr_raw->SetTitle(" ");
	energy_labr_raw->GetXaxis()->SetTitle("Energy [channel]");
	energy_labr_raw->GetYaxis()->SetTitle("Counts/bin");
	//energy_labr_raw->GetXaxis()->SetRangeUser(0,7000);
	//energy_labr_raw->GetYaxis()->SetRangeUser(0,100000);
}