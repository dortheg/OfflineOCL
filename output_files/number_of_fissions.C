void number_of_fissions(){
	TFile *file = TFile::Open("Pu_all_looseEDEtimegate_lowexcorr_bg1_allbgsub_3mar2019/Pu_all_looseEDEtimegate_lowexcorr_bg1_allbgsub_3mar2019.root");

	TH1 *number_of_fissions = (TH1 *)file->Get("number_of_fissions");

	Double_t lastbin = number_of_fissions->GetNbinsX();

	ofstream myfile;
	myfile.open("number_of_fissions.dat");

	float F = 0;

	for(int i=0;i<lastbin+1;i++){
		F += number_of_fissions->GetBinContent(i);
	}

	myfile << "Total number of fissions: " << F << "\n";

	for(int i=0;i<lastbin+1;i++){
		myfile << number_of_fissions->GetBinCenter(i) << " " << number_of_fissions->GetBinContent(i) << "\n";
	}

	myfile.close();

	number_of_fissions->Draw();
	number_of_fissions->GetXaxis()->SetLabelSize(0.044);
	number_of_fissions->GetYaxis()->SetLabelSize(0.044);
	number_of_fissions->GetYaxis()->SetTitleSize(0.044);
	number_of_fissions->GetXaxis()->SetTitleSize(0.044);
	number_of_fissions->GetZaxis()->SetLabelSize(0.044);
	number_of_fissions->GetYaxis()->SetTitleOffset(1.1);
	gStyle->SetOptStat(0);
	number_of_fissions->SetTitle(" ");
	number_of_fissions->GetXaxis()->SetTitle("Excitation energy [keV]");
	number_of_fissions->GetYaxis()->SetTitle("Counts/bin");
	number_of_fissions->GetXaxis()->SetRangeUser(3000,12000);

}