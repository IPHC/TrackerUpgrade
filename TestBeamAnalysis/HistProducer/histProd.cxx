#include "histFunc.h"

#include "Riostream.h"
#include "TROOT.h"
#include "TStyle.h"

#include <vector>
#include <map>
#include <string>

#ifdef __MAKECINT__

#pragma link C++ class std::vector<int>+;
#pragma link C++ class std::map<std::string,std::vector<int> >+;

#endif

void fillHist();

int main(int argc, char *argv[])
{
   if( argc < 4 )
     {
	std::cout << "Please specify input parameters" << std::endl;
	exit(1);
     }

   dut_row = new std::map<std::string,std::vector<int> >();
   
   std::string fin1 = argv[1];
   std::string fout1 = argv[2];
   nmax = atoi(argv[3]);

   f1 = TFile::Open(fin1.c_str());
   tr1 = (TTree*)f1->Get("treeMaker/tbeamTree");

   registerBranches();

   TFile *fout = new TFile(fout1.c_str(),"RECREATE");
   
   createHist();
   
   fillHist();
   
   fout->Write();
   fout->Close();
}

void fillHist()
{
   int nev = tr1->GetEntries();

   for( int i=0;i<nev;i++ )
     {
	if( i > nmax && nmax >= 0. ) break;
	
	tr1->GetEntry(i);
	
	std::map<std::string,std::vector<int> >::iterator it0 = dut_row->find("det0");
	std::map<std::string,std::vector<int> >::iterator it1 = dut_row->find("det1");
	
	bool hasHits0 = 1; 
	if( it0 == dut_row->end() ) hasHits0 = 0;
	
	bool hasHits1 = 1; 
	if( it1 == dut_row->end() ) hasHits1 = 0;
	
	std::vector<int> s0_Raw;
	std::vector<int> s1_Raw;
	std::vector<int> s0_Shift;
	std::vector<int> s1_Shift;
	std::vector<int> s0;
	std::vector<int> s1;
	hits0.clear();
	hits1.clear();

	int nHits0 = 0;
	int nHits1 = 0;
	
	if( hasHits0 ) 
	  {
	     s0_Raw = it0->second;
	     nHits0 = s0_Raw.size();
	     for(int ih=0;ih<nHits0;ih++)
	       {	
		  int ipos = s0_Raw[ih];
		  if( s0_Raw[ih] < 127 ) ipos = 126-s0_Raw[ih];
		  else ipos = 127+(253-s0_Raw[ih]);
		  s0_Shift.push_back(ipos);
	       }
	     for(int j=0;j<256;j++)
	       {
		  int hit = 0;
		  for(int ih=0;ih<nHits0;ih++)
		    {		      
		       if( s0_Shift[ih] == j ) hit = 1;
		    }		  
		  s0.push_back(hit);
	       }	     
	  }
	
	if( hasHits1 )
	  {
	     s1_Raw = it1->second;
	     nHits1 = s1_Raw.size();
	     for(int ih=0;ih<nHits1;ih++)
	       {
		  int ipos = s1_Raw[ih];
		  if( s1_Raw[ih] < 127 ) ipos = 126-s1_Raw[ih];
		  else ipos = 127+(253-s1_Raw[ih]);
		  s1_Shift.push_back(ipos);
	       }	     
	     for(int j=0;j<256;j++)
	       {
		  int hit = 0;
		  for(int ih=0;ih<nHits1;ih++)
		    {		      
		       if( s1_Shift[ih] == j ) hit = 1;
		    }		  
		  s1.push_back(hit);
	       }	     
	  }	
	
	for(int ih=0;ih<nHits0;ih++) h_s0_data->Fill(s0_Shift[ih]);
	for(int ih=0;ih<nHits1;ih++) h_s1_data->Fill(s1_Shift[ih]);
	
	h_occ->Fill(0);
	if( hasHits0 ) h_occ->Fill(1);
	if( hasHits1 ) h_occ->Fill(2);
	if( hasHits0 && hasHits1 ) h_occ->Fill(3);
	if( !hasHits0 ) h_occ->Fill(5);
	if( !hasHits1 ) h_occ->Fill(6);
	if( !hasHits0 && !hasHits1 ) h_occ->Fill(7);
	if( stubWord ) h_occ->Fill(4);
	
	if( hasHits0 && hasHits1 )
	  {
	     int nh = s0.size();
	     for(int ih=0;ih<nh;ih++)
	       {
		  for(int j=0;j<nh;j++)
		    {
		       if( ih == j ) continue;
		       if( s0[ih] == 1 && s1[j] == 1 )
			 {			    
			    float dx = ih-j;
			    h_s_dx->Fill(dx);
			 }		       
		    }		  
	       }	     
	  }

	if( hasHits0 && hasHits1 )
	  {
	     h_stubWord->Fill(stubWord);
	  }	

	if( hasHits0 || hasHits1 )
	  {
	     h_stubWord_oneHit->Fill(stubWord);
	  }	

	if( hasHits0 )
	  {
	     h_stubWord_plane0->Fill(stubWord);
	  }	

	if( hasHits1 )
	  {
	     h_stubWord_plane1->Fill(stubWord);
	  }	
	
	h_stubWord_noCut->Fill(stubWord);
	
/*	bool haveHits0w20 = 0;
	for(int i1=0;i1<s0.size();i1++)
	  {
	     for(int i2=0;i2<s1.size();i2++)
	       {
		  if( fabs(s0[i1]-s1[i2]) < 20 ) haveHits0w20 = 1;
	       }
	  }*/

//	if( haveHits0w20 )
	  {	     
	     for(int iw=0;iw<nw;iw++)
	       {
		  std::vector<STUB*> stubs = getStubs(&s0,&s1,wsize[iw],0,255);
		  std::vector<STUB*> stubs_A = getStubs(&s0,&s1,wsize[iw],0,128);
		  std::vector<STUB*> stubs_B = getStubs(&s0,&s1,wsize[iw],128,255);
		  
		  int ns = stubs.size();
		  
		  if( hasHits0 && hasHits1 )
		    {		  
		       h_stubs[iw]->Fill(ns);
		    }	     

		  if( hasHits0 || hasHits1 )
		    {		  
		       h_stubs_oneHit[iw]->Fill(ns);
		    }	     

		  if( hasHits0 )
		    {		  
		       h_stubs_plane0[iw]->Fill(ns);
		    }	     

		  if( hasHits1 )
		    {		  
		       h_stubs_plane1[iw]->Fill(ns);
		    }	     
		  
		  h_stubs_noCut[iw]->Fill(ns);
	       }
	  }	

	std::vector<CLUST*> s0_clusters = getClusters(&s0,0,256);
	std::vector<CLUST*> s1_clusters = getClusters(&s1,0,256);

//	if( hasHits0 && hasHits1 )
	if( stubWord )
	  {		  
	     int s0_n = s0_clusters.size();
	     double clusterSizeMean = 0.;
	     for(int ic=0;ic<s0_n;ic++)
	       {
		  clusterSizeMean += s0_clusters.at(ic)->hits.size();
	       }
	     clusterSizeMean /= double(s0_n);
	     h_s0_clusterSize->Fill(clusterSizeMean);

	     int s1_n = s1_clusters.size();
	     clusterSizeMean = 0.;
	     for(int ic=0;ic<s1_n;ic++)
	       {
		  clusterSizeMean += s1_clusters.at(ic)->hits.size();
	       }
	     clusterSizeMean /= double(s1_n);
	     h_s1_clusterSize->Fill(clusterSizeMean);
	  }	     
	
	hits0 = s0;
	hits1 = s1;
	dutEvt = event;
	
	tr->Fill();
	
     }// event loop
   
//   tr->Write();
}
