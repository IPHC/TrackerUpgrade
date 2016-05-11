
#include "Riostream.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"

#include <vector>
#include <map>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <cmath>

#include "ReadInputTrees.h"
#include "DUTanalysis.h"
#include "TrackAnalysis.h"

#ifdef __MAKECINT__

#pragma link C++ class std::vector<int>+;
#pragma link C++ class std::map<std::string,std::vector<int> >+;

#endif

int main(int argc, char *argv[])
{

  cout << "Running Track-matching analysis"<<endl;

   gSystem->Load("AutoDict_map_string_vector_int___cxx.so");

   ReadInputTrees trees;// = new ReadInputTrees();

   trees.dut_channel = new std::map<std::string,std::vector<int> >();

   std::string InputFileNameDUT = string(argv[1]);
   std::string InputFileNameEUDET = string(argv[2]);
   std::string OutputFileName = string(argv[3]);
   trees.BTcampaign = string(argv[7]);

   cout << "DUT input file: "<<InputFileNameDUT<<endl;
   cout << "Telescope input file: " << InputFileNameEUDET<<endl;
   cout << "Output file: "<<OutputFileName<<endl;
   cout << "From event: "<<argv[4]<< " to "<<argv[5]<<endl;
   cout << "Type of run: "<<argv[6]<<endl;
   cout << "BT campaign: "<<argv[7]<<endl;

   DUTanalysis dutanalysis;
   TrackAnalysis tkanalysis;

   int evMax = atoi(argv[5]);

   if (strcmp(argv[6],"--DUTRun")==0){
   
     trees.InitializeRun(InputFileNameDUT, "", OutputFileName);
     dutanalysis.Setup(&trees);

     if (evMax==-1 || evMax>trees.tInputDUT->GetEntries()) evMax = trees.tInputDUT->GetEntries();
     for (Long64_t iEvent=atoi(argv[4]); iEvent<atoi(argv[5]); iEvent++){

        //std::cout << "Event "<<iEvent<<endl;
        //if (iEvent % 1000 == 0) std::cout << "Event "<<iEvent<<endl;

        trees.tInputDUT->GetEntry(iEvent);
 
        dutanalysis.doDUTanalysis(&trees);//trees.tInputDUT, &(trees.tOutput));

        trees.tOutput->Fill();
     }

     trees.EndRun();
   }

   if (strcmp(argv[6],"--TrackMatchRun")==0){

     trees.InitializeRun(InputFileNameDUT, InputFileNameEUDET, OutputFileName);
     dutanalysis.Setup(&trees);
     tkanalysis.SetupGeometry(&trees);

     std::vector<int> DUTEVT;
     std::vector<int> EUEVT;
     std::vector<int> MatchedEvent_DUT;
     std::vector<int> MatchedEvent_TEL;

     //Select DUT event numbers
     if (evMax==-1 || evMax>trees.tInputDUT->GetEntries()) evMax = trees.tInputDUT->GetEntries();
     cout << "DUT nEventsMax="<<evMax<<" in a file with "<< trees.tInputDUT->GetEntries()<< " entries"<<endl;
     for (Long64_t iEvent=atoi(argv[4]); iEvent<evMax; iEvent++){
        trees.tInputDUT->GetEntry(iEvent);
	//cout << "iEvent="<<iEvent<<" dut_event="<<trees.event<<endl;
	DUTEVT.push_back(trees.event);
     }
     //Select telescope event numbers
     cout << "TEL nEventsMax="<<evMax<<" in a file with "<< trees.tInputEUDET->GetEntries()<< " entries"<<endl;
     if (evMax==-1 || evMax>trees.tInputEUDET->GetEntries()) evMax = trees.tInputEUDET->GetEntries();
     for (Long64_t iEvent=atoi(argv[4]); iEvent<evMax; iEvent++){
	trees.tInputEUDET->GetEntry(iEvent);
	EUEVT.push_back(trees.euEvt);
     }
     //Match DUT/telescope events with same event number
     int nMatched=0;
     unsigned int iTel = 0;
     unsigned int iDUT = 0;
     while (iDUT<DUTEVT.size() && iTel<EUEVT.size()){
       //cout << "iDUT="<<iDUT<<" DUTEVT="<< DUTEVT[iDUT]<<" iTel="<<iTel<<" EUEVT="<<EUEVT[iTel]<<endl;
       if (DUTEVT[iDUT]==EUEVT[iTel]){
	 MatchedEvent_DUT.push_back(iDUT);
	 MatchedEvent_TEL.push_back(iTel);
	 iDUT++; iTel++; nMatched++;
	 //cout << "Matched"<<endl;
       }
       else if (DUTEVT[iDUT]<EUEVT[iTel]) iDUT++;
       else if (DUTEVT[iDUT]>EUEVT[iTel]) iTel++;
     }
	cout << " nMatched="<< nMatched<<endl;

     //Running on matched events
     for (int iEvent=0; iEvent<nMatched; iEvent++){

       if (iEvent % 10000 == 0) cout << "Event "<<iEvent<<endl;

       trees.tInputDUT->GetEntry(MatchedEvent_DUT[iEvent]);
       trees.tInputEUDET->GetEntry(MatchedEvent_TEL[iEvent]);
       trees.tFitEUDET->GetEntry(MatchedEvent_TEL[iEvent]);

       //cout << "Ntracks="<< trees.xPos->size() << endl;
       if (trees.xPos->size()>=1){ //events with more than 1 track
	 dutanalysis.doDUTanalysis(&trees);         

	//cout << "Npoints="<<trees.xPosFit->size()<<endl;
	//for (int i=0; i<trees.xPosFit->size(); i++){
	//  cout << "i="<<i<<" sensorId="<<trees.sensorIdFit->at(i) <<" xPos="<<trees.xPosFit->at(i)<<" yPos="<<trees.yPosFit->at(i)<<" zPos="<<trees.zPosFit->at(i) <<endl;
	//}
	// cout << "Ntracks="<< trees.xPos->size() <<" Nstubs="<< dutanalysis.stubs.size()<< endl;
	 tkanalysis.doTelescopeAnalysis(&trees);
	 //if (trees.dut_hits0.size() != trees.dut_hits1.size()) continue;
	 tkanalysis.doTrackMatchingAnalysis(&trees, dutanalysis.stubs);

	 dutanalysis.doDUThistos(&trees);
       }
     }

     tkanalysis.ComputeChi2(&trees);

     trees.EndRun();
   }


  return 0;
}
