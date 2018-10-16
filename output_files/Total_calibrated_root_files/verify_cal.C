/***************** ROOT Macro to verify the quality of the particle calibration ***************
***********************************************************************************************
********************************** Author: Alexander Springer *********************************
***********************************************************************************************

How to use this macro:

Call macro with .x verify_cal.C ,
then choose witch ring you want to look at and call a function accordingly.
It would be draw_ede_f0() for the first ring and so on...

If the calibration is good the peaks should be nicely aligned up to the Neutron separation energy.

draw_ede() shows all the 64 spectra on top of each other.

draw_ede_f() draws the ede_fX spectra, which are the summed detectors of the same angle.

We can also have a look at the excitation energy. This plots the different ex_fX root matrices into one plot:
draw_ex()

I tried to use colours that are easy to difference between, not following any code.
They are the same in every plot, so after a while you will know which is which spectrum.

***********************************************************************************************
************************************** Have a nice day. **************************************/

// Main function without any special purpose, one could add some if he/she wants.
void verify_cal(){
    ;
}

void draw_ede_f0(){
    ede_b0_f0->SetLineColor(1);
    ede_b1_f0->SetLineColor(85);
    ede_b2_f0->SetLineColor(28);
    ede_b3_f0->SetLineColor(2);
    ede_b4_f0->SetLineColor(46);
    ede_b5_f0->SetLineColor(14);
    ede_b6_f0->SetLineColor(51);
    ede_b7_f0->SetLineColor(62);
    ede_b0_f0->Draw();
    ede_b1_f0->Draw("SAME");
    ede_b2_f0->Draw("SAME");
    ede_b3_f0->Draw("SAME");
    ede_b4_f0->Draw("SAME");
    ede_b5_f0->Draw("SAME");
    ede_b6_f0->Draw("SAME");
    ede_b7_f0->Draw("SAME");
    c1->BuildLegend(0.98,0.98,0.74,0.74);
    c1->SetTitle("Strip 0");
}
void draw_ede_f1(){
    ede_b0_f1->SetLineColor(1);
    ede_b1_f1->SetLineColor(85);
    ede_b2_f1->SetLineColor(28);
    ede_b3_f1->SetLineColor(2);
    ede_b4_f1->SetLineColor(46);
    ede_b5_f1->SetLineColor(14);
    ede_b6_f1->SetLineColor(51);
    ede_b7_f1->SetLineColor(62);
    ede_b0_f1->Draw();
    ede_b1_f1->Draw("SAME");
    ede_b2_f1->Draw("SAME");
    ede_b3_f1->Draw("SAME");
    ede_b4_f1->Draw("SAME");
    ede_b5_f1->Draw("SAME");
    ede_b6_f1->Draw("SAME");
    ede_b7_f1->Draw("SAME");
    c1->BuildLegend(0.98,0.98,0.74,0.74);
    c1->SetTitle("Strip 1");
}
void draw_ede_f2(){
    ede_b0_f2->SetLineColor(1);
    ede_b1_f2->SetLineColor(85);
    ede_b2_f2->SetLineColor(28);
    ede_b3_f2->SetLineColor(2);
    ede_b4_f2->SetLineColor(46);
    ede_b5_f2->SetLineColor(14);
    ede_b6_f2->SetLineColor(51);
    ede_b7_f2->SetLineColor(62);
    ede_b0_f2->Draw();
    ede_b1_f2->Draw("SAME");
    ede_b2_f2->Draw("SAME");
    ede_b3_f2->Draw("SAME");
    ede_b4_f2->Draw("SAME");
    ede_b5_f2->Draw("SAME");
    ede_b6_f2->Draw("SAME");
    ede_b7_f2->Draw("SAME");
    c1->BuildLegend(0.98,0.98,0.74,0.74);
    c1->SetTitle("Strip 2");
}
void draw_ede_f3(){
    ede_b0_f3->SetLineColor(1);
    ede_b1_f3->SetLineColor(85);
    ede_b2_f3->SetLineColor(28);
    ede_b3_f3->SetLineColor(2);
    ede_b4_f3->SetLineColor(46);
    ede_b5_f3->SetLineColor(14);
    ede_b6_f3->SetLineColor(51);
    ede_b7_f3->SetLineColor(62);
    ede_b0_f3->Draw();
    ede_b1_f3->Draw("SAME");
    ede_b2_f3->Draw("SAME");
    ede_b3_f3->Draw("SAME");
    ede_b4_f3->Draw("SAME");
    ede_b5_f3->Draw("SAME");
    ede_b6_f3->Draw("SAME");
    ede_b7_f3->Draw("SAME");
    c1->BuildLegend(0.98,0.98,0.74,0.74);
    c1->SetTitle("Strip 3");
}
void draw_ede_f4(){
    ede_b0_f4->SetLineColor(1);
    ede_b1_f4->SetLineColor(85);
    ede_b2_f4->SetLineColor(28);
    ede_b3_f4->SetLineColor(2);
    ede_b4_f4->SetLineColor(46);
    ede_b5_f4->SetLineColor(14);
    ede_b6_f4->SetLineColor(51);
    ede_b7_f4->SetLineColor(62);
    ede_b0_f4->Draw();
    ede_b1_f4->Draw("SAME");
    ede_b2_f4->Draw("SAME");
    ede_b3_f4->Draw("SAME");
    ede_b4_f4->Draw("SAME");
    ede_b5_f4->Draw("SAME");
    ede_b6_f4->Draw("SAME");
    ede_b7_f4->Draw("SAME");
    c1->BuildLegend(0.98,0.98,0.74,0.74);
    c1->SetTitle("Strip 4");
}
void draw_ede_f5(){
    ede_b0_f5->SetLineColor(1);
    ede_b1_f5->SetLineColor(85);
    ede_b2_f5->SetLineColor(28);
    ede_b3_f5->SetLineColor(2);
    ede_b4_f5->SetLineColor(46);
    ede_b5_f5->SetLineColor(14);
    ede_b6_f5->SetLineColor(51);
    ede_b7_f5->SetLineColor(62);
    ede_b0_f5->Draw();
    ede_b1_f5->Draw("SAME");
    ede_b2_f5->Draw("SAME");
    ede_b3_f5->Draw("SAME");
    ede_b4_f5->Draw("SAME");
    ede_b5_f5->Draw("SAME");
    ede_b6_f5->Draw("SAME");
    ede_b7_f5->Draw("SAME");
    c1->BuildLegend(0.98,0.98,0.74,0.74);
    c1->SetTitle("Strip 5");
}
void draw_ede_f6(){
    ede_b0_f6->SetLineColor(1);
    ede_b1_f6->SetLineColor(85);
    ede_b2_f6->SetLineColor(28);
    ede_b3_f6->SetLineColor(2);
    ede_b4_f6->SetLineColor(46);
    ede_b5_f6->SetLineColor(14);
    ede_b6_f6->SetLineColor(51);
    ede_b7_f6->SetLineColor(62);
    ede_b0_f6->Draw();
    ede_b1_f6->Draw("SAME");
    ede_b2_f6->Draw("SAME");
    ede_b3_f6->Draw("SAME");
    ede_b4_f6->Draw("SAME");
    ede_b5_f6->Draw("SAME");
    ede_b6_f6->Draw("SAME");
    ede_b7_f6->Draw("SAME");
    c1->BuildLegend(0.98,0.98,0.74,0.74);
    c1->SetTitle("Strip 6");
}
void draw_ede_f7(){
    ede_b0_f7->SetLineColor(1);
    ede_b1_f7->SetLineColor(85);
    ede_b2_f7->SetLineColor(28);
    ede_b3_f7->SetLineColor(2);
    ede_b4_f7->SetLineColor(46);
    ede_b5_f7->SetLineColor(14);
    ede_b6_f7->SetLineColor(51);
    ede_b7_f7->SetLineColor(62);
    ede_b0_f7->Draw();
    ede_b1_f7->Draw("SAME");
    ede_b2_f7->Draw("SAME");
    ede_b3_f7->Draw("SAME");
    ede_b4_f7->Draw("SAME");
    ede_b5_f7->Draw("SAME");
    ede_b6_f7->Draw("SAME");
    ede_b7_f7->Draw("SAME");
    c1->BuildLegend(0.98,0.98,0.74,0.74);
    c1->SetTitle("Strip 7");
}

