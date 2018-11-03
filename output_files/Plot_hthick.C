
void Plot_hthick(){
	TFile *file = TFile::Open("Pu_2nov_lowexfit.root");

	TH2 *h_thick = (TH2 *)file->Get("h_thick");

	h_thick->Draw();
	gPad->SetLogz();
	//h_thick->GetXaxis()->SetTitle("Photon multiplicity PM");
	h_thick->GetXaxis()->SetLabelSize(0.037);
	h_thick->GetYaxis()->SetLabelSize(0.039);
	h_thick->GetYaxis()->SetTitleSize(0.040);
	h_thick->GetXaxis()->SetTitleSize(0.040);
	gStyle->SetOptStat(0);
	h_thick->SetTitle(" ");
	h_thick->GetXaxis()->SetTitle("Apparent #DeltaE thickness [ #mum]");
	h_thick->GetYaxis()->SetTitle("Counts/bin");
	h_thick->GetXaxis()->SetRangeUser(50,350);

	TLine *line_1 = new TLine(110,0,110,14*1000000);
	line_1->SetLineColor(kRed);
  	line_1->Draw();

  	TLine *line_2 = new TLine(170,0,170,14*1000000);
	line_2->SetLineColor(kRed);
  	line_2->Draw();
}