double ANG2PI=3.14159265359/180.;

void ang2()
{
   gROOT->SetBatch();
   gROOT->SetStyle("Plain");

   gROOT->ProcessLine(".x common.C");

   TCanvas *c1 = new TCanvas("c1","c1",0,0,600,500);

   double minpt = 0.1;
   double maxpt = 3.;
   double pt[10000];
   int npt = 0;
   for(int i=0;i<10000;i++)
     {
	if( minpt+i*0.1 > maxpt ) break;
	pt[i] = minpt+i*0.1;
	npt++;
     }
   int npt =const_cast<int>(npt);

   const int na = 7;
   double ang[na] = 
     {25.,35.,45.,55.,65.,75.,85.};
   int col[na] =
     {1,800,632,416,600,616,800};
   
   // colors
   // 1 kBlack
   // 920 kGray
   // 632 kRed
   // 416 kGreen
   // 600 kBlue
   // 400 kYellow
   // 616 kMagenta
   // 432 kCyan
   // 800 kOrange
   // 820 kSpring
   // 840 kTeal
   // 860 kAzure
   // 880 kViolet
   // 900 kPink
   
   double B = 4; // T
   double r = 0.6; // m
//   double r = 1.1; // m
   double h = 2.8*10E-3;
   double w = 90*10E-6;
   
   TGraph *gr[10000];
   TGraph *grd[10000];

   TLegend *leg = new TLegend(0.75,0.50,0.90,0.20);
   leg->SetFillColor(253);
   leg->SetBorderSize(0);
   
   for(int ia=0;ia<na;ia++)
     {       
	int idx = 0;

	double tran[10000];
	double perp[10000];
	double delta[10000];
	
	for(int i=0;i<npt;i++)
	  {
	     double p = pt[i]/TMath::Sin(ang[ia]*ANG2PI);
	     double pl = p*TMath::Cos(ang[ia]*ANG2PI);
	     double pp = pt[i];
	     
	     double R = pp/0.3/B;
	     
	     double ag = aimp(r,R,pl,pp);

	     if( ag == -666 )
	       {
		  continue;
	       }	 
	     
	     double phi = 90.-ag;
	     
	     tran[idx] = phi;
	     perp[idx] = pp;
	     delta[idx] = h/w/ag;

	     idx++;
	  }   

	gr[ia] = new TGraph(idx,perp,tran);
	grd[ia] = new TGraph(idx,perp,delta);
     }

   for(int ia=0;ia<na;ia++)
     {          
	if( ia == 0 ) gr[ia]->Draw("AL");
	else gr[ia]->Draw("SL");
	gr[ia]->GetYaxis()->SetRangeUser(0.,90.);
	gr[ia]->GetXaxis()->SetLimits(0.2,3.2);
	gr[ia]->GetXaxis()->SetTitle("P_{T} [GeV]");
	gr[ia]->GetYaxis()->SetTitle("#varphi [deg]");
	gr[ia]->SetLineColor(col[ia]);
	gr[ia]->SetLineWidth(2);
	std::string pname = std::string(Form("%.0f",ang[ia]))+" deg";
	leg->AddEntry(gr[ia],pname.c_str(),"l");
     }
   leg->Draw();
//   c1->SetLogx(1);
   c1->Print("pics/ang.eps");
   c1->Clear();
   leg->Clear();

   for(int ia=0;ia<na;ia++)
     {          
	if( ia == 0 ) grd[ia]->Draw("AL");
	else grd[ia]->Draw("SL");
	grd[ia]->GetYaxis()->SetRangeUser(0.,4.);
	grd[ia]->GetXaxis()->SetLimits(0.2,3.2);
	grd[ia]->GetXaxis()->SetTitle("P_{T} [GeV]");
	grd[ia]->GetYaxis()->SetTitle("W_{size}");
	grd[ia]->SetLineColor(col[ia]);
	grd[ia]->SetLineWidth(2);
	std::string pname = std::string(Form("%.0f",ang[ia]))+" deg";
	leg->AddEntry(grd[ia],pname.c_str(),"l");
     }
   leg->Draw();
   
   c1->Print("pics/delta.eps");
   
   gApplication->Terminate();
}

double aimp(double r,double R,double pl,double pp)
{
   double rat = r/2/R;
   if( rat >= 1 )
     {
	return = -666;
     }
   
   double tn = pp/pl*TMath::Cos(TMath::ASin(rat));
   double ag = TMath::ATan(tn)/ANG2PI;
   
   return ag;
}
