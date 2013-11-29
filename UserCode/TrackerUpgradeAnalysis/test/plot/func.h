#ifndef FUNC_H
#define FUNC_H

#include "var.h"

std::vector<int> getStubs(std::vector<int> *cbc1,std::vector<int> *cbc2)
{
   std::vector<int> stubs;
   
   int n1 = cbc1->size();
   
   for(int i1=0;i1<n1;i1++)
     {
	if( cbc1->at(i1) && cbc2->at(i1) ) stubs.push_back(i1);
     }   
   
   return stubs;
}

#endif
