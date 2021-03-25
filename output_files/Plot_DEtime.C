
void Plot_DEtime(){
	TFile *file = TFile::Open("Pu_113054_before_time_align.root");

	TH2 *de_align_time = (TH2 *)file->Get("de_align_time");

	de_align_time->Draw();
	//gPad->SetLogy();
	//de_align_time->GetXaxis()->SetTitle("Photon multiplicity PM");
	de_align_time->GetXaxis()->SetLabelSize(0.037);
	de_align_time->GetYaxis()->SetLabelSize(0.037);
	de_align_time->GetYaxis()->SetTitleSize(0.041);
	de_align_time->GetXaxis()->SetTitleSize(0.041);
	de_align_time->GetZaxis()->SetLabelSize(0.037);
	gStyle->SetOptStat(0);
	de_align_time->SetTitle(" ");
	de_align_time->GetXaxis()->SetTitle("#Delta t [ns]");
	de_align_time->GetYaxis()->SetTitle("#DeltaE detector number");
	//de_align_time->GetXaxis()->SetRangeUser(0,7000);
	//de_align_time->GetYaxis()->SetRangeUser(0,100000);
}