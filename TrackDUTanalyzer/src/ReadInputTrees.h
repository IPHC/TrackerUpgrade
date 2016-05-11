
#ifndef READINPUTTREES_H
#define READINPUTTREES_H

//#include "DUTanalysis.h"

class ReadInputTrees {

  public:

  string BTcampaign;

  TFile* fInputDUT;
  TTree* tInputDUT;
  std::map<std::string,std::vector<int> > *dut_channel;
  UInt_t stubWord;
  unsigned int event;

  TFile* fInputEUDET;
  TTree* tInputEUDET;
  std::vector<double> *xPos;
  std::vector<double> *yPos;
  std::vector<double> *dxdz;
  std::vector<double> *dydz;
  std::vector<double> *chi2;
  std::vector<double> *sensorId;
  int euEvt;
  TTree* tFitEUDET;
  std::vector<double> *xPosFit;
  std::vector<double> *yPosFit;
  std::vector<double> *zPosFit;
  std::vector<double> *sensorIdFit;

  TFile* fOutput;
  TTree* tOutput;
  int dut_event;
  std::vector<int> dut_hits0;
  std::vector<int> dut_hits1;

  //std::vector<STUB*> stubs;

  TH2F* hitMap2D_hit0;
  TH2F* hitMap2D_hit1;
  TH1F* hitMap1D_hit0;
  TH1F* hitMap1D_hit1;
  TH1F* stubMap1D;

  TH1F* hitOccupancy_hit0;
  TH1F* hitOccupancy_hit1;
  TH1F* stubOccupancy;

  TH2F* beamprofile2D_TkAtDUT;
  TH1F* beamprofile1D_X_TkAtDUT;
  TH2F* beamprofile2D_1tk_TkAtDUT;
  TH1F* beamprofile1D_X_1tk_TkAtDUT;

  TH1F* diffX_1tk_1stub_stubPos_TkAtDUT;
  TH1F* diffX_1tk_gt1StubBest_stubPos_TkAtDUT;
  TH1F* diffX_gt1tk_1Stub_stubPos_TkAtDUT;
  TH1F* diffX_gt1tk_gt1StubBest_stubPos_TkAtDUT;
  TH1F* diffX_gt1tk_gt1StubBest2nd_stubPos_TkAtDUT;

  TH1F* diffX_1tk_1stub_stubPos_TkAtDUT_bestMatch_dZScan[20];
  TH1F* diffX_gt1tk_1stub_stubPos_TkAtDUT_bestMatch_dZscan[20];
  TH1F* diffX_gt1tk_gt1stub_stubPos_TkAtDUT_bestMatch_dZscan[20];

  TH1F* deltaZ_1tk_1stub_stubPos_TkOrigin;
  TH1F* deltaZ_gt1tk_1stub_stubPos_TkOrigin;

  TH1F* deltaZ_1tk_1stub_stubPos_TkOrigin_offsetScan[20];
  TH1F* deltaZ_gt1tk_1stub_stubPos_TkOrigin_offsetScan[20];

  TH1F* chi2_1tk_1stub_vsZ;

  TH1F* telescopeResidualsX_p0;
  TH1F* telescopeResidualsY_p0;
  TH1F* telescopeResidualsX_p1;
  TH1F* telescopeResidualsY_p1;
  TH1F* telescopeResidualsX_p2;
  TH1F* telescopeResidualsY_p2;
  TH1F* telescopeResidualsX_p3;
  TH1F* telescopeResidualsY_p3;
  TH1F* telescopeResidualsX_p20;
  TH1F* telescopeResidualsY_p20;
  TH1F* telescopeResidualsX_p4;
  TH1F* telescopeResidualsY_p4;
  TH1F* telescopeResidualsX_p5;
  TH1F* telescopeResidualsY_p5;

  TH1F* telescopeX_p0;
  TH1F* telescopeY_p0;

  void InitializeRun(string, string, string);
  void EndRun();
};

