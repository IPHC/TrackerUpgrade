#include "var.h"
#include "func.h"

#include "Riostream.h"
#include "TROOT.h"
#include "TStyle.h"

void fillHist(TTree*,std::map<std::string,TH1D*>*);
void draw(std::string,std::string);

int main(int argc, char *argv[])
{
   if( argc < 3 )
     {
	std::cout << "Please specify input parameters" << std::endl;
	exit(1);
     }

   dutt = new std::vector<int>();
   dutb = new std::vector<int>();
   fixt = new std::vector<int>();
   fixb = new std::vector<int>();
   
   std::string fin1 = argv[1];
//   std::string fin2 = argv[2];
   fn1 = argv[2];
//   fn2 = argv[4];
   
   gROOT->SetBatch();
   gROOT->SetStyle("Plain");
   
   gStyle->SetOptStat(0);

   gROOT->ProcessLine(".x common.C");

   c1 = new TCanvas("c1","c1",0,0,600,500);
   c1->Draw();
   c1->cd();
   c1->SetLogy(1);

   TPad *c1_1;
   
   gStyle->SetHistTopMargin(0);

   leg = new TLegend(0.65,0.90,0.90,0.75);
   leg->SetFillColor(253);
   leg->SetBorderSize(0);

   TFile *f1 = TFile::Open(fin1.c_str());
   TTree *tr1 = (TTree*)f1->Get("SiPixelAna/DATA");

//   TFile *f2 = TFile::Open(fin2.c_str());
//   TTree *tr2 = (TTree*)f2->Get("HepMCVal/gen");
   
   registerBranches(f1,tr1);
//   registerBranches(f2,tr2);
   
   createHist(&v1,0);
//   createHist(&v2,1);
   
   fillHist(tr1,&v1);
//   fillHist(tr2,&v2);
   
   draw("h_DUTT_data","Strip ID");
   draw("h_DUTB_data","Strip ID");
   draw("h_FIXT_data","Strip ID");
   draw("h_FIXB_data","Strip ID");
   draw("h_DUT_stubs_n","Number of stubs");
   draw("h_FIX_stubs_n","Number of stubs");
   draw("h_DUT_stubs","Strip ID");
   draw("h_FIX_stubs","Strip ID");
}

void fillHist(TTree *tr,std::map<std::string,TH1D*> *v)
{
   int nev = tr->GetEntries();
   for( int i=0;i<nev;i++ )
     {
	tr->GetEntry(i);
	
	for(int j=0;j<dutt->size();j++)
	  {	 
	     if( dutt->at(j) > 0 ) v->find("h_DUTT_data")->second->Fill(j);
	  }
	for(int j=0;j<dutb->size();j++)
	  {	 
	     if( dutb->at(j) > 0 ) v->find("h_DUTB_data")->second->Fill(j);
	  }

	for(int j=0;j<fixt->size();j++)
	  {	 
	     if( fixt->at(j) > 0 ) v->find("h_FIXT_data")->second->Fill(j);
	  }
	for(int j=0;j<fixb->size();j++)
	  {	 
	     if( fixb->at(j) > 0 ) v->find("h_FIXB_data")->second->Fill(j);
	  }
	
	std::vector<int> DUT_stubs = getStubs(dutt,dutb);
	v->find("h_DUT_stubs_n")->second->Fill(DUT_stubs.size());		

	std::vector<int> FIX_stubs = getStubs(fixt,fixb);
	v->find("h_FIX_stubs_n")->second->Fill(FIX_stubs.size());
	
	for(int ie=0;ie<DUT_stubs.size();ie++)
	  {
	     v->find("h_DUT_stubs")->second->Fill(DUT_stubs[ie]);
	  }	
	for(int ie=0;ie<FIX_stubs.size();ie++)
	  {
	     v->find("h_FIX_stubs")->second->Fill(FIX_stubs[ie]);
	  }	
     }
}

void draw(std::string hn,std::string xaxist)
{   
   TH1D *h1 = v1.find(hn.c_str())->second;
//   addbin(h1);
//   h1->Scale(1./h1->Integral());
   h1->SetMarkerSize(0.0);
   h1->Draw("hist e1");
/*   TH1D *h2 = v2.find(hn.c_str())->second;
   addbin(h2);
   h2->Scale(1./h2->Integral());
   h2->Draw("hist e1p same");
   h2->SetLineColor(kRed);
   h2->SetMarkerColor(kRed);
   h2->SetMarkerStyle(20);
   h2->SetMarkerSize(0.7);*/
//   leg->AddEntry(h1,fn1.c_str(),"lp");
//   leg->AddEntry(h2,fn2.c_str(),"lp");
//   leg->Draw();
   h1->GetXaxis()->SetTitle(xaxist.c_str());
/*   double m1 = h1->GetMaximum();
   double m2 = h2->GetMaximum();
   if( m1 > m2 ) h1->SetMaximum(1.3*m1);
   else h1->SetMaximum(1.3*m2);*/
   std::string fsave = "pics/"+hn+".eps";
   c1->Print(fsave.c_str());
   c1->Clear();
   leg->Clear();
}
