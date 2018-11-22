
void PlotExEgamma(){
	TFile *file = TFile::Open("Pu_all_newbg_Ecorr_noexcorr_looseEDEtimegate_noparticlegate_20nov2018.root");

	TH2 *exgam_veto_ppac = (TH2 *)file->Get("exgam_veto_ppac");

	exgam_veto_ppac->Draw();
	gPad->SetLogz();


	//TF1 f("Linear law","[0]+x*[1]",.5,10.5);

	//f.Draw();

	TLine *line = new TLine(0,0,10000,10000);
	line->SetLineColor(kRed);
  	line->Draw();
}