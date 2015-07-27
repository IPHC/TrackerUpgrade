void clusterSize()
{
   gROOT->SetBatch();
   gROOT->SetStyle("Plain");

   gROOT->ProcessLine(".x common.C");
   gROOT->ProcessLine(".L addbin.C");
   
   TCanvas *c1 = new TCanvas("c1","c1",0,0,600,500);

   const int nf = 12;
   TFile *f[nf];
   float angle[nf] = {0,1,2,3,5,9,10,11,12,13,15,20};
   float angleErr[nf] = {0,0,0,0,0,0,0,0,0,0,0,0};
   std::string fname[nf];
   for(int i=0;i<nf;i++)
     {
	fname[i] = "hist/deg"+std::string(Form("%d",int(angle[i])))+".root";
     }   
   
   float clusterSize[nf];
   float clusterSizeErr[nf];
   
   for(int i=0;i<nf;i++)
     {	
	f[i] = TFile::Open(fname[i].c_str());
	
	TH1D *h_clusterSize = (TH1D*)f[i]->Get("h_s0_clusterSize");

	double mean = h_clusterSize->GetMean();
	double meanErr = h_clusterSize->GetMeanError();
	clusterSize[i] = mean;
	clusterSizeErr[i] = meanErr;

	f[i]->Close();
     }   
   
   TGraphErrors *ClusterSize_vs_Ang;

   ClusterSize_vs_Ang = new TGraphErrors(nf,angle,clusterSize,angleErr,clusterSizeErr);
   ClusterSize_vs_Ang->SetMarkerSize(0.7);
   ClusterSize_vs_Ang->SetLineWidth(2);	
   ClusterSize_vs_Ang->SetMarkerColor(kBlack);
   ClusterSize_vs_Ang->SetLineColor(kBlack);

   TLegend *leg = new TLegend(0.65,0.80,0.85,0.50);
   leg->SetFillColor(253);
   leg->SetBorderSize(0);

   ClusterSize_vs_Ang->GetYaxis()->SetRangeUser(0.5,1.5);
   ClusterSize_vs_Ang->GetXaxis()->SetTitle("Rotation angle [deg]");
   ClusterSize_vs_Ang->GetYaxis()->SetTitle("Cluster size (strips)");
   
   ClusterSize_vs_Ang->Draw("APL");
   
   c1->Print("pics/ClusterSize_vs_Ang.eps");
   c1->Clear();
   
   gApplication->Terminate();
}