// All the spectra in one plot
void draw_ede(){
    ede_b0_f0->SetLineColor(1);
    ede_b1_f0->SetLineColor(85);
    ede_b2_f0->SetLineColor(28);
    ede_b3_f0->SetLineColor(2);
    ede_b4_f0->SetLineColor(46);
    ede_b5_f0->SetLineColor(14);
    ede_b6_f0->SetLineColor(51);
    ede_b7_f0->SetLineColor(62);
    ede_b0_f0->Draw();
    ede_b1_f0->Draw("SAME");
    ede_b2_f0->Draw("SAME");
    ede_b3_f0->Draw("SAME");
    ede_b4_f0->Draw("SAME");
    ede_b5_f0->Draw("SAME");
    ede_b6_f0->Draw("SAME");
    ede_b7_f0->Draw("SAME");
    ede_b0_f1->SetLineColor(1);
    ede_b1_f1->SetLineColor(85);
    ede_b2_f1->SetLineColor(28);
    ede_b3_f1->SetLineColor(2);
    ede_b4_f1->SetLineColor(46);
    ede_b5_f1->SetLineColor(14);
    ede_b6_f1->SetLineColor(51);
    ede_b7_f1->SetLineColor(62);
    ede_b0_f1->Draw("SAME");
    ede_b1_f1->Draw("SAME");
    ede_b2_f1->Draw("SAME");
    ede_b3_f1->Draw("SAME");
    ede_b4_f1->Draw("SAME");
    ede_b5_f1->Draw("SAME");
    ede_b6_f1->Draw("SAME");
    ede_b7_f1->Draw("SAME");
    ede_b0_f2->SetLineColor(1);
    ede_b1_f2->SetLineColor(85);
    ede_b2_f2->SetLineColor(28);
    ede_b3_f2->SetLineColor(2);
    ede_b4_f2->SetLineColor(46);
    ede_b5_f2->SetLineColor(14);
    ede_b6_f2->SetLineColor(51);
    ede_b7_f2->SetLineColor(62);
    ede_b0_f2->Draw("SAME");
    ede_b1_f2->Draw("SAME");
    ede_b2_f2->Draw("SAME");
    ede_b3_f2->Draw("SAME");
    ede_b4_f2->Draw("SAME");
    ede_b5_f2->Draw("SAME");
    ede_b6_f2->Draw("SAME");
    ede_b7_f2->Draw("SAME");
    ede_b0_f3->SetLineColor(1);
    ede_b1_f3->SetLineColor(85);
    ede_b2_f3->SetLineColor(28);
    ede_b3_f3->SetLineColor(2);
    ede_b4_f3->SetLineColor(46);
    ede_b5_f3->SetLineColor(14);
    ede_b6_f3->SetLineColor(51);
    ede_b7_f3->SetLineColor(62);
    ede_b0_f3->Draw("SAME");
    ede_b1_f3->Draw("SAME");
    ede_b2_f3->Draw("SAME");
    ede_b3_f3->Draw("SAME");
    ede_b4_f3->Draw("SAME");
    ede_b5_f3->Draw("SAME");
    ede_b6_f3->Draw("SAME");
    ede_b7_f3->Draw("SAME");
    ede_b0_f4->SetLineColor(1);
    ede_b1_f4->SetLineColor(85);
    ede_b2_f4->SetLineColor(28);
    ede_b3_f4->SetLineColor(2);
    ede_b4_f4->SetLineColor(46);
    ede_b5_f4->SetLineColor(14);
    ede_b6_f4->SetLineColor(51);
    ede_b7_f4->SetLineColor(62);
    ede_b0_f4->Draw("SAME");
    ede_b1_f4->Draw("SAME");
    ede_b2_f4->Draw("SAME");
    ede_b3_f4->Draw("SAME");
    ede_b4_f4->Draw("SAME");
    ede_b5_f4->Draw("SAME");
    ede_b6_f4->Draw("SAME");
    ede_b7_f4->Draw("SAME");
    ede_b0_f5->SetLineColor(1);
    ede_b1_f5->SetLineColor(85);
    ede_b2_f5->SetLineColor(28);
    ede_b3_f5->SetLineColor(2);
    ede_b4_f5->SetLineColor(46);
    ede_b5_f5->SetLineColor(14);
    ede_b6_f5->SetLineColor(51);
    ede_b7_f5->SetLineColor(62);
    ede_b0_f5->Draw("SAME");
    ede_b1_f5->Draw("SAME");
    ede_b2_f5->Draw("SAME");
    ede_b3_f5->Draw("SAME");
    ede_b4_f5->Draw("SAME");
    ede_b5_f5->Draw("SAME");
    ede_b6_f5->Draw("SAME");
    ede_b7_f5->Draw("SAME");
    ede_b0_f6->SetLineColor(1);
    ede_b1_f6->SetLineColor(85);
    ede_b2_f6->SetLineColor(28);
    ede_b3_f6->SetLineColor(2);
    ede_b4_f6->SetLineColor(46);
    ede_b5_f6->SetLineColor(14);
    ede_b6_f6->SetLineColor(51);
    ede_b7_f6->SetLineColor(62);
    ede_b0_f6->Draw("SAME");
    ede_b1_f6->Draw("SAME");
    ede_b2_f6->Draw("SAME");
    ede_b3_f6->Draw("SAME");
    ede_b4_f6->Draw("SAME");
    ede_b5_f6->Draw("SAME");
    ede_b6_f6->Draw("SAME");
    ede_b7_f6->Draw("SAME");
    ede_b0_f7->SetLineColor(1);
    ede_b1_f7->SetLineColor(85);
    ede_b2_f7->SetLineColor(28);
    ede_b3_f7->SetLineColor(2);
    ede_b4_f7->SetLineColor(46);
    ede_b5_f7->SetLineColor(14);
    ede_b6_f7->SetLineColor(51);
    ede_b7_f7->SetLineColor(62);
    ede_b0_f7->Draw("SAME");
    ede_b1_f7->Draw("SAME");
    ede_b2_f7->Draw("SAME");
    ede_b3_f7->Draw("SAME");
    ede_b4_f7->Draw("SAME");
    ede_b5_f7->Draw("SAME");
    ede_b6_f7->Draw("SAME");
    ede_b7_f7->Draw("SAME");
}

