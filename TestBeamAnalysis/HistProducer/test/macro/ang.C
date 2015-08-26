double ANG2PI=3.14159265359/180.;

void ang()
{
   gROOT->SetBatch();
   gROOT->SetStyle("Plain");

   gROOT->ProcessLine(".x common.C");

   TCanvas *c1 = new TCanvas("c1","c1",0,0,600,500);

   double minang = 26.;
   double maxang = 89.;
   double agl[1000];
   int na = 0;
   for(int i=0;i<1000;i++)
     {
	if( minang+i > maxang ) break;
	agl[i] = minang+i;
	na++;
     }
   int nagl =const_cast<int>(na);

   const int np = 8;
   double p[np] = 
     {0.5,1.0,1.5,2.0,2.5,3.0,3.5,4.0}; // GeV
//   double p[np] = 
//     {1.0,2.0,4.0,10.0,50.0,100.0,500.0}; // GeV
   int col[np] =
     {1,800,632,416,600,400,616,432};
   
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

   TGraph *gr[np];

   TLegend *leg = new TLegend(0.75,0.50,0.90,0.20);
   leg->SetFillColor(253);
   leg->SetBorderSize(0);
   
   for(int ip=0;ip<np;ip++)
     {       
	int idx = 0;

	double tran[1000];
	double perp[1000];
	
	for(int i=0;i<nagl;i++)
	  {
	     double pl = p[ip]*TMath::Cos(agl[i]*ANG2PI);
	     double pp = p[ip]*TMath::Sin(agl[i]*ANG2PI);
	     
	     double R = pp/0.3/B;
	     
	     double ag = aimp(r,R,pl,pp);

	     if( ag == -666 )
	       {
		  continue;
	       }	   
	     tran[idx] = ag;
	     perp[idx] = pp;

	     idx++;
	  }   

	gr[ip] = new TGraph(idx,perp,tran);
   
	if( ip == 0 ) gr[ip]->Draw("AL");
	else gr[ip]->Draw("SL");
	gr[ip]->GetXaxis()->SetLimits(0.,4.5);
//	gr[ip]->GetXaxis()->SetLimits(1.0,1000.0);
	gr[ip]->GetXaxis()->SetTitle("P_{T} [GeV]");
	gr[ip]->GetYaxis()->SetTitle("Angle");
	gr[ip]->SetLineColor(col[ip]);
	gr[ip]->SetLineWidth(2);
	std::string pname = std::string(Form("%.1f",p[ip]))+" GeV";
	leg->AddEntry(gr[ip],pname.c_str(),"l");
     }
   leg->Draw();

//   c1->SetLogx(1);
   c1->Print("pics/ang.eps");
   
   gApplication->Terminate();
}

double aimp(double r,double R,double pl,double pp)
{
   double rat = r/2/R;
   if( rat > 1. )
     {
	return = -666;
     }
   
   double tn = pp/pl*TMath::Cos(TMath::ASin(rat));
   
   return TMath::ATan(tn)/ANG2PI;
}
