#ifndef FUNC_H
#define FUNC_H

#include "histVar.h"

std::vector<STUB*> getStubs(std::vector<int> *cbc1,
			    std::vector<int> *cbc2,
			    int ws,
			    int chb,
			    int che)
{
   std::vector<STUB*> stubs;
   stubs.clear();
   
   int n1 = cbc1->size();   

   bool ofr = 0;
   if( chb < 0 || chb > 256 ) ofr = 1;
   if( che < 0 || che > 256 ) ofr = 1;
   if( chb > che ) ofr = 1;
   if( ofr )
     {
	std::cout << "Channel range must be from 0 min to 256 max" << std::endl;
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

std::vector<CLUST*> getClusters(std::vector<int> *cbc,
				int chb,
				int che)
{
   std::vector<CLUST*> clusters;
   clusters.clear();
   
   int n = cbc->size();
   
   bool ofr = 0;
   if( chb < 0 || chb > 256 ) ofr = 1;
   if( che < 0 || che > 256 ) ofr = 1;
   if( chb > che ) ofr = 1;
   if( ofr )
     {
	std::cout << "Channel range must be from 0 min to 256 max" << std::endl;
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

std::vector<CSTUB*> getClusterStubs(std::vector<CLUST*> cbc1,
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

#endif
