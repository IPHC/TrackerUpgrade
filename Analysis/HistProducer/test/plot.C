void plot()
{
   gROOT->SetBatch();
   gROOT->SetStyle("Plain");

   gROOT->ProcessLine(".x common.C");
   gROOT->ProcessLine(".L addbin.C");

   TCanvas *c1 = new TCanvas("c1","c1",0,0,600,500);

   TFile *f;

   std::string fn = "hist/deg20.root";
   f = TFile::Open(fn.c_str());

   TH1D *h_s0_data = (TH1D*)f->Get("h_s0_data");
   h_s0_data->Draw("e1 hist");
   h_s0_data->GetXaxis()->SetTitle("Hits in plane #0");
   h_s0_data->GetYaxis()->SetTitle("");
   h_s0_data->SetMarkerSize(0);
   c1->Print("pics/h_s0_data.eps");
   c1->Clear();

   TH1D *h_s1_data = (TH1D*)f->Get("h_s1_data");
   h_s1_data->Draw("e1 hist");
   h_s1_data->GetXaxis()->SetTitle("Hits in plane #1");
   h_s1_data->GetYaxis()->SetTitle("");
   h_s1_data->SetMarkerSize(0);
   c1->Print("pics/h_s1_data.eps");
   c1->Clear();

   TH1D *h_stubs = (TH1D*)f->Get("h_stubs_w7");
   h_stubs->Draw("e1 hist");
   h_stubs->GetXaxis()->SetTitle("Number of stubs");
   h_stubs->GetYaxis()->SetTitle("");
   h_stubs->SetMarkerSize(0);
   c1->Print("pics/h_stubs.eps");
   c1->Clear();

   TH1D *h_s_dx = (TH1D*)f->Get("h_s_dx");
   h_s_dx->Draw("e1 hist");
   h_s_dx->GetXaxis()->SetTitle("x^{0}-x^{1}");
   h_s_dx->GetYaxis()->SetTitle("");
   h_s_dx->SetMarkerSize(0);
   c1->Print("pics/h_s_dx.eps");
   c1->Clear();

   TH1D *h_s0_clusterSize = (TH1D*)f->Get("h_s0_clusterSize");
   h_s0_clusterSize->Draw("e1 hist");
   h_s0_clusterSize->GetXaxis()->SetTitle("Average cluster size in plane #0");
   h_s0_clusterSize->GetYaxis()->SetTitle("");
   h_s0_clusterSize->SetMarkerSize(0);
   c1->Print("pics/h_s0_clusterSize.eps");
   c1->Clear();

   TH1D *h_s1_clusterSize = (TH1D*)f->Get("h_s1_clusterSize");
   h_s1_clusterSize->Draw("e1 hist");
   h_s1_clusterSize->GetXaxis()->SetTitle("Average cluster size in plane #1");
   h_s1_clusterSize->GetYaxis()->SetTitle("");
   h_s1_clusterSize->SetMarkerSize(0);
   c1->Print("pics/h_s1_clusterSize.eps");
   c1->Clear();
   
   gApplication->Terminate();
}
