#ifndef VAR_H
#define VAR_H

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"

#include <iostream>
#include <math.h>

TFile *f1;
TTree *tr1;

const int nw = 9;
int wsize[nw] = {0,1,2,3,4,5,6,7,8};
int nmax = -1;

// Ntuple variables
std::map<std::string,std::vector<int> > *dut_row;
UInt_t stubWord;

// Histograms
TH1D *h_s0_data;
TH1D *h_s1_data;
TH1D *h_s_dx;
TH1D *h_stubs[nw];
TH1D *h_stubs_noCut[nw];
TH1D *h_stubs_oneHit[nw];
TH1D *h_stubs_plane0[nw];
TH1D *h_stubs_plane1[nw];
TH1D *h_stubWord;
TH1D *h_stubWord_noCut;
TH1D *h_stubWord_oneHit;
TH1D *h_stubWord_plane0;
TH1D *h_stubWord_plane1;
TH1D *h_s0_clusterSize;
TH1D *h_s1_clusterSize;

// ROOT read functions
void registerBranches()
{
   tr1->SetBranchAddress("dut_row",&dut_row);
   tr1->SetBranchAddress("stubWord",&stubWord);
}

// Create histograms
void createHist()
{
   // hits in sensor 0
   std::string hname = "h_s0_data";
   h_s0_data = new TH1D(hname.c_str(),hname.c_str(),256,0,256);
   h_s0_data->Sumw2();

   // hits in sensor 1
   hname = "h_s1_data";
   h_s1_data = new TH1D(hname.c_str(),hname.c_str(),256,0,256);
   h_s1_data->Sumw2();

   // dx for all hits
   hname = "h_s_dx";
   h_s_dx = new TH1D(hname.c_str(),hname.c_str(),150,-25,25);
   h_s_dx->Sumw2();
   
   // stubs
   for(int i=0;i<nw;i++)
     {
	hname = "h_stubs_w"+std::string(Form("%d",wsize[i]));
	h_stubs[i] = new TH1D(hname.c_str(),hname.c_str(),5,0,5);
	h_stubs[i]->Sumw2();
     }   

   // stubs_noCut
   for(int i=0;i<nw;i++)
     {
	hname = "h_stubs_noCut_w"+std::string(Form("%d",wsize[i]));
	h_stubs_noCut[i] = new TH1D(hname.c_str(),hname.c_str(),5,0,5);
	h_stubs_noCut[i]->Sumw2();
     }   

   // stubs_oneHit
   for(int i=0;i<nw;i++)
     {
	hname = "h_stubs_oneHit_w"+std::string(Form("%d",wsize[i]));
	h_stubs_oneHit[i] = new TH1D(hname.c_str(),hname.c_str(),5,0,5);
	h_stubs_oneHit[i]->Sumw2();
     }   

   // stubs_plane0
   for(int i=0;i<nw;i++)
     {
	hname = "h_stubs_plane0_w"+std::string(Form("%d",wsize[i]));
	h_stubs_plane0[i] = new TH1D(hname.c_str(),hname.c_str(),5,0,5);
	h_stubs_plane0[i]->Sumw2();
     }   

   // stubs_plane1
   for(int i=0;i<nw;i++)
     {
	hname = "h_stubs_plane1_w"+std::string(Form("%d",wsize[i]));
	h_stubs_plane1[i] = new TH1D(hname.c_str(),hname.c_str(),5,0,5);
	h_stubs_plane1[i]->Sumw2();
     }   
   
   hname = "h_stubWord";
   h_stubWord = new TH1D(hname.c_str(),hname.c_str(),5,0,5);
   h_stubWord->Sumw2();

   hname = "h_stubWord_noCut";
   h_stubWord_noCut = new TH1D(hname.c_str(),hname.c_str(),5,0,5);
   h_stubWord_noCut->Sumw2();

   hname = "h_stubWord_oneHit";
   h_stubWord_oneHit = new TH1D(hname.c_str(),hname.c_str(),5,0,5);
   h_stubWord_oneHit->Sumw2();

   hname = "h_stubWord_plane0";
   h_stubWord_plane0 = new TH1D(hname.c_str(),hname.c_str(),5,0,5);
   h_stubWord_plane0->Sumw2();

   hname = "h_stubWord_plane1";
   h_stubWord_plane1 = new TH1D(hname.c_str(),hname.c_str(),5,0,5);
   h_stubWord_plane1->Sumw2();
   
   // cluster size in sensor 0
   hname = "h_s0_clusterSize";
   h_s0_clusterSize = new TH1D(hname.c_str(),hname.c_str(),30,0,5);
   h_s0_clusterSize->Sumw2();

   // cluster size in sensor 1
   hname = "h_s1_clusterSize";
   h_s1_clusterSize = new TH1D(hname.c_str(),hname.c_str(),30,0,5);
   h_s1_clusterSize->Sumw2();
}

class STUB
{
 public:
   STUB(int _pos,std::vector<int> _hits)
     {
	pos = _pos;
	hits = _hits;
     }
   int pos;
   std::vector<int> hits;
};

class CLUST
{
 public:
   CLUST(float _pos,std::vector<int> _hits)
     {
	pos = _pos;
	hits = _hits;
     };
   CLUST() {};
   float pos;
   std::vector<int> hits;
};

class CSTUB
{
 public:
   CSTUB(CLUST _clt,CLUST _clb)
     {
	clt = _clt;
	clb = _clb;
     }
   CLUST clt;
   CLUST clb;
};

#endif
