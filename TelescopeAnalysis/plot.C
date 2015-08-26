void plot()
{
   gROOT->SetBatch();
   gROOT->SetStyle("Plain");

   gROOT->ProcessLine(".x common.C");
   gROOT->ProcessLine(".L addbin.C");

   TCanvas *c1 = new TCanvas("c1","c1",0,0,600,500);

   TFile *f;

   std::string fn = "output.root";
   f = TFile::Open(fn.c_str());

   TH1D *h_posDiffX = (TH1D*)f->Get("h_posDiffX");
   h_posDiffX->Draw("e1 hist");
   h_posDiffX->GetXaxis()->SetTitle("s^{DUT}-x^{TEL} [mm]");
   h_posDiffX->GetYaxis()->SetTitle("Number of events");
   h_posDiffX->SetMarkerSize(0);
   c1->Print("pics/h_posDiffX.eps");
   c1->Clear();

   TH1D *h_posDiffX_zoom = (TH1D*)f->Get("h_posDiffX_zoom");
   h_posDiffX_zoom->Draw("e1 hist");
   h_posDiffX_zoom->GetXaxis()->SetTitle("s^{DUT}-x^{TEL} [mm]");
   h_posDiffX_zoom->GetYaxis()->SetTitle("Number of events");
   h_posDiffX_zoom->SetMarkerSize(0);
   c1->Print("pics/h_posDiffX_zoom.eps");
   c1->Clear();
   
   TH1D *h_posDiffY = (TH1D*)f->Get("h_posDiffY");
   h_posDiffY->Draw("e1 hist");
   h_posDiffY->GetXaxis()->SetTitle("s^{DUT}-y^{TEL} [mm]");
   h_posDiffY->GetYaxis()->SetTitle("Number of events");
   h_posDiffY->SetMarkerSize(0);
   c1->Print("pics/h_posDiffY.eps");
   c1->Clear();

   TH1D *h_effStubAll = (TH1D*)f->Get("h_effStubAll");
   TH1D *h_effStubPass = (TH1D*)f->Get("h_effStubPass");
   TH1D *h_effStub = (TH1D*)h_effStubAll->Clone("h_effStub");
   for(int ib=1;ib<h_effStub->GetXaxis()->GetNbins()+1;ib++)
     {
	double v0 = h_effStubAll->GetBinContent(ib);
	double v1 = h_effStubPass->GetBinContent(ib);
	double eff = (v0 > 0.) ? v1/v0 : 0.;
	h_effStub->SetBinContent(ib,eff);
	h_effStub->SetBinError(ib,0.);
     }      
   h_effStub->Draw("e1 hist");
   h_effStub->GetXaxis()->SetTitle("#strips");
   h_effStub->GetYaxis()->SetTitle("Efficiency");
   h_effStub->SetMarkerSize(0);
   c1->Print("pics/h_effStub.eps");
   c1->Clear();

   TH1D *h_dutStubPos = (TH1D*)f->Get("h_dutStubPos");
   h_dutStubPos->Draw("e1 hist");
   h_dutStubPos->GetXaxis()->SetTitle("s^{DUT} [#strips]");
   h_dutStubPos->GetYaxis()->SetTitle("Number of events");
   h_dutStubPos->SetMarkerSize(0);
   c1->Print("pics/h_dutStubPos.eps");
   c1->Clear();

   TH1D *h_telStubPos = (TH1D*)f->Get("h_telStubPos");
   h_telStubPos->Draw("e1 hist");
   h_telStubPos->GetXaxis()->SetTitle("s^{TEL} [#strips]");
   h_telStubPos->GetYaxis()->SetTitle("Number of events");
   h_telStubPos->SetMarkerSize(0);
   c1->Print("pics/h_telStubPos.eps");
   c1->Clear();

   TH1D *h_dxStubPos = (TH1D*)f->Get("h_dxStubPos");
   h_dxStubPos->Draw("e1 hist");
   h_dxStubPos->GetXaxis()->SetTitle("s^{TEL}-s^{DUT} [#strips]");
   h_dxStubPos->GetYaxis()->SetTitle("Number of events");
   h_dxStubPos->SetMarkerSize(0);
   c1->Print("pics/h_dxStubPos.eps");
   c1->Clear();

   TH2D *h_telBeamProf = (TH2D*)f->Get("h_telBeamProf");
   h_telBeamProf->Draw("");
   h_telBeamProf->GetXaxis()->SetTitle("X^{TEL} [mm]");
   h_telBeamProf->GetYaxis()->SetTitle("Y^{TEL} [mm]");
   h_telBeamProf->SetMarkerSize(0.5);
   c1->Print("pics/h_telBeamProf.eps");
   c1->Clear();

   TH1D *h_stripProf = (TH1D*)f->Get("h_stripProf");
   h_stripProf->Draw("e1 hist");
   h_stripProf->GetXaxis()->SetTitle("y^{DUT} [mm]");
   h_stripProf->GetYaxis()->SetTitle("Number of events");
   h_stripProf->SetMarkerSize(0);
   c1->Print("pics/h_stripProf.eps");
   c1->Clear();
   
   gApplication->Terminate();
}
