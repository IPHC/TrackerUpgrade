#ifndef VAR_H
#define VAR_H

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLegend.h"
#include "TCanvas.h"

#include <iostream>

std::vector<int> *dutt;
std::vector<int> *dutb;
std::vector<int> *fixt;
std::vector<int> *fixb;

std::string fn1;
//std::string fn2;

TLegend *leg;
TCanvas *c1;

void registerBranches(TFile *fi,TTree *tr)
{
   fi->cd();
   
   tr->SetBranchAddress("DUTT",&dutt);
   tr->SetBranchAddress("DUTB",&dutb);
}

std::map<std::string,TH1D*> v1;
//std::map<std::string,TH1D*> v2;

const int nf = 1;

TH1D *h_DUTT_data[nf];
TH1D *h_DUTB_data[nf];
TH1D *h_FIXT_data[nf];
TH1D *h_FIXB_data[nf];
TH1D *h_DUT_stubs_n[nf];
TH1D *h_FIX_stubs_n[nf];
TH1D *h_DUT_stubs[nf];
TH1D *h_FIX_stubs[nf];

void createHist(std::map<std::string,TH1D*>* v, int i)
{
   std::string DUTT_data_name = "h_DUTT_data";
   std::string h_DUTT_data_name = DUTT_data_name+"_"+std::string(Form("%d",i));
   h_DUTT_data[i] = new TH1D(h_DUTT_data_name.c_str(),h_DUTT_data_name.c_str(),256,-0.5,255.5);
   h_DUTT_data[i]->Sumw2();
   v->insert(std::pair<std::string,TH1D*>(DUTT_data_name,h_DUTT_data[i]));

   std::string DUTB_data_name = "h_DUTB_data";
   std::string h_DUTB_data_name = DUTB_data_name+"_"+std::string(Form("%d",i));
   h_DUTB_data[i] = new TH1D(h_DUTB_data_name.c_str(),h_DUTB_data_name.c_str(),256,-0.5,255.5);
   h_DUTB_data[i]->Sumw2();
   v->insert(std::pair<std::string,TH1D*>(DUTB_data_name,h_DUTB_data[i]));

   std::string DUT_stubs_n_name = "h_DUT_stubs_n";
   std::string h_DUT_stubs_n_name = DUT_stubs_n_name+"_"+std::string(Form("%d",i));
   h_DUT_stubs_n[i] = new TH1D(h_DUT_stubs_n_name.c_str(),h_DUT_stubs_n_name.c_str(),50,0.,50.);
   h_DUT_stubs_n[i]->Sumw2();
   v->insert(std::pair<std::string,TH1D*>(DUT_stubs_n_name,h_DUT_stubs_n[i]));

   std::string FIX_stubs_n_name = "h_FIX_stubs_n";
   std::string h_FIX_stubs_n_name = FIX_stubs_n_name+"_"+std::string(Form("%d",i));
   h_FIX_stubs_n[i] = new TH1D(h_FIX_stubs_n_name.c_str(),h_FIX_stubs_n_name.c_str(),50,0.,50.);
   h_FIX_stubs_n[i]->Sumw2();
   v->insert(std::pair<std::string,TH1D*>(FIX_stubs_n_name,h_FIX_stubs_n[i]));

   std::string DUT_stubs_name = "h_DUT_stubs";
   std::string h_DUT_stubs_name = DUT_stubs_name+"_"+std::string(Form("%d",i));
   h_DUT_stubs[i] = new TH1D(h_DUT_stubs_name.c_str(),h_DUT_stubs_name.c_str(),256,-0.5,255.5);
   h_DUT_stubs[i]->Sumw2();
   v->insert(std::pair<std::string,TH1D*>(DUT_stubs_name,h_DUT_stubs[i]));

   std::string FIX_stubs_name = "h_FIX_stubs";
   std::string h_FIX_stubs_name = FIX_stubs_name+"_"+std::string(Form("%d",i));
   h_FIX_stubs[i] = new TH1D(h_FIX_stubs_name.c_str(),h_FIX_stubs_name.c_str(),256,-0.5,255.5);
   h_FIX_stubs[i]->Sumw2();
   v->insert(std::pair<std::string,TH1D*>(FIX_stubs_name,h_FIX_stubs[i]));
}

#endif
