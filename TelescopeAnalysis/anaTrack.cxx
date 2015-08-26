#include "TROOT.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TSystem.h"

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <cmath>

int main()
{
   std::vector<double> *xPos = new std::vector<double>;
   std::vector<double> *yPos = new std::vector<double>;
   std::vector<double> *dxdz = new std::vector<double>;
   std::vector<double> *dydz = new std::vector<double>;
   int euEvt;

   double FEI4_z = 244.0; // mm
   double DUT_z = 416.0; // mm
   double dxCorr = 10.0; // mm
   
   TFile *f = TFile::Open("NTuple.root");
   TTree *tr = (TTree*)f->Get("tracks");
   tr->SetBranchAddress("xPos",&xPos);
   tr->SetBranchAddress("yPos",&yPos);
   tr->SetBranchAddress("dxdz",&dxdz);
   tr->SetBranchAddress("dydz",&dydz);
   tr->SetBranchAddress("euEvt",&euEvt);

   std::vector<int> *hits0 = new std::vector<int>;
   std::vector<int> *hits1 = new std::vector<int>;
   int dutEvt;

   std::vector<int> DUTEVT;
   std::vector<std::vector<int> > HITS0;
   std::vector<std::vector<int> > HITS1;

   std::vector<int> EUEVT;
   std::vector<std::vector<double> > XPOS;
   std::vector<std::vector<double> > YPOS;
   std::vector<std::vector<double> > DXDZ;
   std::vector<std::vector<double> > DYDZ;
   
   TFile *fDut = TFile::Open("DUT.root");
   TTree *trDut = (TTree*)fDut->Get("DUT");
   trDut->SetBranchAddress("hits0",&hits0);
   trDut->SetBranchAddress("hits1",&hits1);
   trDut->SetBranchAddress("dutEvt",&dutEvt);

   TFile *foutput = new TFile("output.root","RECREATE");
   TH1D *h_posDiffX = new TH1D("h_posDiffX","h_posDiffX",200,-10.,30.);
   h_posDiffX->Sumw2();
   TH1D *h_posDiffY = new TH1D("h_posDiffY","h_posDiffY",200,-10.,30.);
   h_posDiffY->Sumw2();
   TH1D *h_posDiffX_zoom = new TH1D("h_posDiffX_zoom","h_posDiffX_zoom",50,9.,11.);
   h_posDiffX_zoom->Sumw2();
   TH1D *h_effStubAll = new TH1D("h_effStubAll","h_effStubAll",256,0.,256.);
   h_effStubAll->Sumw2();
   TH1D *h_effStubPass = new TH1D("h_effStubPass","h_effStubPass",256,0.,256.);
   h_effStubPass->Sumw2();
   TH1D *h_dutStubPos = new TH1D("h_dutStubPos","h_dutStubPos",256,0.,256.);
   h_dutStubPos->Sumw2();
   TH1D *h_telStubPos = new TH1D("h_telStubPos","h_telStubPos",256,0.,256.);
   h_telStubPos->Sumw2();
   TH1D *h_dxStubPos = new TH1D("h_dxStubPos","h_dxStubPos",512,-256.5,255.5);
   h_dxStubPos->Sumw2();
   TH2D *h_telBeamProf = new TH2D("h_telBeamProf","h_telBeamProf",50,50,-500.,500.,-500.,500.);
   h_telBeamProf->Sumw2();
   TH1D *h_stripProf = new TH1D("h_stripProf","h_stripProf",50,-10.,10.);
   h_stripProf->Sumw2();
   
   int nEventsDut = trDut->GetEntries();
   
   int nEvents = tr->GetEntries();
   
   for(int j=0;j<nEventsDut;j++)
     {
	trDut->GetEntry(j);
	
	DUTEVT.push_back(dutEvt);
	std::vector<int> hits0Copy(*hits0);
	std::vector<int> hits1Copy(*hits1);
	HITS0.push_back(hits0Copy);
	HITS1.push_back(hits1Copy);
     }

   for(int j=0;j<nEvents;j++)
     {
	tr->GetEntry(j);

	EUEVT.push_back(euEvt);
	std::vector<double> xPosCopy(*xPos);
	std::vector<double> yPosCopy(*yPos);
	std::vector<double> dxdzCopy(*dxdz);
	std::vector<double> dydzCopy(*dydz);
	XPOS.push_back(xPosCopy);
	YPOS.push_back(yPosCopy);
	DXDZ.push_back(dxdzCopy);
	DYDZ.push_back(dydzCopy);
     }
   
   f->Close();
   fDut->Close();

   int stubw = 7;
   
   for(int i=0;i<nEvents;i++)
     {
	for(int j=0;j<nEventsDut;j++)
	  {
	     if( DUTEVT[j] != EUEVT[i] ) continue;

	     int stubPos = -1;
	     
	     int nStrips = HITS0[j].size();
	     for(int is=0;is<nStrips;is++)
	       {
		  if( HITS0[j][is] == 1 )
		    {
		       int nStrips2 = HITS1[j].size();
		       
		       int nmin = (is-stubw < 0) ? 0 : is-stubw;
		       int nmax = (is+stubw+1 > nStrips2) ? nStrips2 : is+stubw+1;
		       for(int is2=nmin;is2<nmax;is2++)
			 {
			    if( HITS1[j][is2] == 1 )
			      {
				 stubPos = is;
				 break;
			      }			    
			 }		       
		    }		  
	       }	     

	     int nTracks = XPOS[i].size();
	     if( nTracks != 1 ) continue;
	     
	     double dzDUT = (DUT_z-FEI4_z);
	     double xDUT = dzDUT*DXDZ[i][0]+XPOS[i][0];
	     double yDUT = dzDUT*DYDZ[i][0]+YPOS[i][0];

	     h_telBeamProf->Fill(xDUT,yDUT);
	     
	     int effStubPos = abs(128+floor(xDUT*1000./90.));
	     
	     h_dutStubPos->Fill(stubPos);
	     h_telStubPos->Fill(effStubPos);
	     if( stubPos >= 0 ) h_dxStubPos->Fill(effStubPos-stubPos);

	     if( stubPos == 150 )
	       {
		  h_stripProf->Fill(yDUT);
	       }	     
	     
	     int nBins = h_effStubAll->GetXaxis()->GetNbins();
	     if( effStubPos <= nBins )
	       {
		  h_effStubAll->Fill(effStubPos);
		  if( abs(effStubPos-20-stubPos) <= 7 && stubPos >= 0 )
		    {
		       h_effStubPass->Fill(effStubPos);
		    }	     
	       }	     
	     
	     if( stubPos >= 0. )
	       {
		  h_posDiffX->Fill((stubPos*90-xDUT*1000.)/1000.);
		  h_posDiffX_zoom->Fill((stubPos*90-xDUT*1000.)/1000.);
		  h_posDiffY->Fill((stubPos*90-yDUT*1000.)/1000.);
	       }	     

	     /*	double dzP0 = (0.-FEI4_z);
	      double xP0 = dzP0*dxdz->at(i)+xPos->at(i);
	      double yP0 = dzP0*dydz->at(i)+yPos->at(i);
	      * 
	      double dzP1 = (101.-FEI4_z);
	      double xP1 = dzP1*dxdz->at(i)+xPos->at(i);
	      double yP1 = dzP1*dydz->at(i)+yPos->at(i);
	      * 
	      double dzP2 = (206.-FEI4_z);
	      double xP2 = dzP2*dxdz->at(i)+xPos->at(i);
	      double yP2 = dzP2*dydz->at(i)+yPos->at(i);
	      * 
	      std::cout << xP0*1000. << " " << yP0*1000. << std::endl;
	      std::cout << xP1*1000. << " " << yP1*1000. << std::endl;
	      std::cout << xP2*1000. << " " << yP2*1000. << std::endl;*/
	  }	
     }   

   foutput->Write();
   foutput->Close();
}