void ReadInputTrees::InitializeRun(string InputFileNameDUT, string InputFileNameEUDET, string OutputFileName){

  fInputDUT = TFile::Open(InputFileNameDUT.c_str(),"READ");
  fInputDUT->ls();
  tInputDUT = (TTree*) fInputDUT->Get("treeMaker/tbeamTree");
  tInputDUT->SetBranchAddress("dut_channel",&dut_channel);
  tInputDUT->SetBranchAddress("stubWord",&stubWord);
  tInputDUT->SetBranchAddress("event",&event);

  xPos = new std::vector<double>;
  yPos = new std::vector<double>;
  dxdz = new std::vector<double>;
  dydz = new std::vector<double>;
  chi2 = new std::vector<double>;
  sensorId = new std::vector<double>;

  xPosFit = new std::vector<double>;
  yPosFit = new std::vector<double>;
  zPosFit = new std::vector<double>;
  sensorIdFit = new std::vector<double>;

  if (InputFileNameEUDET!=""){
    fInputEUDET = TFile::Open(InputFileNameEUDET.c_str(), "READ");
    fInputEUDET->ls();

    tInputEUDET = (TTree*) fInputEUDET->Get("tracks");
    tInputEUDET->SetBranchAddress("xPos",&xPos);
    tInputEUDET->SetBranchAddress("yPos",&yPos);
    tInputEUDET->SetBranchAddress("dxdz",&dxdz);
    tInputEUDET->SetBranchAddress("dydz",&dydz);
    tInputEUDET->SetBranchAddress("chi2",&chi2);
    tInputEUDET->SetBranchAddress("euEvt",&euEvt);
    tInputEUDET->SetBranchAddress("iden",&sensorId);

    tFitEUDET = (TTree*) fInputEUDET->Get("fitpoints");
    tFitEUDET->SetBranchAddress("xPos",&xPosFit);
    tFitEUDET->SetBranchAddress("yPos",&yPosFit);
    tFitEUDET->SetBranchAddress("zPos",&zPosFit);
    tFitEUDET->SetBranchAddress("sensorId",&sensorIdFit);

  }

  fOutput = new TFile(OutputFileName.c_str(),"RECREATE");
  tOutput = new TTree("Tree","Tree");
  tOutput->Branch("dut_event",&dut_event,"dut_event/I");
  tOutput->Branch("dut_hits0",&dut_hits0);
  tOutput->Branch("dut_hits1",&dut_hits1);

  fOutput->ls();

  return;
}

void ReadInputTrees::EndRun(){

     fOutput->cd();

     hitMap2D_hit0->Write();
     hitMap2D_hit1->Write();
     hitMap1D_hit0->Write();
     hitMap1D_hit1->Write();
     stubMap1D->Write();
     hitOccupancy_hit0->Write();
     hitOccupancy_hit1->Write();
     stubOccupancy->Write();

     diffX_1tk_1stub_stubPos_TkAtDUT->Write();
     diffX_1tk_gt1StubBest_stubPos_TkAtDUT->Write();
     diffX_gt1tk_1Stub_stubPos_TkAtDUT->Write();
     diffX_gt1tk_gt1StubBest_stubPos_TkAtDUT->Write();
     diffX_gt1tk_gt1StubBest2nd_stubPos_TkAtDUT->Write();

     for (int i=0; i<20; i++){
	diffX_1tk_1stub_stubPos_TkAtDUT_bestMatch_dZScan[i]->Write();
	diffX_gt1tk_1stub_stubPos_TkAtDUT_bestMatch_dZscan[i]->Write();
	diffX_gt1tk_gt1stub_stubPos_TkAtDUT_bestMatch_dZscan[i]->Write();
     }

     deltaZ_1tk_1stub_stubPos_TkOrigin->Write();
     deltaZ_gt1tk_1stub_stubPos_TkOrigin->Write();

     for (int i=0; i<20; i++){
       deltaZ_1tk_1stub_stubPos_TkOrigin_offsetScan[i]->Write();
       deltaZ_gt1tk_1stub_stubPos_TkOrigin_offsetScan[i]->Write();
     }

     chi2_1tk_1stub_vsZ->Write();

     beamprofile2D_TkAtDUT->Write();
     beamprofile1D_X_TkAtDUT->Write();
     beamprofile2D_1tk_TkAtDUT->Write();
     beamprofile1D_X_1tk_TkAtDUT->Write();

     telescopeResidualsX_p0->Write();
     telescopeResidualsY_p0->Write();
     telescopeResidualsX_p1->Write();
     telescopeResidualsY_p1->Write();
     telescopeResidualsX_p2->Write();
     telescopeResidualsY_p2->Write();
     telescopeResidualsX_p3->Write();
     telescopeResidualsY_p3->Write();
     telescopeResidualsX_p20->Write();
     telescopeResidualsY_p20->Write(); 
     telescopeResidualsX_p4->Write();
     telescopeResidualsY_p4->Write();
     telescopeResidualsX_p5->Write();
     telescopeResidualsY_p5->Write();

     telescopeX_p0->Write();
     telescopeY_p0->Write();

     tOutput->Write();
     fOutput->Close();
     fOutput->ls();

  return;
}

#endif
