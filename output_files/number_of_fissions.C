void number_of_fissions(){
	TFile *file = TFile::Open("Pu_fiss.root");

	TH1 *number_of_fissions = (TH1 *)file->Get("number_of_fissions");

	Double_t lastbin = number_of_fissions->GetNbinsX();

	ofstream myfile;
	myfile.open("number_of_fissions.dat");

	for(int i=0;i<lastbin;i++){
		myfile << number_of_fissions->GetBinCenter(i) << " " << number_of_fissions->GetBinContent(i) << "\n";
	}

	myfile.close();

}