// This one is for Drawing all ede_fX's so all summed up detectors
void draw_ede_f(){
    ede_f0->SetLineColor(1);
    ede_f1->SetLineColor(85);
    ede_f2->SetLineColor(28);
    ede_f3->SetLineColor(2);
    ede_f4->SetLineColor(46);
    ede_f5->SetLineColor(14);
    ede_f6->SetLineColor(51);
    ede_f7->SetLineColor(62);
    ede_f0->Draw();
    ede_f1->Draw("SAME");
    ede_f2->Draw("SAME");
    ede_f3->Draw("SAME");
    ede_f4->Draw("SAME");
    ede_f5->Draw("SAME");
    ede_f6->Draw("SAME");
    ede_f7->Draw("SAME");
    c1->BuildLegend(0.98,0.98,0.74,0.74);
    c1->SetTitle("Summed up detectors for different angles");
}

// This is concerning the excitation energy!
void draw_ex(){
    ex_f0->SetLineColor(1);
    ex_f1->SetLineColor(85);
    ex_f2->SetLineColor(28);
    ex_f3->SetLineColor(2);
    ex_f4->SetLineColor(46);
    ex_f5->SetLineColor(14);
    ex_f6->SetLineColor(51);
    ex_f7->SetLineColor(62);
    ex_f0->Draw();
    ex_f1->Draw("SAME");
    ex_f2->Draw("SAME");
    ex_f3->Draw("SAME");
    ex_f4->Draw("SAME");
    ex_f5->Draw("SAME");
    ex_f6->Draw("SAME");
    ex_f7->Draw("SAME");
    c1->BuildLegend(0.98,0.98,0.74,0.74);
    c1->SetTitle("Excitation Energy");
}

/*
For Key in the left top corner, use:
c1->BuildLegend(0.3,0.98,0.02,0.74);
*/