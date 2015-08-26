double ANG2PI=3.14159265359/180.;

void ang3()
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

   double B = 4; // T
   double h = 2.8*10E-3;
   double w = 90*10E-6;

   const int nr = 2;
   double r[2] = 
     {0.6,1.1};
   double col[2] =
     {1,632};
   
   TGraph *gr[10000];
   TGraph *grd[10000];

   TLegend *leg = new TLegend(0.75,0.85,0.90,0.70);
   leg->SetFillColor(253);
   leg->SetBorderSize(0);
   
   for(int ir=0;ir<nr;ir++)
     {	
	int idx = 0;
	
	double tran[10000];
	double perp[10000];
	double delta[10000];
	
	for(int i=0;i<npt;i++)
	  {
	     double pp = pt[i];
	     
	     double R = pp/0.3/B;
	     
	     double ag = aimp(r[ir],R);
	
	     if( ag == -666 )
	       {
		  continue;
	       }	 
	     
	     tran[idx] = ag/ANG2PI;
	     perp[idx] = pp;
	     delta[idx] = h/w*ag;
	     
	     idx++;
	  }   
   
	gr[ir] = new TGraph(idx,perp,tran);
	grd[ir] = new TGraph(idx,perp,delta);
     }   

   for(int ir=0;ir<nr;ir++)
     {	
	if( ir == 0 ) gr[ir]->Draw("AL");
	else gr[ir]->Draw("LS");
	gr[ir]->GetYaxis()->SetRangeUser(0.,90.);
	gr[ir]->GetXaxis()->SetLimits(0.2,3.2);
	gr[ir]->GetXaxis()->SetTitle("P_{T} [GeV]");
	gr[ir]->GetYaxis()->SetTitle("#varphi [deg]");
	gr[ir]->SetLineColor(col[ir]);
	gr[ir]->SetLineWidth(2);
	if( ir == 1 ) gr[ir]->SetLineStyle(2);
	std::string pname = std::string(Form("%.1f",r[ir]))+" m";
	leg->AddEntry(gr[ir],pname.c_str(),"l");
     }
   
   leg->Draw();
   c1->Print("pics/ang.eps");
   c1->Clear();
   leg->Clear();

   for(int ir=0;ir<nr;ir++)
     {	
	if( ir == 0 ) grd[ir]->Draw("AL");
	else grd[ir]->Draw("LS");
	grd[ir]->GetYaxis()->SetRangeUser(0.,40.);
	grd[ir]->GetXaxis()->SetLimits(0.2,3.2);
	grd[ir]->GetXaxis()->SetTitle("P_{T} [GeV]");
	grd[ir]->GetYaxis()->SetTitle("W_{size}");
	grd[ir]->SetLineColor(col[ir]);
	grd[ir]->SetLineWidth(2);
	if( ir == 1 ) grd[ir]->SetLineStyle(2);
	std::string pname = std::string(Form("%.1f",r[ir]))+" m";
	leg->AddEntry(grd[ir],pname.c_str(),"l");
     }
   
   leg->Draw();
   
   c1->Print("pics/delta.eps");
   
   gApplication->Terminate();
}

double aimp(double r,double R)
{
   double rat = r/2/R;
   if( rat >= 1 )
     {
	return = -666;
     }
   
   double ag = TMath::ASin(rat);
   
   return ag;
}
