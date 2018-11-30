
void PlotExEgamma(){
	TFile *file = TFile::Open("Pu_all_properfisscount_bg1_Ecorr_lowexcorr_looseEDEtimegate_protongate_28nov2018/Pu_all_properfisscount_bg1_Ecorr_lowexcorr_looseEDEtimegate_protongate_moreplots_28nov2018.root");

	TH2 *exgam_veto_ppac = (TH2 *)file->Get("exgam_veto_ppac");

	exgam_veto_ppac->Draw();
	gPad->SetLogz();


	//TF1 f("Linear law","[0]+x*[1]",.5,10.5);

	//f.Draw();

	TLine *line = new TLine(0,0,10000,10000);
	line->SetLineColor(kRed);
  	line->Draw();
}