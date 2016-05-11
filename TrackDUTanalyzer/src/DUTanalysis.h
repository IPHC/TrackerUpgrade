
#ifndef DUTANALYSIS_H
#define DUTANALYSIS_H

#include "ReadInputTrees.h"
//const int wsize[nw] = {0,1,2,3,4,5,6,7,8};


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
} ;

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


class DUTanalysis {

  public:

    DUTanalysis();
    ~DUTanalysis();

    int nMaxChannel;
    bool doHitShift;
 
    int ipos_max1, ipos_max2;

    std::vector<STUB*> getStubs(std::vector<int> *, std::vector<int> *, int, int, int);
    std::vector<CLUST*> getClusters(std::vector<int> *, int, int);
    std::vector<CSTUB*> getClusterStubs(std::vector<CLUST*>, std::vector<CLUST*>, int);

    std::vector<STUB*> stubs;

    void Setup(ReadInputTrees* );
    void doDUTanalysis(ReadInputTrees* );
    void doDUThistos(ReadInputTrees*);
};

DUTanalysis::DUTanalysis(){

}

void DUTanalysis::Setup(ReadInputTrees* trees){

       if ((*trees).BTcampaign == "JuneBT") {
          nMaxChannel = 255; //2CBC
          doHitShift = true; //Hits are inverted within CBC
        }
        if ((*trees).BTcampaign == "NovBT")  {
          nMaxChannel = 2048-16; //16CBC
          doHitShift = false;
        }

  if ((*trees).BTcampaign == "JuneBT"){
    (*trees).hitMap2D_hit0 = new TH2F("hitMap2D_hit0","hitMap2D_hit0",nMaxChannel,0,nMaxChannel-1,1,-0.5,0.5);
    (*trees).hitMap2D_hit1 = new TH2F("hitMap2D_hit1","hitMap2D_hit1",nMaxChannel,0,nMaxChannel-1,1,-0.5,0.5);
  }
  else if ((*trees).BTcampaign == "NovBT"){
    (*trees).hitMap2D_hit0 = new TH2F("hitMap2D_hit0","hitMap2D_hit0",nMaxChannel/2,0,nMaxChannel/2-1,2,-0.5,1.5);
    (*trees).hitMap2D_hit1 = new TH2F("hitMap2D_hit1","hitMap2D_hit1",nMaxChannel/2,0,nMaxChannel/2-1,2,-0.5,1.5);
    (*trees).hitMap1D_hit0 = new TH1F("hitMap1D_hit0","hitMap1D_hit0",nMaxChannel/2,0,nMaxChannel/2-1);
    (*trees).hitMap1D_hit1 = new TH1F("hitMap1D_hit1","hitMap1D_hit1",nMaxChannel/2,0,nMaxChannel/2-1);
    (*trees).stubMap1D = new TH1F("stubMap1D","stubMap1D",nMaxChannel/2,0,nMaxChannel/2-1);
  }

  (*trees).hitOccupancy_hit0 = new TH1F("hitOccupancy_hit0","hitOccupancy_hit0",nMaxChannel,0,nMaxChannel-1);
  (*trees).hitOccupancy_hit1 = new TH1F("hitOccupancy_hit1","hitOccupancy_hit1",nMaxChannel,0,nMaxChannel-1);
  (*trees).stubOccupancy = new TH1F("stubOccupancy", "stubOccupancy", nMaxChannel,0,nMaxChannel-1);

  ipos_max1 = 0;
  ipos_max2 = 0;

  //cout << "Setup done"<<endl;

  return;
}

DUTanalysis::~DUTanalysis(){

  cout << "ipos_max1="<<ipos_max1<<endl;
  cout << "ipos_max2="<<ipos_max2<<endl;
}

