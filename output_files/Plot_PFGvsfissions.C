
void Plot_PFGvsfissions(){
	TFile *file = TFile::Open("Pu_all_looseEDEtimegate_lowexcorr_bg1_allbgsub_3mar2019/Pu_all_looseEDEtimegate_lowexcorr_bg1_allbgsub_3mar2019.root");

	//both number_of_fissions and exgam_ppac have ex-binwidths of 10keV
	double scale_factor = 1000/10;

	TH2 *exgam_ppac = (TH2 *)file->Get("exgam_ppac");

	TH1D* exgam_ppac_py = exgam_ppac->ProjectionY();
	exgam_ppac_py->GetXaxis()->SetLabelSize(0.037);
	exgam_ppac_py->GetYaxis()->SetLabelSize(0.037);
	exgam_ppac_py->GetYaxis()->SetTitleSize(0.040);
	exgam_ppac_py->GetXaxis()->SetTitleSize(0.040);
	exgam_ppac_py->Scale(scale_factor);
	exgam_ppac_py->GetYaxis()->SetRangeUser(1000, 2000000);
	exgam_ppac_py->GetXaxis()->SetRangeUser(2000, 12000);
	exgam_ppac_py->Draw();
	exgam_ppac_py->SetTitle(" ");
	exgam_ppac_py->GetXaxis()->SetTitle("E_{x} [keV]");
	exgam_ppac_py->GetYaxis()->SetTitle("Counts/MeV");
	gPad->SetLogy();
	gStyle->SetOptStat(0);
	TH1D *number_of_fissions= (TH1D *)file->Get("number_of_fissions");	
	number_of_fissions->SetLineColor(2);
	number_of_fissions->Scale(scale_factor);
	number_of_fissions->Draw("same");

	TLine *line = new TLine(5400,0,5400,2000000);
	line->SetLineStyle(2);
	line->Draw("same");
	TLine *line_1 = new TLine(6100,0,6100,2000000);
	line_1->SetLineStyle(9);
	line_1->Draw("same");

	auto legend = new TLegend(0.5,0.6,0.88,0.9);
	legend->AddEntry(line_1,"B_{f,A}","l");
	legend->AddEntry(line,"B_{f,B}","l");
	legend->AddEntry(exgam_ppac_py,"^{240}Pu(d,pf#gamma)","l");
    legend->AddEntry(number_of_fissions,"^{240}Pu(d,pf)","l");
   legend->Draw("same");

}