double errf(double v1,double ve1,double v2,double ve2);

void eff()
{
   gROOT->SetBatch();
   gROOT->SetStyle("Plain");

   gROOT->ProcessLine(".x common.C");
   gROOT->ProcessLine(".L addbin.C");

   double angMisA = 0;
   
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
   
   const int nwStub = 3;
   int wStub[nwStub] = {3,5,7};

   float eff[nwStub][nf];
   float effErr[nwStub][nf];
   float pt[nf];
   float eff_SW[nf];
   float effErr_SW[nf];
   
   std::string stubv = "_plane1";
   
   for(int iw=0;iw<nwStub;iw++)
     {	
	std::string stubName = "h_stubs"+stubv+"_w"+std::string(Form("%d",wStub[iw]));
	
	for(int i=0;i<nf;i++)
	  {	
	     f[i] = TFile::Open(fname[i].c_str());

	     if( iw == 0 )
	       {
		  std::string stubWordName = "h_stubWord"+stubv;
		  TH1D *h_stubWord = (TH1D*)f[i]->Get(stubWordName.c_str());

		  double err_SW;
		  double err0_SW;
		  double allErr_SW;
		  double all_SW = h_stubWord->IntegralAndError(0,h_stubWord->GetXaxis()->GetNbins()+1,allErr_SW);
		  double noStub_SW = h_stubWord->IntegralAndError(0,1,err0_SW);
		  double yesStub_SW = h_stubWord->IntegralAndError(2,h_stubWord->GetXaxis()->GetNbins()+1,err_SW);
		  eff_SW[i] = (all_SW > 0.) ? yesStub_SW/all_SW : 0.;
		  effErr_SW[i] = errf(yesStub_SW,err_SW,all_SW,allErr_SW);
	       }	     
	     
	     TH1D *h_stubs = (TH1D*)f[i]->Get(stubName.c_str());
	     
	     double err;
	     double err0;
	     double allErr;
	     double all = h_stubs->IntegralAndError(0,h_stubs->GetXaxis()->GetNbins()+1,allErr);
	     double noStub = h_stubs->IntegralAndError(0,1,err0);
	     double yesStub = h_stubs->IntegralAndError(2,h_stubs->GetXaxis()->GetNbins()+1,err);
	     eff[iw][i] = (all > 0.) ? yesStub/all : 0.;
	     effErr[iw][i] = errf(yesStub,err,all,allErr);

	     if( iw == 0 )
	       {		  
		  if( angle[i] != 0. )
		    pt[i] = 1.1*0.15*4/TMath::Sin((angle[i]+angMisA)*3.14/180.);
		  else
		    pt[i] = 0.;
	       }	     
	     
	     f[i]->Close();
	  }		
     }   
   
   TGraphErrors *Eff_vs_Ang[nwStub];
   TGraphErrors *Eff_vs_Pt[nwStub];
 
   TGraphErrors *Eff_vs_Ang_SW;
   TGraphErrors *Eff_vs_Pt_SW;

   Eff_vs_Ang_SW = new TGraphErrors(nf,angle,eff_SW,angleErr,effErr_SW);
   Eff_vs_Ang_SW->SetMarkerSize(0.7);
   Eff_vs_Ang_SW->SetLineWidth(2);
   Eff_vs_Ang_SW->SetLineStyle(2);

   Eff_vs_Pt_SW = new TGraphErrors(nf,pt,eff_SW,angleErr,effErr_SW);
   Eff_vs_Pt_SW->SetMarkerSize(0.7);
   Eff_vs_Pt_SW->SetLineWidth(2);
   Eff_vs_Pt_SW->SetLineStyle(2);
   
   for(int iw=0;iw<nwStub;iw++)
     {	
	Eff_vs_Ang[iw] = new TGraphErrors(nf,angle,eff[iw],angleErr,effErr[iw]);
	Eff_vs_Ang[iw]->SetMarkerSize(0.7);

	Eff_vs_Pt[iw] = new TGraphErrors(nf,pt,eff[iw],angleErr,effErr[iw]);
	Eff_vs_Pt[iw]->SetMarkerSize(0.7);
	
	Eff_vs_Ang[iw]->SetLineWidth(2);
	Eff_vs_Pt[iw]->SetLineWidth(2);
	
	if( iw == 0 )
	  {	     
	     Eff_vs_Ang[iw]->SetMarkerColor(kBlack);
	     Eff_vs_Ang[iw]->SetLineColor(kBlack);

	     Eff_vs_Pt[iw]->SetMarkerColor(kBlack);
	     Eff_vs_Pt[iw]->SetLineColor(kBlack);
	  }
	else if( iw == 1 )
	  {
	     Eff_vs_Ang[iw]->SetMarkerColor(kRed);
	     Eff_vs_Ang[iw]->SetLineColor(kRed);

	     Eff_vs_Pt[iw]->SetMarkerColor(kRed);
	     Eff_vs_Pt[iw]->SetLineColor(kRed);
	  }	
	else if( iw == 2 )
	  {
	     Eff_vs_Ang[iw]->SetMarkerColor(kBlue);
	     Eff_vs_Ang[iw]->SetLineColor(kBlue);

	     Eff_vs_Pt[iw]->SetMarkerColor(kBlue);
	     Eff_vs_Pt[iw]->SetLineColor(kBlue);
	  }	
     }

   TLegend *leg = new TLegend(0.65,0.80,0.85,0.50);
   leg->SetFillColor(253);
   leg->SetBorderSize(0);
   
   std::string lab = "Window #pm ";
   std::string lab0 = lab+std::string(Form("%d",wStub[0]));
   leg->AddEntry(Eff_vs_Ang[0],lab0.c_str(),"p");
   lab0 = lab+std::string(Form("%d",wStub[1]));
   leg->AddEntry(Eff_vs_Ang[1],lab0.c_str(),"p");
   lab0 = lab+std::string(Form("%d",wStub[2]));
   leg->AddEntry(Eff_vs_Ang[2],lab0.c_str(),"p");
   leg->AddEntry(Eff_vs_Ang_SW,"Stub word","lp");

   for(int iw=0;iw<nwStub;iw++)
     {	
	Eff_vs_Ang[iw]->GetYaxis()->SetRangeUser(0.,1.);
	Eff_vs_Ang[iw]->GetXaxis()->SetTitle("Rotation angle [deg]");
	Eff_vs_Ang[iw]->GetYaxis()->SetTitle("Stub efficiency");
	
	if( iw == 0 ) Eff_vs_Ang[iw]->Draw("APL");
	else Eff_vs_Ang[iw]->Draw("PSL same");

	Eff_vs_Ang[iw]->GetYaxis()->SetRangeUser(0.,1.2);
     }
   
   Eff_vs_Ang_SW->Draw("PSL same");
   
   leg->Draw();
   c1->Print("pics/Eff_vs_Ang.eps");
   c1->Clear();

   for(int iw=0;iw<nwStub;iw++)
     {	
	Eff_vs_Pt[iw]->GetYaxis()->SetRangeUser(0.,1.);
	Eff_vs_Pt[iw]->GetXaxis()->SetTitle("p_{T} [GeV]");
	Eff_vs_Pt[iw]->GetYaxis()->SetTitle("Stub efficiency");
	
	if( iw == 0 ) Eff_vs_Pt[iw]->Draw("APL");
	else Eff_vs_Pt[iw]->Draw("PSL same");

	Eff_vs_Pt[iw]->GetYaxis()->SetRangeUser(0.,1.2);
     }
   
   Eff_vs_Pt_SW->Draw("PSL same");
   
   leg->Draw();
   c1->Print("pics/Eff_vs_Pt.eps");
   c1->Clear();
   
   gApplication->Terminate();
}

double errf(double v1,double ve1,double v2,double ve2)
{   
   if( v2 == 0 ) return -666;
   
   double err = pow(v2-v1,2)*ve1*ve1/pow(v2,4) +
     v1*v1*(ve2*ve2-ve1*ve1)/pow(v2,4);
   
   err = sqrt(err);
   
   return err;
}
