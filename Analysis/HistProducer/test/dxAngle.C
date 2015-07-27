void dxAngle()
{
   gROOT->SetBatch();
   gROOT->SetStyle("Plain");

   gROOT->ProcessLine(".x common.C");
   gROOT->ProcessLine(".L addbin.C");
   
   TCanvas *c1 = new TCanvas("c1","c1",0,0,600,500);

   const int nf = 11;
   TFile *f[nf];
   float angle[nf] = {1,2,3,5,9,10,11,12,13,15,20};
   float angleErr[nf] = {0,0,0,0,0,0,0,0,0,0,0};
   std::string fname[nf];
   for(int i=0;i<nf;i++)
     {
	fname[i] = "hist/deg"+std::string(Form("%d",int(angle[i])))+".root";
     }   
   
   float dx[nf];
   float dxErr[nf];
   
   for(int i=0;i<nf;i++)
     {	
	f[i] = TFile::Open(fname[i].c_str());
	
	TH1D *h_dx = (TH1D*)f[i]->Get("h_s_dx");

	double mean = h_dx->GetMean();
	double meanErr = h_dx->GetMeanError();
	dx[i] = mean;
	dxErr[i] = meanErr;

	f[i]->Close();
     }   
   
   TGraphErrors *Dx_vs_Ang;

   Dx_vs_Ang = new TGraphErrors(nf,angle,dx,angleErr,dxErr);
   Dx_vs_Ang->SetMarkerSize(0.7);
   Dx_vs_Ang->SetLineWidth(2);	
   Dx_vs_Ang->SetMarkerColor(kBlack);
   Dx_vs_Ang->SetLineColor(kBlack);

   TLegend *leg = new TLegend(0.65,0.80,0.85,0.50);
   leg->SetFillColor(253);
   leg->SetBorderSize(0);

//   Dx_vs_Ang->GetYaxis()->SetRangeUser(0.5,1.5);
   Dx_vs_Ang->GetXaxis()->SetTitle("Rotation angle [deg]");
   Dx_vs_Ang->GetYaxis()->SetTitle("x^{0}-x^{1} (strips)");
   
   Dx_vs_Ang->Draw("APL");
   
   c1->Print("pics/Dx_vs_Ang.eps");
   c1->Clear();
   
   gApplication->Terminate();
}
