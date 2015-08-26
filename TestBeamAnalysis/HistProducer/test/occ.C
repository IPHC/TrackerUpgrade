void occ()
{
   gROOT->SetBatch();
   gROOT->SetStyle("Plain");

   gROOT->ProcessLine(".x common.C");
   gROOT->ProcessLine(".L addbin.C");

   TCanvas *c1 = new TCanvas("c1","c1",0,0,600,500);

   TFile *f;

   const int nRun = 5;
   std::string run[nRun] = {"762","768","773","831","844"};
   
   for(int i=0;i<nRun;i++)
     {	
	std::string runName = run[i];
	std::string fn = "hist/"+runName+".root";
	f = TFile::Open(fn.c_str());
	
	TH1D *h_occ = (TH1D*)f->Get("h_occ");
	float ntot = h_occ->GetBinContent(1);
	h_occ->Scale(1./ntot);
	h_occ->GetXaxis()->SetRangeUser(1.,8.);
	h_occ->Draw();
	h_occ->Draw("e1 hist");
	h_occ->GetXaxis()->SetTitle("");
	h_occ->GetYaxis()->SetTitle("Efficiency");
	h_occ->GetXaxis()->SetBinLabel(2,"S0");
	h_occ->GetXaxis()->SetBinLabel(3,"S1");
	h_occ->GetXaxis()->SetBinLabel(4,"S1&&S2");
	h_occ->GetXaxis()->SetBinLabel(5,"STUB");
	h_occ->GetXaxis()->SetBinLabel(6,"!S0");
	h_occ->GetXaxis()->SetBinLabel(7,"!S1");
	h_occ->GetXaxis()->SetBinLabel(8,"!S1&&!S2");
	h_occ->SetMarkerSize(0);
	std::string pname = "pics/h_occ_"+runName+".eps";
	c1->Print(pname.c_str());
	c1->Clear();
     }   
   
   gApplication->Terminate();
}