std::vector<STUB*> DUTanalysis::getStubs(std::vector<int> *cbc1,
                            std::vector<int> *cbc2,
                            int ws,
                            int chb,
                            int che)
{
   std::vector<STUB*> stubs;
   stubs.clear();
   
   int n1 = cbc1->size();
   
   bool ofr = 0;
   if( chb < 0 || chb > nMaxChannel ) ofr = 1;
   if( che < 0 || che > nMaxChannel ) ofr = 1;
   if( chb > che ) ofr = 1; 
   if( ofr ) 
     { 
        std::cout << "Channel range must be from 0 min to "<< nMaxChannel<<" max" << std::endl;
        exit(1);
     }   
     

   for(int i1=chb;i1<che;i1++)
     {
        if( cbc1->size() > 0 )
          {          
             if( cbc1->at(i1) )
               { 
                  std::vector<int> hits;
                  hits.clear();
                  
                  int istart = (i1-ws < 0) ? 0 : i1-ws;
                  int iend = (i1+ws >= n1) ? n1 : i1+ws+1;
                  for(int i2=istart;i2<iend;i2++) 
                    {
                       if( cbc2->size() > 0 )
                         {                   
                            if( cbc2->at(i2) ) hits.push_back(i2);
                         }                     
                    }    
                    
                  if( hits.size() > 0 )
                    {             
                       STUB *stub = new STUB(i1,hits);
                       stubs.push_back(stub);
                    }        
               }             
          }          
     }    
     
   return stubs;
}  

std::vector<CLUST*> DUTanalysis::getClusters(std::vector<int> *cbc,
                                int chb,
                                int che)
{
   std::vector<CLUST*> clusters;
   clusters.clear();

   //int n = cbc->size();

   bool ofr = 0;
   if( chb < 0 || chb > nMaxChannel ) ofr = 1;
   if( che < 0 || che > nMaxChannel ) ofr = 1;
   if( chb > che ) ofr = 1;
   if( ofr )
     {
        std::cout << "Channel range must be from 0 min to "<<nMaxChannel<<" max" << std::endl;
        exit(1);
     }

   for(int i=chb;i<che;i++)
     {
        if( cbc->size() > 0 )
          {
             if( cbc->at(i) )
               {
                  std::vector<int> hits;
                  hits.clear();
                  hits.push_back(i);

                  if( i < che-1 )
                    {
                       for(int j=i+1;j<che;j++)
                         {
                            if( cbc->at(j) ) hits.push_back(j);
                            else
                              {
                                 i += j-i;
                                 break;
                              }
                         }
                    }


                  float pos = 0;
                  int nh = hits.size();
                  for(int ih=0;ih<nh;ih++)
                    {
                       pos += hits.at(ih)/float(nh);
                    }

                  CLUST *cluster = new CLUST(pos,hits);
                  clusters.push_back(cluster);
               }
          }
     }

   return clusters;
}


std::vector<CSTUB*> DUTanalysis::getClusterStubs(std::vector<CLUST*> cbc1,
                                    std::vector<CLUST*> cbc2,
                                    int ws)
{
   std::vector<CSTUB*> cstubs;
   cstubs.clear();

   int ncl1 = cbc1.size();
   int ncl2 = cbc2.size();

   for(int i1=0;i1<ncl1;i1++)
     {
        float pos1 = cbc1.at(i1)->pos;

        for(int i2=0;i2<ncl2;i2++)
          {
             float pos2 = cbc2.at(i2)->pos;

             if( fabs(pos1-pos2) < ws )
               {
                  CSTUB *cstub = new CSTUB(*cbc1.at(i1),*cbc2.at(i2));
                  cstubs.push_back(cstub);
               }
          }
     }

   return cstubs;
}

void DUTanalysis::doDUThistos(ReadInputTrees* trees){

	   //if ((*trees).dut_hits0.size() != (*trees).dut_hits1.size()) continue;

	//Events with one track
	if ((*trees).xPos->size()==1) return;

	//Occupancy plots
           int nHits0=0; int nHits1=0;
           for(unsigned int is=0;is<(*trees).dut_hits0.size();is++){
             if( (*trees).dut_hits0[is] == 1 ) nHits0++;
	   }
	   (*trees).hitOccupancy_hit0->Fill(nHits0);

           for(unsigned int is=0;is<(*trees).dut_hits1.size();is++){
             if( (*trees).dut_hits1[is] == 1 ) nHits1++;
           }
           (*trees).hitOccupancy_hit1->Fill(nHits1);

	   if ((*trees).dut_hits0.size() == (*trees).dut_hits1.size()){
	     (*trees).stubOccupancy->Fill(stubs.size());
	     for (unsigned int i=0; i<stubs.size(); i++) (*trees).stubMap1D->Fill(stubs[i]->pos);
	   }

  return;
}

