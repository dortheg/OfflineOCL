
void Plot_PFGvsfissions(){
	TFile *file = TFile::Open("test.root");

	TH2 *exgam_ppac = (TH2 *)file->Get("exgam_ppac_newfiss");

	TH1D* exgam_ppac_py = exgam_ppac->ProjectionY();
	exgam_ppac_py->Draw();
	gPad->SetLogy();

	TH1D *number_of_fissions= (TH1D *)file->Get("number_of_fissions");	
	number_of_fissions->SetLineColor(2);
	number_of_fissions->Draw("same");

}