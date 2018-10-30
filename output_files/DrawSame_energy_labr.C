
void DrawSame_energy_labr(){

	TFile *file = TFile::Open("Pu_all_25oct2018_notimegate.root");

	TH2 *energy_labr_01 = (TH2 *)file->Get("energy_labr_01");
	TH2 *energy_labr_02 = (TH2 *)file->Get("energy_labr_02");
	TH2 *energy_labr_03 = (TH2 *)file->Get("energy_labr_03");
	TH2 *energy_labr_04 = (TH2 *)file->Get("energy_labr_04");
	TH2 *energy_labr_05 = (TH2 *)file->Get("energy_labr_05");
	TH2 *energy_labr_06 = (TH2 *)file->Get("energy_labr_06");
	TH2 *energy_labr_07 = (TH2 *)file->Get("energy_labr_07");
	TH2 *energy_labr_08 = (TH2 *)file->Get("energy_labr_08");
	TH2 *energy_labr_09 = (TH2 *)file->Get("energy_labr_09");
	TH2 *energy_labr_10 = (TH2 *)file->Get("energy_labr_10");

	TH2 *energy_labr_11 = (TH2 *)file->Get("energy_labr_11");
	TH2 *energy_labr_12 = (TH2 *)file->Get("energy_labr_12");
	TH2 *energy_labr_13 = (TH2 *)file->Get("energy_labr_13");
	TH2 *energy_labr_14 = (TH2 *)file->Get("energy_labr_14");
	TH2 *energy_labr_15 = (TH2 *)file->Get("energy_labr_15");
	TH2 *energy_labr_16 = (TH2 *)file->Get("energy_labr_16");
	TH2 *energy_labr_17 = (TH2 *)file->Get("energy_labr_17");
	TH2 *energy_labr_18 = (TH2 *)file->Get("energy_labr_18");
	TH2 *energy_labr_19 = (TH2 *)file->Get("energy_labr_19");
	TH2 *energy_labr_20 = (TH2 *)file->Get("energy_labr_20");

	TH2 *energy_labr_21 = (TH2 *)file->Get("energy_labr_21");
	TH2 *energy_labr_22 = (TH2 *)file->Get("energy_labr_22");
	TH2 *energy_labr_23 = (TH2 *)file->Get("energy_labr_23");
	TH2 *energy_labr_24 = (TH2 *)file->Get("energy_labr_24");
	TH2 *energy_labr_25 = (TH2 *)file->Get("energy_labr_25");
	TH2 *energy_labr_26 = (TH2 *)file->Get("energy_labr_26");
	TH2 *energy_labr_27 = (TH2 *)file->Get("energy_labr_27");
	TH2 *energy_labr_28 = (TH2 *)file->Get("energy_labr_28");
	TH2 *energy_labr_29 = (TH2 *)file->Get("energy_labr_29");
	TH2 *energy_labr_30 = (TH2 *)file->Get("energy_labr_30");

	TH2 *energy_labr_31 = (TH2 *)file->Get("energy_labr_31");
	TH2 *energy_labr_32 = (TH2 *)file->Get("energy_labr_32");

	energy_labr_01->SetLineColor(1);
	energy_labr_02->SetLineColor(3);
	energy_labr_03->SetLineColor(4);
	energy_labr_04->SetLineColor(5);
	energy_labr_05->SetLineColor(6);
	energy_labr_06->SetLineColor(7);
	energy_labr_07->SetLineColor(8);
	energy_labr_08->SetLineColor(9);
	energy_labr_09->SetLineColor(10);
	energy_labr_10->SetLineColor(11);

	energy_labr_11->SetLineColor(12);
	energy_labr_12->SetLineColor(13);
	energy_labr_13->SetLineColor(14);
	energy_labr_14->SetLineColor(15);
	energy_labr_15->SetLineColor(16);
	energy_labr_16->SetLineColor(17);
	energy_labr_17->SetLineColor(18);
	energy_labr_18->SetLineColor(19);
	energy_labr_19->SetLineColor(20);
	energy_labr_20->SetLineColor(21);

	energy_labr_21->SetLineColor(22);
	energy_labr_22->SetLineColor(228);
	energy_labr_23->SetLineColor(130);
	energy_labr_24->SetLineColor(209);
	energy_labr_25->SetLineColor(213);
	energy_labr_26->SetLineColor(207);
	energy_labr_27->SetLineColor(205);
	energy_labr_28->SetLineColor(226);
	energy_labr_29->SetLineColor(223);
	energy_labr_30->SetLineColor(221);

	energy_labr_31->SetLineColor(227);
	energy_labr_32->SetLineColor(210);

	int j = 1;

	energy_labr_01->Draw("apl");
	energy_labr_02->Draw("pl same");
	energy_labr_03->Draw("pl same");
	energy_labr_04->Draw("pl same");
	energy_labr_05->Draw("pl same");
	energy_labr_06->Draw("pl same");
	energy_labr_07->Draw("pl same");
	energy_labr_08->Draw("pl same");
	energy_labr_09->Draw("pl same");
	energy_labr_10->Draw("pl same");

	if(j==1){

	energy_labr_11->Draw("pl same");
	energy_labr_12->Draw("pl same");
	energy_labr_13->Draw("pl same");
	energy_labr_14->Draw("pl same");
	energy_labr_15->Draw("pl same");
	energy_labr_16->Draw("pl same");
	energy_labr_17->Draw("pl same");
	energy_labr_18->Draw("pl same");
	energy_labr_19->Draw("pl same");
	energy_labr_20->Draw("pl same");

	energy_labr_21->Draw("pl same");
	energy_labr_22->Draw("pl same");
	energy_labr_23->Draw("pl same");
	energy_labr_24->Draw("pl same");
	energy_labr_25->Draw("pl same");
	energy_labr_26->Draw("pl same");
	energy_labr_27->Draw("pl same");
	energy_labr_28->Draw("pl same");
	energy_labr_29->Draw("pl same");
	energy_labr_30->Draw("pl same");

	energy_labr_31->Draw("pl same");
	energy_labr_32->Draw("pl same");
	}



}