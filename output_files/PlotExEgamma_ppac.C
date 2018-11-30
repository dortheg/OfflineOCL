
void PlotExEgamma_ppac(){
	TFile *file = TFile::Open("Pu_all_properfisscount_bg1_Ecorr_lowexcorr_looseEDEtimegate_protongate_28nov2018/Pu_all_properfisscount_bg1_Ecorr_lowexcorr_looseEDEtimegate_protongate_moreplots_28nov2018.root");

	TH2 *exgam_ppac = (TH2 *)file->Get("exgam_ppac");

	exgam_ppac->Draw();
	gPad->SetLogz();


	//TF1 f("Linear law","[0]+x*[1]",.5,10.5);

	//f.Draw();

	TLine *line = new TLine(0,5400,10000, 5400);
	line->SetLineColor(kRed);
	TLine *line_1 = new TLine(0,6140,10000, 6140);
	line_1->SetLineColor(kRed);
  	line->Draw();
  	line_1->Draw();

  	gStyle->SetOptStat(0);
	exgam_ppac->SetTitle(" ");
	exgam_ppac->GetXaxis()->SetLabelSize(0.037);
	exgam_ppac->GetYaxis()->SetLabelSize(0.037);
	exgam_ppac->GetYaxis()->SetTitleSize(0.040);
	exgam_ppac->GetXaxis()->SetTitleSize(0.040);
	exgam_ppac->GetZaxis()->SetLabelSize(0.037);
	exgam_ppac->GetXaxis()->SetTitle("E#gamma [keV]");
	exgam_ppac->GetYaxis()->SetTitle("Ex [keV]");
	exgam_ppac->GetYaxis()->SetRangeUser(0,13000);
	exgam_ppac->GetXaxis()->SetRangeUser(0,10000);
}