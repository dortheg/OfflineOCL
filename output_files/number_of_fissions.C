void number_of_fissions(){
	TFile *file = TFile::Open("*Pu_all_newbg_1_Ecorr_noexcorr_looseEDEtimegate_protongate_22nov2018.root");

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

}