void DUTanalysis::doDUTanalysis(ReadInputTrees* trees){//TTree* tInputDUT, TTree** tOutput){
/*
        if ((*trees).BTcampaign == "JuneBT") {
	  nMaxChannel = 256; //2CBC
	  doHitShift = true; //Hits are inverted within CBC
	}
	if ((*trees).BTcampaign == "NovBT")  {
	  nMaxChannel = 2048; //16CBC
	  doHitShift = false;
	}
*/
	//Setup(trees);

        std::map<std::string,std::vector<int> >::iterator it0 = (*trees).dut_channel->find("det0");
        std::map<std::string,std::vector<int> >::iterator it1 = (*trees).dut_channel->find("det1");

        bool hasHits0 = 1;
        if( it0 == (*trees).dut_channel->end() ) hasHits0 = 0;

        bool hasHits1 = 1;
        if( it1 == (*trees).dut_channel->end() ) hasHits1 = 0;

        std::vector<int> s0_Raw;
        std::vector<int> s1_Raw;
        std::vector<int> s0_Shift;
        std::vector<int> s1_Shift;
        std::vector<int> s0;
        std::vector<int> s1;
        (*trees).dut_hits0.clear();
        (*trees).dut_hits1.clear();

        int nHits0 = 0;
        int nHits1 = 0;

	//cout << "A"<<endl;

        if( hasHits0 )
          {
             s0_Raw = it0->second;
             nHits0 = s0_Raw.size();
             for(int ih=0;ih<nHits0;ih++)
               {
                  int ipos = s0_Raw[ih];
		  if (ipos>ipos_max1) ipos_max1=ipos;
		  if (ipos<1016) {
		    (*trees).hitMap2D_hit0->Fill(ipos, 0);
		    (*trees).hitMap1D_hit0->Fill(ipos);
		  }
		  else (*trees).hitMap2D_hit0->Fill(ipos-1016, 1);
		  if (doHitShift){
                    if( s0_Raw[ih] < 127 ) ipos = 126-s0_Raw[ih];
                    else ipos = 127+(253-s0_Raw[ih]);
		  }
                  s0_Shift.push_back(ipos);
               }
             for(int j=0;j<nMaxChannel;j++)
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
                  if (ipos>ipos_max2) ipos_max2=ipos;
                  if (ipos<1024) {
		    (*trees).hitMap2D_hit1->Fill(ipos, 0);
		    (*trees).hitMap1D_hit1->Fill(ipos);
		  }
                  else (*trees).hitMap2D_hit1->Fill(ipos-1024, 1);
                  if (doHitShift){
                    if( s1_Raw[ih] < 127 ) ipos = 126-s1_Raw[ih];
                    else ipos = 127+(253-s1_Raw[ih]);
		  }
                  s1_Shift.push_back(ipos);
               }
             for(int j=0;j<nMaxChannel;j++)
               {
                  int hit = 0;
                  for(int ih=0;ih<nHits1;ih++)
                    {
                       if( s1_Shift[ih] == j ) hit = 1;
                    }
                  s1.push_back(hit);
               }
          }

        stubs = getStubs(&s0,&s1,7,0,nMaxChannel-1);
        //std::vector<STUB*> stubs_A = getStubs(&s0,&s1,wsize[iw],0,128);
        //std::vector<STUB*> stubs_B = getStubs(&s0,&s1,wsize[iw],128,255);

	//for (int is=0; is<stubs.size(); is++) cout << "Stub "<<is<<" pos="<< stubs[is]->pos<< " nhits="<<stubs[is]->hits.size()<<endl;

        std::vector<CLUST*> s0_clusters = getClusters(&s0,0,nMaxChannel);
        std::vector<CLUST*> s1_clusters = getClusters(&s1,0,nMaxChannel);

        (*trees).dut_hits0 = s0;
        (*trees).dut_hits1 = s1;
        (*trees).dut_event = (*trees).event;

        //cout << "dut_event="<<(*trees).dut_event<<endl;


}


#endif
