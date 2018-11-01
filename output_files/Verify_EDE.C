void Verify_EDE(){
	TFile *file = TFile::Open("Pu_all_ring0_correction.root");

	TH2 *ede_b0_f0 = (TH2 *)file->Get("ede_b0_f0");
	TH2 *ede_b0_f1 = (TH2 *)file->Get("ede_b0_f1");
	TH2 *ede_b0_f2 = (TH2 *)file->Get("ede_b0_f2");
	TH2 *ede_b0_f3 = (TH2 *)file->Get("ede_b0_f3");
	TH2 *ede_b0_f4 = (TH2 *)file->Get("ede_b0_f4");
	TH2 *ede_b0_f5 = (TH2 *)file->Get("ede_b0_f5");
	TH2 *ede_b0_f6 = (TH2 *)file->Get("ede_b0_f6");
	TH2 *ede_b0_f7 = (TH2 *)file->Get("ede_b0_f7");

	TH2 *ede_b1_f0 = (TH2 *)file->Get("ede_b1_f0");
	TH2 *ede_b1_f1 = (TH2 *)file->Get("ede_b1_f1");
	TH2 *ede_b1_f2 = (TH2 *)file->Get("ede_b1_f2");
	TH2 *ede_b1_f3 = (TH2 *)file->Get("ede_b1_f3");
	TH2 *ede_b1_f4 = (TH2 *)file->Get("ede_b1_f4");
	TH2 *ede_b1_f5 = (TH2 *)file->Get("ede_b1_f5");
	TH2 *ede_b1_f6 = (TH2 *)file->Get("ede_b1_f6");
	TH2 *ede_b1_f7 = (TH2 *)file->Get("ede_b1_f7");

	TH2 *ede_b2_f0 = (TH2 *)file->Get("ede_b2_f0");
	TH2 *ede_b2_f1 = (TH2 *)file->Get("ede_b2_f1");
	TH2 *ede_b2_f2 = (TH2 *)file->Get("ede_b2_f2");
	TH2 *ede_b2_f3 = (TH2 *)file->Get("ede_b2_f3");
	TH2 *ede_b2_f4 = (TH2 *)file->Get("ede_b2_f4");
	TH2 *ede_b2_f5 = (TH2 *)file->Get("ede_b2_f5");
	TH2 *ede_b2_f6 = (TH2 *)file->Get("ede_b2_f6");
	TH2 *ede_b2_f7 = (TH2 *)file->Get("ede_b2_f7");

	TH2 *ede_b3_f0 = (TH2 *)file->Get("ede_b3_f0");
	TH2 *ede_b3_f1 = (TH2 *)file->Get("ede_b3_f1");
	TH2 *ede_b3_f2 = (TH2 *)file->Get("ede_b3_f2");
	TH2 *ede_b3_f3 = (TH2 *)file->Get("ede_b3_f3");
	TH2 *ede_b3_f4 = (TH2 *)file->Get("ede_b3_f4");
	TH2 *ede_b3_f5 = (TH2 *)file->Get("ede_b3_f5");
	TH2 *ede_b3_f6 = (TH2 *)file->Get("ede_b3_f6");
	TH2 *ede_b3_f7 = (TH2 *)file->Get("ede_b3_f7");

	TH2 *ede_b4_f0 = (TH2 *)file->Get("ede_b4_f0");
	TH2 *ede_b4_f1 = (TH2 *)file->Get("ede_b4_f1");
	TH2 *ede_b4_f2 = (TH2 *)file->Get("ede_b4_f2");
	TH2 *ede_b4_f3 = (TH2 *)file->Get("ede_b4_f3");
	TH2 *ede_b4_f4 = (TH2 *)file->Get("ede_b4_f4");
	TH2 *ede_b4_f5 = (TH2 *)file->Get("ede_b4_f5");
	TH2 *ede_b4_f6 = (TH2 *)file->Get("ede_b4_f6");
	TH2 *ede_b4_f7 = (TH2 *)file->Get("ede_b4_f7");

	TH2 *ede_b5_f0 = (TH2 *)file->Get("ede_b5_f0");
	TH2 *ede_b5_f1 = (TH2 *)file->Get("ede_b5_f1");
	TH2 *ede_b5_f2 = (TH2 *)file->Get("ede_b5_f2");
	TH2 *ede_b5_f3 = (TH2 *)file->Get("ede_b5_f3");
	TH2 *ede_b5_f4 = (TH2 *)file->Get("ede_b5_f4");
	TH2 *ede_b5_f5 = (TH2 *)file->Get("ede_b5_f5");
	TH2 *ede_b5_f6 = (TH2 *)file->Get("ede_b5_f6");
	TH2 *ede_b5_f7 = (TH2 *)file->Get("ede_b5_f7");

	TH2 *ede_b6_f0 = (TH2 *)file->Get("ede_b6_f0");
	TH2 *ede_b6_f1 = (TH2 *)file->Get("ede_b6_f1");
	TH2 *ede_b6_f2 = (TH2 *)file->Get("ede_b6_f2");
	TH2 *ede_b6_f3 = (TH2 *)file->Get("ede_b6_f3");
	TH2 *ede_b6_f4 = (TH2 *)file->Get("ede_b6_f4");
	TH2 *ede_b6_f5 = (TH2 *)file->Get("ede_b6_f5");
	TH2 *ede_b6_f6 = (TH2 *)file->Get("ede_b6_f6");
	TH2 *ede_b6_f7 = (TH2 *)file->Get("ede_b6_f7");

	TH2 *ede_b7_f0 = (TH2 *)file->Get("ede_b7_f0");
	TH2 *ede_b7_f1 = (TH2 *)file->Get("ede_b7_f1");
	TH2 *ede_b7_f2 = (TH2 *)file->Get("ede_b7_f2");
	TH2 *ede_b7_f3 = (TH2 *)file->Get("ede_b7_f3");
	TH2 *ede_b7_f4 = (TH2 *)file->Get("ede_b7_f4");
	TH2 *ede_b7_f5 = (TH2 *)file->Get("ede_b7_f5");
	TH2 *ede_b7_f6 = (TH2 *)file->Get("ede_b7_f6");
	TH2 *ede_b7_f7 = (TH2 *)file->Get("ede_b7_f7");

	// ede_b0_f0->Draw("same");
	// ede_b1_f0->Draw("same");
	// ede_b2_f0->Draw("same");
	// ede_b3_f0->Draw("same");
	// ede_b4_f0->Draw("same");
	// ede_b5_f0->Draw("same");
	// ede_b6_f0->Draw("same");
	// ede_b7_f0->Draw("same");

	// ede_b0_f0->GetXaxis()->SetRangeUser(14000,16000);
	// ede_b0_f0->Add(ede_b1_f0);
	// ede_b0_f0->Add(ede_b2_f0);
	// ede_b0_f0->Add(ede_b3_f0);
	// ede_b0_f0->Add(ede_b4_f0);
	// ede_b0_f0->Add(ede_b5_f0);
	// ede_b0_f0->Add(ede_b6_f0);
	// ede_b0_f0->Add(ede_b7_f0);
	// TH1 *hx_ede_b0_f0 = ede_b0_f0->ProjectionX();
	// hx_ede_b0_f0->Draw();

	// ede_b0_f1->Add(ede_b1_f1);
	// ede_b0_f1->Add(ede_b2_f1);
	// ede_b0_f1->Add(ede_b3_f1);
	// ede_b0_f1->Add(ede_b4_f1);
	// ede_b0_f1->Add(ede_b5_f1);
	// ede_b0_f1->Add(ede_b6_f1);
	// ede_b0_f1->Add(ede_b7_f1);
	// ede_b0_f1->Draw();


	TH1 *hx_ede_b0_f0 = ede_b0_f0->ProjectionX();
	TH1 *hx_ede_b0_f1 = ede_b0_f1->ProjectionX();
	TH1 *hx_ede_b0_f2 = ede_b0_f2->ProjectionX();
	TH1 *hx_ede_b0_f3 = ede_b0_f3->ProjectionX();
	TH1 *hx_ede_b0_f4 = ede_b0_f4->ProjectionX();
	TH1 *hx_ede_b0_f5 = ede_b0_f5->ProjectionX();
	TH1 *hx_ede_b0_f6 = ede_b0_f6->ProjectionX();
	TH1 *hx_ede_b0_f7 = ede_b0_f7->ProjectionX();

	TH1 *hx_ede_b1_f0 = ede_b1_f0->ProjectionX();
	TH1 *hx_ede_b1_f1 = ede_b1_f1->ProjectionX();
	TH1 *hx_ede_b1_f2 = ede_b1_f2->ProjectionX();
	TH1 *hx_ede_b1_f3 = ede_b1_f3->ProjectionX();
	TH1 *hx_ede_b1_f4 = ede_b1_f4->ProjectionX();
	TH1 *hx_ede_b1_f5 = ede_b1_f5->ProjectionX();
	TH1 *hx_ede_b1_f6 = ede_b1_f6->ProjectionX();
	TH1 *hx_ede_b1_f7 = ede_b1_f7->ProjectionX();

	TH1 *hx_ede_b2_f0 = ede_b2_f0->ProjectionX();
	TH1 *hx_ede_b2_f1 = ede_b2_f1->ProjectionX();
	TH1 *hx_ede_b2_f2 = ede_b2_f2->ProjectionX();
	TH1 *hx_ede_b2_f3 = ede_b2_f3->ProjectionX();
	TH1 *hx_ede_b2_f4 = ede_b2_f4->ProjectionX();
	TH1 *hx_ede_b2_f5 = ede_b2_f5->ProjectionX();
	TH1 *hx_ede_b2_f6 = ede_b2_f6->ProjectionX();
	TH1 *hx_ede_b2_f7 = ede_b2_f7->ProjectionX();

	TH1 *hx_ede_b3_f0 = ede_b3_f0->ProjectionX();
	TH1 *hx_ede_b3_f1 = ede_b3_f1->ProjectionX();
	TH1 *hx_ede_b3_f2 = ede_b3_f2->ProjectionX();
	TH1 *hx_ede_b3_f3 = ede_b3_f3->ProjectionX();
	TH1 *hx_ede_b3_f4 = ede_b3_f4->ProjectionX();
	TH1 *hx_ede_b3_f5 = ede_b3_f5->ProjectionX();
	TH1 *hx_ede_b3_f6 = ede_b3_f6->ProjectionX();
	TH1 *hx_ede_b3_f7 = ede_b3_f7->ProjectionX();

	TH1 *hx_ede_b4_f0 = ede_b4_f0->ProjectionX();
	TH1 *hx_ede_b4_f1 = ede_b4_f1->ProjectionX();
	TH1 *hx_ede_b4_f2 = ede_b4_f2->ProjectionX();
	TH1 *hx_ede_b4_f3 = ede_b4_f3->ProjectionX();
	TH1 *hx_ede_b4_f4 = ede_b4_f4->ProjectionX();
	TH1 *hx_ede_b4_f5 = ede_b4_f5->ProjectionX();
	TH1 *hx_ede_b4_f6 = ede_b4_f6->ProjectionX();
	TH1 *hx_ede_b4_f7 = ede_b4_f7->ProjectionX();

	TH1 *hx_ede_b5_f0 = ede_b5_f0->ProjectionX();
	TH1 *hx_ede_b5_f1 = ede_b5_f1->ProjectionX();
	TH1 *hx_ede_b5_f2 = ede_b5_f2->ProjectionX();
	TH1 *hx_ede_b5_f3 = ede_b5_f3->ProjectionX();
	TH1 *hx_ede_b5_f4 = ede_b5_f4->ProjectionX();
	TH1 *hx_ede_b5_f5 = ede_b5_f5->ProjectionX();
	TH1 *hx_ede_b5_f6 = ede_b5_f6->ProjectionX();
	TH1 *hx_ede_b5_f7 = ede_b5_f7->ProjectionX();

	TH1 *hx_ede_b6_f0 = ede_b6_f0->ProjectionX();
	TH1 *hx_ede_b6_f1 = ede_b6_f1->ProjectionX();
	TH1 *hx_ede_b6_f2 = ede_b6_f2->ProjectionX();
	TH1 *hx_ede_b6_f3 = ede_b6_f3->ProjectionX();
	TH1 *hx_ede_b6_f4 = ede_b6_f4->ProjectionX();
	TH1 *hx_ede_b6_f5 = ede_b6_f5->ProjectionX();
	TH1 *hx_ede_b6_f6 = ede_b6_f6->ProjectionX();
	TH1 *hx_ede_b6_f7 = ede_b6_f7->ProjectionX();

	TH1 *hx_ede_b7_f0 = ede_b7_f0->ProjectionX();
	TH1 *hx_ede_b7_f1 = ede_b7_f1->ProjectionX();
	TH1 *hx_ede_b7_f2 = ede_b7_f2->ProjectionX();
	TH1 *hx_ede_b7_f3 = ede_b7_f3->ProjectionX();
	TH1 *hx_ede_b7_f4 = ede_b7_f4->ProjectionX();
	TH1 *hx_ede_b7_f5 = ede_b7_f5->ProjectionX();
	TH1 *hx_ede_b7_f6 = ede_b7_f6->ProjectionX();
	TH1 *hx_ede_b7_f7 = ede_b7_f7->ProjectionX();

	hx_ede_b0_f0->SetLineColor(1);
	hx_ede_b1_f0->SetLineColor(2);
	hx_ede_b2_f0->SetLineColor(3);
	hx_ede_b3_f0->SetLineColor(4);
	hx_ede_b4_f0->SetLineColor(5);
	hx_ede_b5_f0->SetLineColor(6);
	hx_ede_b6_f0->SetLineColor(7);
	hx_ede_b7_f0->SetLineColor(8);

	hx_ede_b0_f1->SetLineColor(1);
	hx_ede_b1_f1->SetLineColor(2);
	hx_ede_b2_f1->SetLineColor(3);
	hx_ede_b3_f1->SetLineColor(4);
	hx_ede_b4_f1->SetLineColor(5);
	hx_ede_b5_f1->SetLineColor(6);
	hx_ede_b6_f1->SetLineColor(7);
	hx_ede_b7_f1->SetLineColor(8);

	hx_ede_b0_f2->SetLineColor(1);
	hx_ede_b1_f2->SetLineColor(2);
	hx_ede_b2_f2->SetLineColor(3);
	hx_ede_b3_f2->SetLineColor(4);
	hx_ede_b4_f2->SetLineColor(5);
	hx_ede_b5_f2->SetLineColor(6);
	hx_ede_b6_f2->SetLineColor(7);
	hx_ede_b7_f2->SetLineColor(8);

	hx_ede_b0_f3->SetLineColor(1);
	hx_ede_b1_f3->SetLineColor(2);
	hx_ede_b2_f3->SetLineColor(3);
	hx_ede_b3_f3->SetLineColor(4);
	hx_ede_b4_f3->SetLineColor(5);
	hx_ede_b5_f3->SetLineColor(6);
	hx_ede_b6_f3->SetLineColor(7);
	hx_ede_b7_f3->SetLineColor(8);

	hx_ede_b0_f4->SetLineColor(1);
	hx_ede_b1_f4->SetLineColor(2);
	hx_ede_b2_f4->SetLineColor(3);
	hx_ede_b3_f4->SetLineColor(4);
	hx_ede_b4_f4->SetLineColor(5);
	hx_ede_b5_f4->SetLineColor(6);
	hx_ede_b6_f4->SetLineColor(7);
	hx_ede_b7_f4->SetLineColor(8);

	hx_ede_b0_f5->SetLineColor(1);
	hx_ede_b1_f5->SetLineColor(2);
	hx_ede_b2_f5->SetLineColor(3);
	hx_ede_b3_f5->SetLineColor(4);
	hx_ede_b4_f5->SetLineColor(5);
	hx_ede_b5_f5->SetLineColor(6);
	hx_ede_b6_f5->SetLineColor(7);
	hx_ede_b7_f5->SetLineColor(8);

	hx_ede_b0_f6->SetLineColor(1);
	hx_ede_b1_f6->SetLineColor(2);
	hx_ede_b2_f6->SetLineColor(3);
	hx_ede_b3_f6->SetLineColor(4);
	hx_ede_b4_f6->SetLineColor(5);
	hx_ede_b5_f6->SetLineColor(6);
	hx_ede_b6_f6->SetLineColor(7);
	hx_ede_b7_f6->SetLineColor(8);

	hx_ede_b0_f7->SetLineColor(1);
	hx_ede_b1_f7->SetLineColor(2);
	hx_ede_b2_f7->SetLineColor(3);
	hx_ede_b3_f7->SetLineColor(4);
	hx_ede_b4_f7->SetLineColor(5);
	hx_ede_b5_f7->SetLineColor(6);
	hx_ede_b6_f7->SetLineColor(7);
	hx_ede_b7_f7->SetLineColor(8);

	hx_ede_b0_f0->Draw();
	hx_ede_b1_f0->Draw("same");
	hx_ede_b2_f0->Draw("same");
	hx_ede_b3_f0->Draw("same");
	hx_ede_b4_f0->Draw("same");
	hx_ede_b5_f0->Draw("same");
	hx_ede_b6_f0->Draw("same");
	hx_ede_b7_f0->Draw("same");

	//  hx_ede_b0_f1->Draw("same");
	// hx_ede_b1_f1->Draw("same");
	// hx_ede_b2_f1->Draw("same");
	// hx_ede_b3_f1->Draw("same");
	// hx_ede_b4_f1->Draw("same");
	// hx_ede_b5_f1->Draw("same");
	// hx_ede_b6_f1->Draw("same");
	// hx_ede_b7_f1->Draw("same");

	// hx_ede_b0_f2->Draw("same");
	// hx_ede_b1_f2->Draw("same");
	// hx_ede_b2_f2->Draw("same");
	// hx_ede_b3_f2->Draw("same");
	// hx_ede_b4_f2->Draw("same");
	// hx_ede_b5_f2->Draw("same");
	// hx_ede_b6_f2->Draw("same");
	// hx_ede_b7_f2->Draw("same");

	//hx_ede_b0_f3->Draw("same");
	// hx_ede_b1_f3->Draw("same");
	// hx_ede_b2_f3->Draw("same");
	// hx_ede_b3_f3->Draw("same");
	// hx_ede_b4_f3->Draw("same");
	// hx_ede_b5_f3->Draw("same");
	// hx_ede_b6_f3->Draw("same");
	// hx_ede_b7_f3->Draw("same");

	//hx_ede_b0_f4->Draw("same");
	// hx_ede_b1_f4->Draw("same");
	// hx_ede_b2_f4->Draw("same");
	// hx_ede_b3_f4->Draw("same");
	// hx_ede_b4_f4->Draw("same");
	// hx_ede_b5_f4->Draw("same");
	// hx_ede_b6_f4->Draw("same");
	// hx_ede_b7_f4->Draw("same");

	//hx_ede_b0_f5->Draw("same");
	// hx_ede_b1_f5->Draw("same");
	// hx_ede_b2_f5->Draw("same");
	// hx_ede_b3_f5->Draw("same");
	// hx_ede_b4_f5->Draw("same");
	// hx_ede_b5_f5->Draw("same");
	// hx_ede_b6_f5->Draw("same");
	// hx_ede_b7_f5->Draw("same");

	//hx_ede_b0_f6->Draw("same");
	//hx_ede_b1_f6->Draw("same");
	//hx_ede_b2_f6->Draw("same");
	//hx_ede_b3_f6->Draw("same");
	//hx_ede_b4_f6->Draw("same");
	//hx_ede_b5_f6->Draw("same");
	//hx_ede_b6_f6->Draw("same");
	//hx_ede_b7_f6->Draw("same");

	//hx_ede_b0_f7->Draw("same");
	// hx_ede_b1_f7->Draw("same");
	// hx_ede_b2_f7->Draw("same");
	// hx_ede_b3_f7->Draw("same");
	// hx_ede_b4_f7->Draw("same");
	// hx_ede_b5_f7->Draw("same");
	// hx_ede_b6_f7->Draw("same");
	// hx_ede_b7_f7->Draw("same");


	TH1 *hy_ede_b0_f0 = ede_b0_f0->ProjectionY();
	TH1 *hy_ede_b0_f1 = ede_b0_f1->ProjectionY();
	TH1 *hy_ede_b0_f2 = ede_b0_f2->ProjectionY();
	TH1 *hy_ede_b0_f3 = ede_b0_f3->ProjectionY();
	TH1 *hy_ede_b0_f4 = ede_b0_f4->ProjectionY();
	TH1 *hy_ede_b0_f5 = ede_b0_f5->ProjectionY();
	TH1 *hy_ede_b0_f6 = ede_b0_f6->ProjectionY();
	TH1 *hy_ede_b0_f7 = ede_b0_f7->ProjectionY();

	TH1 *hy_ede_b1_f0 = ede_b1_f0->ProjectionY();
	TH1 *hy_ede_b1_f1 = ede_b1_f1->ProjectionY();
	TH1 *hy_ede_b1_f2 = ede_b1_f2->ProjectionY();
	TH1 *hy_ede_b1_f3 = ede_b1_f3->ProjectionY();
	TH1 *hy_ede_b1_f4 = ede_b1_f4->ProjectionY();
	TH1 *hy_ede_b1_f5 = ede_b1_f5->ProjectionY();
	TH1 *hy_ede_b1_f6 = ede_b1_f6->ProjectionY();
	TH1 *hy_ede_b1_f7 = ede_b1_f7->ProjectionY();

	TH1 *hy_ede_b2_f0 = ede_b2_f0->ProjectionY();
	TH1 *hy_ede_b2_f1 = ede_b2_f1->ProjectionY();
	TH1 *hy_ede_b2_f2 = ede_b2_f2->ProjectionY();
	TH1 *hy_ede_b2_f3 = ede_b2_f3->ProjectionY();
	TH1 *hy_ede_b2_f4 = ede_b2_f4->ProjectionY();
	TH1 *hy_ede_b2_f5 = ede_b2_f5->ProjectionY();
	TH1 *hy_ede_b2_f6 = ede_b2_f6->ProjectionY();
	TH1 *hy_ede_b2_f7 = ede_b2_f7->ProjectionY();

	TH1 *hy_ede_b3_f0 = ede_b3_f0->ProjectionY();
	TH1 *hy_ede_b3_f1 = ede_b3_f1->ProjectionY();
	TH1 *hy_ede_b3_f2 = ede_b3_f2->ProjectionY();
	TH1 *hy_ede_b3_f3 = ede_b3_f3->ProjectionY();
	TH1 *hy_ede_b3_f4 = ede_b3_f4->ProjectionY();
	TH1 *hy_ede_b3_f5 = ede_b3_f5->ProjectionY();
	TH1 *hy_ede_b3_f6 = ede_b3_f6->ProjectionY();
	TH1 *hy_ede_b3_f7 = ede_b3_f7->ProjectionY();

	TH1 *hy_ede_b4_f0 = ede_b4_f0->ProjectionY();
	TH1 *hy_ede_b4_f1 = ede_b4_f1->ProjectionY();
	TH1 *hy_ede_b4_f2 = ede_b4_f2->ProjectionY();
	TH1 *hy_ede_b4_f3 = ede_b4_f3->ProjectionY();
	TH1 *hy_ede_b4_f4 = ede_b4_f4->ProjectionY();
	TH1 *hy_ede_b4_f5 = ede_b4_f5->ProjectionY();
	TH1 *hy_ede_b4_f6 = ede_b4_f6->ProjectionY();
	TH1 *hy_ede_b4_f7 = ede_b4_f7->ProjectionY();

	TH1 *hy_ede_b5_f0 = ede_b5_f0->ProjectionY();
	TH1 *hy_ede_b5_f1 = ede_b5_f1->ProjectionY();
	TH1 *hy_ede_b5_f2 = ede_b5_f2->ProjectionY();
	TH1 *hy_ede_b5_f3 = ede_b5_f3->ProjectionY();
	TH1 *hy_ede_b5_f4 = ede_b5_f4->ProjectionY();
	TH1 *hy_ede_b5_f5 = ede_b5_f5->ProjectionY();
	TH1 *hy_ede_b5_f6 = ede_b5_f6->ProjectionY();
	TH1 *hy_ede_b5_f7 = ede_b5_f7->ProjectionY();

	TH1 *hy_ede_b6_f0 = ede_b6_f0->ProjectionY();
	TH1 *hy_ede_b6_f1 = ede_b6_f1->ProjectionY();
	TH1 *hy_ede_b6_f2 = ede_b6_f2->ProjectionY();
	TH1 *hy_ede_b6_f3 = ede_b6_f3->ProjectionY();
	TH1 *hy_ede_b6_f4 = ede_b6_f4->ProjectionY();
	TH1 *hy_ede_b6_f5 = ede_b6_f5->ProjectionY();
	TH1 *hy_ede_b6_f6 = ede_b6_f6->ProjectionY();
	TH1 *hy_ede_b6_f7 = ede_b6_f7->ProjectionY();

	TH1 *hy_ede_b7_f0 = ede_b7_f0->ProjectionY();
	TH1 *hy_ede_b7_f1 = ede_b7_f1->ProjectionY();
	TH1 *hy_ede_b7_f2 = ede_b7_f2->ProjectionY();
	TH1 *hy_ede_b7_f3 = ede_b7_f3->ProjectionY();
	TH1 *hy_ede_b7_f4 = ede_b7_f4->ProjectionY();
	TH1 *hy_ede_b7_f5 = ede_b7_f5->ProjectionY();
	TH1 *hy_ede_b7_f6 = ede_b7_f6->ProjectionY();
	TH1 *hy_ede_b7_f7 = ede_b7_f7->ProjectionY();

	hy_ede_b0_f0->SetLineColor(1);
	hy_ede_b1_f0->SetLineColor(2);
	hy_ede_b2_f0->SetLineColor(3);
	hy_ede_b3_f0->SetLineColor(4);
	hy_ede_b4_f0->SetLineColor(5);
	hy_ede_b5_f0->SetLineColor(6);
	hy_ede_b6_f0->SetLineColor(7);
	hy_ede_b7_f0->SetLineColor(8);

	hy_ede_b0_f1->SetLineColor(1);
	hy_ede_b1_f1->SetLineColor(2);
	hy_ede_b2_f1->SetLineColor(3);
	hy_ede_b3_f1->SetLineColor(4);
	hy_ede_b4_f1->SetLineColor(5);
	hy_ede_b5_f1->SetLineColor(6);
	hy_ede_b6_f1->SetLineColor(7);
	hy_ede_b7_f1->SetLineColor(8);

	hy_ede_b0_f2->SetLineColor(1);
	hy_ede_b1_f2->SetLineColor(2);
	hy_ede_b2_f2->SetLineColor(3);
	hy_ede_b3_f2->SetLineColor(4);
	hy_ede_b4_f2->SetLineColor(5);
	hy_ede_b5_f2->SetLineColor(6);
	hy_ede_b6_f2->SetLineColor(7);
	hy_ede_b7_f2->SetLineColor(8);

	hy_ede_b0_f3->SetLineColor(1);
	hy_ede_b1_f3->SetLineColor(2);
	hy_ede_b2_f3->SetLineColor(3);
	hy_ede_b3_f3->SetLineColor(4);
	hy_ede_b4_f3->SetLineColor(5);
	hy_ede_b5_f3->SetLineColor(6);
	hy_ede_b6_f3->SetLineColor(7);
	hy_ede_b7_f3->SetLineColor(8);

	hy_ede_b0_f4->SetLineColor(1);
	hy_ede_b1_f4->SetLineColor(2);
	hy_ede_b2_f4->SetLineColor(3);
	hy_ede_b3_f4->SetLineColor(4);
	hy_ede_b4_f4->SetLineColor(5);
	hy_ede_b5_f4->SetLineColor(6);
	hy_ede_b6_f4->SetLineColor(7);
	hy_ede_b7_f4->SetLineColor(8);

	hy_ede_b0_f5->SetLineColor(1);
	hy_ede_b1_f5->SetLineColor(2);
	hy_ede_b2_f5->SetLineColor(3);
	hy_ede_b3_f5->SetLineColor(4);
	hy_ede_b4_f5->SetLineColor(5);
	hy_ede_b5_f5->SetLineColor(6);
	hy_ede_b6_f5->SetLineColor(7);
	hy_ede_b7_f5->SetLineColor(8);

	hy_ede_b0_f6->SetLineColor(1);
	hy_ede_b1_f6->SetLineColor(2);
	hy_ede_b2_f6->SetLineColor(3);
	hy_ede_b3_f6->SetLineColor(4);
	hy_ede_b4_f6->SetLineColor(5);
	hy_ede_b5_f6->SetLineColor(6);
	hy_ede_b6_f6->SetLineColor(7);
	hy_ede_b7_f6->SetLineColor(8);

	hy_ede_b0_f7->SetLineColor(1);
	hy_ede_b1_f7->SetLineColor(2);
	hy_ede_b2_f7->SetLineColor(3);
	hy_ede_b3_f7->SetLineColor(4);
	hy_ede_b4_f7->SetLineColor(5);
	hy_ede_b5_f7->SetLineColor(6);
	hy_ede_b6_f7->SetLineColor(7);
	hy_ede_b7_f7->SetLineColor(8);

	// hy_ede_b0_f0->Draw();
	// hy_ede_b1_f0->Draw("same");
	// hy_ede_b2_f0->Draw("same");
	// hy_ede_b3_f0->Draw("same");
	// hy_ede_b4_f0->Draw("same");
	// hy_ede_b5_f0->Draw("same");
	// hy_ede_b6_f0->Draw("same");
	// hy_ede_b7_f0->Draw("same");

	// hy_ede_b0_f1->Draw();
	// hy_ede_b1_f1->Draw("same");
	// hy_ede_b2_f1->Draw("same");
	// hy_ede_b3_f1->Draw("same");
	// hy_ede_b4_f1->Draw("same");
	// hy_ede_b5_f1->Draw("same");
	// hy_ede_b6_f1->Draw("same");
	// hy_ede_b7_f1->Draw("same");

	// hy_ede_b0_f2->Draw();
	// hy_ede_b1_f2->Draw("same");
	// hy_ede_b2_f2->Draw("same");
	// hy_ede_b3_f2->Draw("same");
	// hy_ede_b4_f2->Draw("same");
	// hy_ede_b5_f2->Draw("same");
	// hy_ede_b6_f2->Draw("same");
	// hy_ede_b7_f2->Draw("same");

	// hy_ede_b0_f3->Draw();
	// hy_ede_b1_f3->Draw("same");
	// hy_ede_b2_f3->Draw("same");
	// hy_ede_b3_f3->Draw("same");
	// hy_ede_b4_f3->Draw("same");
	// hy_ede_b5_f3->Draw("same");
	// hy_ede_b6_f3->Draw("same");
	// hy_ede_b7_f3->Draw("same");

	// hy_ede_b0_f4->Draw();
	// hy_ede_b1_f4->Draw("same");
	// hy_ede_b2_f4->Draw("same");
	// hy_ede_b3_f4->Draw("same");
	// hy_ede_b4_f4->Draw("same");
	// hy_ede_b5_f4->Draw("same");
	// hy_ede_b6_f4->Draw("same");
	// hy_ede_b7_f4->Draw("same");

	// hy_ede_b0_f5->Draw();
	// hy_ede_b1_f5->Draw("same");
	// hy_ede_b2_f5->Draw("same");
	// hy_ede_b3_f5->Draw("same");
	// hy_ede_b4_f5->Draw("same");
	// hy_ede_b5_f5->Draw("same");
	// hy_ede_b6_f5->Draw("same");
	// hy_ede_b7_f5->Draw("same");

	// hy_ede_b0_f6->Draw();
	// hy_ede_b1_f6->Draw("same");
	// hy_ede_b2_f6->Draw("same");
	// hy_ede_b3_f6->Draw("same");
	// hy_ede_b4_f6->Draw("same");
	// hy_ede_b5_f6->Draw("same");
	// hy_ede_b6_f6->Draw("same");
	// hy_ede_b7_f6->Draw("same");

	// hy_ede_b0_f7->Draw();
	// hy_ede_b1_f7->Draw("same");
	// hy_ede_b2_f7->Draw("same");
	// hy_ede_b3_f7->Draw("same");
	// hy_ede_b4_f7->Draw("same");
	// hy_ede_b5_f7->Draw("same");
	// hy_ede_b6_f7->Draw("same");
	// hy_ede_b7_f7->Draw("same");

}