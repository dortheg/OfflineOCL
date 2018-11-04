
void Plot_labrtime(){
	TFile *file = TFile::Open("Pu_113054_after_time_align.root");

	TH2 *labr_align_time_ = (TH2 *)file->Get("labr_align_time");

	//labr_align_time->Draw();
	TH1 *labr_align_time = labr_align_time_->ProjectionX();
	//gPad->SetLogy();
	//labr_align_time->GetXaxis()->SetTitle("Photon multiplicity PM");
	labr_align_time->GetXaxis()->SetLabelSize(0.040);
	labr_align_time->GetYaxis()->SetLabelSize(0.040);
	labr_align_time->GetYaxis()->SetTitleSize(0.040);
	labr_align_time->GetXaxis()->SetTitleSize(0.040);
	labr_align_time->GetZaxis()->SetLabelSize(0.037);
	gStyle->SetOptStat(0);
	labr_align_time->SetTitle(" ");
	labr_align_time->GetXaxis()->SetTitle("#Delta t [ns]");
	labr_align_time->GetYaxis()->SetTitle("LaBr_{3}  detector number");
	labr_align_time->GetXaxis()->SetRangeUser(-70,70);
	//labr_align_time->GetYaxis()->SetRangeUser(0,100000);
	labr_align_time->Draw();


	TLine *line_1 = new TLine(-3,0,-3,2000000);
	line_1->SetLineColor(kRed);
  	line_1->Draw("same");

  	TLine *line_2 = new TLine(3,0,3,2000000);
	line_2->SetLineColor(kRed);
  	line_2->Draw("same");

  	TLine *line_3 = new TLine(-48,0,-48,2000000);
	line_3->SetLineColor(kGreen);
  	line_3->Draw("same");

  	TLine *line_4 = new TLine(-42,0,-42,2000000);
	line_4->SetLineColor(kGreen);
  	line_4->Draw("same");
}