#ifndef TRACKANALYSIS_H
#define TRACKANALYSIS_H

#include "TMath.h"

#include "ReadInputTrees.h"
#include "DUTanalysis.h"

class TrackAnalysis {

  public:

    TrackAnalysis();
    ~TrackAnalysis();

    bool duplicateOrigin;
    int factor;
    int origin;
    unsigned int idFeI4;

    double pitch;
 
    double FEI4_z; 
    double DUT_z;
    double dxCorr;

    double* zPosTelPlane;

    int nEvents1tk1stub;
    double chi2[20];

    void SetupGeometry(ReadInputTrees* );
    void ComputeChi2(ReadInputTrees* );
    void doTelescopeAnalysis(ReadInputTrees*);
    void doTrackMatchingAnalysis(ReadInputTrees*, std::vector<STUB*> );//TTree*, TTree**);
    double FindBestTrackMatchAtDUT(ReadInputTrees*, std::vector<STUB*>, double, double, int*, int*);
};


TrackAnalysis::TrackAnalysis(){

  idFeI4 = 4;

  duplicateOrigin = false;

  if (duplicateOrigin) {
    factor = 7;
    origin = idFeI4;
  }
  else {
    factor = 1;
    origin = 0;
  }

}

TrackAnalysis::~TrackAnalysis(){

}

void TrackAnalysis::SetupGeometry(ReadInputTrees* trees){

       pitch = 90.;
	zPosTelPlane = new double[7];

       if ((*trees).BTcampaign == "JuneBT") {
          FEI4_z = 244.0;
          DUT_z = 416.0;
	  dxCorr = 0;
        }
        if ((*trees).BTcampaign == "NovBT")  {
	  zPosTelPlane[0] = 0;
          zPosTelPlane[1] = 120;
          zPosTelPlane[2] = 119 + zPosTelPlane[1];
          zPosTelPlane[3] = 410 + zPosTelPlane[2];
          zPosTelPlane[4] = 35 + zPosTelPlane[3];
          zPosTelPlane[5] = 85 + zPosTelPlane[4];
          zPosTelPlane[6] = 120 + zPosTelPlane[5];

          FEI4_z =684.0;
          DUT_z = 1380.;//1250.0; //to be calibrated
          dxCorr = -12.9528;//-12.95;

        }

	nEvents1tk1stub = 0;
	for (int i=0; i<20; i++) chi2[i] = 0;

	(*trees).diffX_1tk_1stub_stubPos_TkAtDUT = new TH1F("diffX_1tk_1stub_stubPos_TkAtDUT", "diffX_1tk_1stub_stubPos_TkAtDUT", 10000, -50, 50);
        (*trees).diffX_1tk_gt1StubBest_stubPos_TkAtDUT = new TH1F("diffX_1tk_gt1StubBest_stubPos_TkAtDUT", "diffX_1tk_gt1StubBest_stubPos_TkAtDUT", 10000, -50, 50);
        (*trees).diffX_gt1tk_1Stub_stubPos_TkAtDUT = new TH1F("diffX_gt1tk_1Stub_stubPos_TkAtDUT","diffX_gt1tk_1Stub_stubPos_TkAtDUT",10000, -50, 50);
	(*trees).diffX_gt1tk_gt1StubBest_stubPos_TkAtDUT = new TH1F("diffX_gt1tk_gt1StubBest_stubPos_TkAtDUT","diffX_gt1tk_gt1StubBest_stubPos_TkAtDUT", 10000, -50, 50);
	(*trees).diffX_gt1tk_gt1StubBest2nd_stubPos_TkAtDUT = new TH1F("diffX_gt1tk_gt1StubBest2nd_stubPos_TkAtDUT","diffX_gt1tk_gt1StubBest2nd_stubPos_TkAtDUT",10000, -50, 50);

	for (int i=0; i<20; i++){
	  (*trees).diffX_1tk_1stub_stubPos_TkAtDUT_bestMatch_dZScan[i] = new TH1F(Form("diffX_1tk_1stub_stubPos_TkAtDUT_bestMatch_dZScan_%i",i),Form("diffX_1tk_1stub_stubPos_TkAtDUT_bestMatch_dZScan_%i",i),10000, -50, 50);
	  (*trees).diffX_gt1tk_1stub_stubPos_TkAtDUT_bestMatch_dZscan[i] = new TH1F(Form("diffX_gt1tk_1stub_stubPos_TkAtDUT_bestMatch_dZscan_%i",i),Form("diffX_gt1tk_1stub_stubPos_TkAtDUT_bestMatch_dZscan_%i",i),10000, -50, 50);
          (*trees).diffX_gt1tk_gt1stub_stubPos_TkAtDUT_bestMatch_dZscan[i] = new TH1F(Form("diffX_gt1tk_gt1stub_stubPos_TkAtDUT_bestMatch_dZscan_%i",i),Form("diffX_gt1tk_gt1stub_stubPos_TkAtDUT_bestMatch_dZscan_%i",i),10000, -50, 50);
	}

	(*trees).deltaZ_1tk_1stub_stubPos_TkOrigin = new TH1F("deltaZ_1tk_1stub_stubPos_TkOrigin","deltaZ_1tk_1stub_stubPos_TkOrigin",30,0,3000);
        (*trees).deltaZ_gt1tk_1stub_stubPos_TkOrigin = new TH1F("deltaZ_gt1tk_1stub_stubPos_TkOrigin","deltaZ_gt1tk_1stub_stubPos_TkOrigin",30,0,3000);

	for (int i=0; i<20; i++){
	  (*trees).deltaZ_1tk_1stub_stubPos_TkOrigin_offsetScan[i] = new TH1F(Form("deltaZ_1tk_1stub_stubPos_TkOrigin_offsetScan_%i",i),Form("deltaZ_1tk_1stub_stubPos_TkOrigin_offsetScan_%i",i),100,-10000,10000);
          (*trees).deltaZ_gt1tk_1stub_stubPos_TkOrigin_offsetScan[i] = new TH1F(Form("deltaZ_gt1tk_1stub_stubPos_TkOrigin_offsetScan_%i",i),Form("deltaZ_gt1tk_1stub_stubPos_TkOrigin_offsetScan_%i",i),100,-10000,10000);
	}

	(*trees).chi2_1tk_1stub_vsZ = new TH1F("chi2_1tk_1stub_vsZ","chi2_1tk_1stub_vsZ",20,1200-10,1600-10);

	(*trees).beamprofile2D_TkAtDUT = new TH2F("beamprofile2D_TkAtDUT","beamprofile2D_TkAtDUT",1016,-508*pitch/1000,508*pitch/1000,1000,-100,100);
	(*trees).beamprofile1D_X_TkAtDUT = new TH1F("beamprofile1D_X_TkAtDUT","beamprofile1D_X_TkAtDUT",1016,-508*pitch/1000,508*pitch/1000);

	(*trees).beamprofile2D_1tk_TkAtDUT = new TH2F("beamprofile2D_1tk_TkAtDUT","beamprofile2D_1tk_TkAtDUT",1016,-508*pitch/1000,508*pitch/1000,1000,-100,100);
	(*trees).beamprofile1D_X_1tk_TkAtDUT = new TH1F("beamprofile1D_X_1tk_TkAtDUT","beamprofile1D_X_1tk_TkAtDUT",1016,-508*pitch/1000,508*pitch/1000);


     int nBinTel = 10000;
     double xminTel = -2;
     double xmaxTel = 2;
     (*trees).telescopeResidualsX_p0 = new TH1F("telescopeResidualsX_p0", "telescopeResidualsX_p0", nBinTel, xminTel, xmaxTel);
     (*trees).telescopeResidualsY_p0 = new TH1F("telescopeResidualsY_p0","telescopeResidualsY_p0", nBinTel, xminTel, xmaxTel);
     (*trees).telescopeResidualsX_p1 = new TH1F("telescopeResidualsX_p1","telescopeResidualsX_p1", nBinTel, xminTel, xmaxTel);
     (*trees).telescopeResidualsY_p1 = new TH1F("telescopeResidualsY_p1","telescopeResidualsY_p1", nBinTel, xminTel, xmaxTel);
     (*trees).telescopeResidualsX_p2 = new TH1F("telescopeResidualsX_p2","telescopeResidualsX_p2", nBinTel, xminTel, xmaxTel);
     (*trees).telescopeResidualsY_p2 = new TH1F("telescopeResidualsY_p2","telescopeResidualsY_p2", nBinTel, xminTel, xmaxTel);
     (*trees).telescopeResidualsX_p3 = new TH1F("telescopeResidualsX_p3","telescopeResidualsX_p3", nBinTel, xminTel, xmaxTel);
     (*trees).telescopeResidualsY_p3 = new TH1F("telescopeResidualsY_p3","telescopeResidualsY_p3", nBinTel, xminTel, xmaxTel);
     (*trees).telescopeResidualsX_p20 = new TH1F("telescopeResidualsX_p20","telescopeResidualsX_p20", nBinTel, xminTel, xmaxTel);
     (*trees).telescopeResidualsY_p20 = new TH1F("telescopeResidualsY_p20","telescopeResidualsY_p20", nBinTel, xminTel, xmaxTel); 
     (*trees).telescopeResidualsX_p4 = new TH1F("telescopeResidualsX_p4","telescopeResidualsX_p4", nBinTel, xminTel, xmaxTel);
     (*trees).telescopeResidualsY_p4 = new TH1F("telescopeResidualsY_p4","telescopeResidualsY_p4", nBinTel, xminTel, xmaxTel);
     (*trees).telescopeResidualsX_p5 = new TH1F("telescopeResidualsX_p5","telescopeResidualsX_p5", nBinTel, xminTel, xmaxTel);
     (*trees).telescopeResidualsY_p5 = new TH1F("telescopeResidualsY_p5","telescopeResidualsY_p5", nBinTel, xminTel, xmaxTel); 

     nBinTel = 100000;
     xminTel = -10;
     xmaxTel = 10;
     (*trees).telescopeX_p0 = new TH1F("telescopeX_p0","telescopeX_p0",nBinTel, xminTel, xmaxTel);
     (*trees).telescopeY_p0 = new TH1F("telescopeY_p0","telescopeY_p0",nBinTel, xminTel, xmaxTel);

  return;
}

void TrackAnalysis::ComputeChi2(ReadInputTrees*  trees){

  for (int i=0; i<20; i++){
    cout << "i="<<i<<" chi2="<< chi2[i]/((double)nEvents1tk1stub) <<endl;
    (*trees).chi2_1tk_1stub_vsZ->SetBinContent(i+1, chi2[i]/((double)nEvents1tk1stub)); 
  }

}

double TrackAnalysis::FindBestTrackMatchAtDUT(ReadInputTrees* trees, std::vector<STUB*> stubs, double dzDUT, double offset, int* iDUTmatched, int* iTelMatched){

	int Ntracks = (*trees).xPos->size() / factor;
        double* xTk = new double[Ntracks];
        double* yTk = new double[Ntracks];

        double zOrigin = zPosTelPlane[idFeI4];

        for (int i=0; i<Ntracks; i++){
          xTk[i] = (dzDUT-zOrigin) * (*trees).dxdz->at(i*factor+origin) + (*trees).xPos->at(i*factor+origin) - offset;
          yTk[i] = (dzDUT-zOrigin) * (*trees).dydz->at(i*factor+origin) + (*trees).yPos->at(i*factor+origin);
        }

        double* xDUT = new double[stubs.size()];

          double bestdiffX=10000;
          int i1=-1, j1=-1;
          for (unsigned int i=0; i<stubs.size(); i++){
            xDUT[i] = ((1016-stubs[i]->pos)*pitch)/1000.;//((stubs[i]->pos-508.)*pitch)/1000.;
            for (int j=0; j<Ntracks; j++){
              if (fabs(xDUT[i]-xTk[j])<bestdiffX) {
                bestdiffX=fabs(xDUT[i]-xTk[j]);
                i1=i; j1=j;
              }
            }
          }

	 *iDUTmatched = i1;
	 *iTelMatched = j1;

	 return (xDUT[i1]-xTk[j1]);
}

void TrackAnalysis::doTelescopeAnalysis(ReadInputTrees* trees){

  //Telescope residuals
  if (duplicateOrigin==false) return;

  //cout << "Ntracks="<<(*trees).xPos->size()<<endl;
  if ((*trees).xPos->size()!=7) return; //events with only one track 

  double xTk[7];
  double yTk[7];

  double* xTelPlane = new double[(*trees).xPosFit->size()];
  double* yTelPlane = new double[(*trees).xPosFit->size()];
  //double* zTelPlane = new double[(*trees).xPosFit->size()];

/*
  cout << "Npoints="<<(*trees).xPosFit->size()<<endl;
  for (int i=0; i<(*trees).xPosFit->size(); i++){
    cout << "i="<<i<<" sensorId="<<(*trees).sensorIdFit->at(i) <<" xPos="<<(*trees).xPosFit->at(i)<<" yPos="<<(*trees).yPosFit->at(i)<<" zPos="<<(*trees).zPosFit->at(i) <<endl;
    xTelPlane[i] = (*trees).xPosFit->at(i) - 10;
    yTelPlane[i] = (*trees).yPosFit->at(i) - 10;
    zTelPlane[i] = (*trees).zPosFit->at(i);
  }
*/

  double dist = 0;
  double dist_max = 0;
  int selectedHit[7];
  //double zOrigin = zPosTelPlane[0];//FEI4_z;

  (*trees).telescopeX_p0->Fill((*trees).xPos->at(0));
  (*trees).telescopeY_p0->Fill((*trees).yPos->at(0));

  for (unsigned int i=0; i<(*trees).xPos->size(); i++){ 
    if (i==idFeI4) continue;
    //xTk[i] = (zPosTelPlane[i] - zOrigin) * (*trees).dxdz->at(0) + (*trees).xPos->at(0);
    //yTk[i] = (zPosTelPlane[i] - zOrigin) * (*trees).dydz->at(0) + (*trees).yPos->at(0);
    xTk[i] = (*trees).xPos->at(i);
    yTk[i] = (*trees).yPos->at(i);
    //cout << "Fitpoint i="<<i<<" xPos="<<(*trees).xPos->at(i)<<" yPos="<<(*trees).yPos->at(i)<<" zPos="<<(*trees).zPos->at(i)<<endl;
    //selectedHit[i] = i;
    //xTelPlane[i] = (zPosTelPlane[i] - zOrigin) * (*trees).dxdz->at(0) + (*trees).xPos->at(0) ;
    //yTelPlane[i] = (zPosTelPlane[i] - zOrigin) * (*trees).dydz->at(0) + (*trees).yPos->at(0) ;
    //cout << "i="<<i<<" sensorId="<<(*trees).sensorId->at(i) <<" xFitPoint="<<xTk[i]<<" xTkAtPlane="<<xTelPlane[i]<<endl;
 
    dist_max = 100000;    
    //cout << "Npoints="<<(*trees).xPosFit->size()<<endl;
    for (unsigned int j=0; j<(*trees).xPosFit->size(); j++){
      //cout << "i="<<i<<" sensorId="<<(*trees).sensorIdFit->at(j) <<" xPos="<<(*trees).xPosFit->at(j)<<" yPos="<<(*trees).yPosFit->at(j)<<" zPos="<<(*trees).zPosFit->at(j) <<endl;
      if ((*trees).sensorId->at(i) == (*trees).sensorIdFit->at(j)){
        xTelPlane[j] = (*trees).xPosFit->at(j) - 10.6;
        yTelPlane[j] = (*trees).yPosFit->at(j) - 10.6/2;
        dist = sqrt((xTk[i]-xTelPlane[j])*(xTk[i]-xTelPlane[j])+(yTk[i]-yTelPlane[j])*(yTk[i]-yTelPlane[j]));
        if (dist < dist_max) { 
	  dist_max = dist;
	  selectedHit[i] = j;
        } 
      }
    }

  }

  //for (int i=0; i<(*trees).xPos->size(); i++) cout << "i="<<i<<" selectedHit="<<selectedHit[i]<<endl;


  (*trees).telescopeResidualsX_p0->Fill(xTk[0]-xTelPlane[selectedHit[0]]);
  (*trees).telescopeResidualsY_p0->Fill(yTk[0]-yTelPlane[selectedHit[0]]);

  (*trees).telescopeResidualsX_p1->Fill(xTk[1]-xTelPlane[selectedHit[1]]);
  (*trees).telescopeResidualsY_p1->Fill(yTk[1]-yTelPlane[selectedHit[1]]);

  (*trees).telescopeResidualsX_p2->Fill(xTk[2]-xTelPlane[selectedHit[2]]);
  (*trees).telescopeResidualsY_p2->Fill(yTk[2]-yTelPlane[selectedHit[2]]);

  (*trees).telescopeResidualsX_p3->Fill(xTk[3]-xTelPlane[selectedHit[3]]);
  (*trees).telescopeResidualsY_p3->Fill(yTk[3]-yTelPlane[selectedHit[3]]);

  //(*trees).telescopeResidualsX_p20->Fill(xTk[4]-xTelPlane[selectedHit[4]]); //X and Y inverted (90 deg rotation FeI4)
  //(*trees).telescopeResidualsY_p20->Fill(yTk[4]-yTelPlane[selectedHit[4]]);

  (*trees).telescopeResidualsX_p4->Fill(xTk[5]-xTelPlane[selectedHit[5]]);
  (*trees).telescopeResidualsY_p4->Fill(yTk[5]-yTelPlane[selectedHit[5]]);

  (*trees).telescopeResidualsX_p5->Fill(xTk[6]-xTelPlane[selectedHit[6]]);
  (*trees).telescopeResidualsY_p5->Fill(yTk[6]-yTelPlane[selectedHit[6]]);

  return;
}

void TrackAnalysis::doTrackMatchingAnalysis(ReadInputTrees* trees, std::vector<STUB*> stubs){

	//cout << "doTrackMatchingAnalysis size="<<(*trees).dxdz->size() << endl;
	//if ((*trees).xPos->size()!=1) return; //Ntracks==1

	int Ntracks = (*trees).xPos->size() / factor;
        double dzDUT = DUT_z;//(DUT_z-FEI4_z); //Are we sure the track origin is set to the FeI4 plane ?
	double zOrigin = zPosTelPlane[0];

	double* xTk = new double[Ntracks];
        double* yTk = new double[Ntracks];

	for (int i=0; i<Ntracks; i++){
          xTk[i] = (dzDUT-zOrigin) * (*trees).dxdz->at(i*factor+origin) + (*trees).xPos->at(i*factor+origin) - dxCorr;
          yTk[i] = (dzDUT-zOrigin) * (*trees).dydz->at(i*factor+origin) + (*trees).yPos->at(i*factor+origin);
         (*trees).beamprofile2D_TkAtDUT->Fill(xTk[i],yTk[i]);
         (*trees).beamprofile1D_X_TkAtDUT->Fill(xTk[i]);
	}

	if (Ntracks==1){
	  (*trees).beamprofile2D_1tk_TkAtDUT->Fill(xTk[0],yTk[0]);
	  (*trees).beamprofile1D_X_1tk_TkAtDUT->Fill(xTk[0]);

          //int ixTk = std::abs(508.+floor(xTk[0]*1000./pitch));
	}

	if ((*trees).dut_hits0.size() != (*trees).dut_hits1.size()) return;
	double* xDUT = new double[stubs.size()];

        //for (int is=0; is<stubs.size(); is++) cout << "Stub "<<is<<" pos="<< stubs[is]->pos<< " nhits="<<stubs[is]->hits.size()<<endl;

	if (stubs.size()>=1){
	  if (Ntracks==1 && stubs.size()==1) nEvents1tk1stub++;

	  double bestdiffX=10000;
	  double bestdiffX_2nd = 10000;
	  int i1=-1, j1=-1;
	  int i2=-1, j2=-1;
	  for (unsigned int i=0; i<stubs.size(); i++){
	    xDUT[i] = ((1016-stubs[i]->pos)*pitch)/1000.;//((stubs[i]->pos-508.)*pitch)/1000.;
	    for (int j=0; j<Ntracks; j++){
	      if (fabs(xDUT[i]-xTk[j])<bestdiffX) {
		bestdiffX=fabs(xDUT[i]-xTk[j]);
		i1=i; j1=j;
	      }
	    }
	  }
          for (unsigned int i=0; i<stubs.size(); i++){
            for (int j=0; j<Ntracks; j++){
	      if ((fabs(xDUT[i]-xTk[j])<bestdiffX_2nd) && (i!=i1 && j!=j1)){
		bestdiffX_2nd = fabs(xDUT[i]-xTk[j]);
		i2=i; j2=j;
	      }
	    }
 	  }

	  //cout << "Tk extrapol at DUT: xTk="<<xTk[0]<< " xDUT="<< xDUT[0]<<" ixDUT="<< stubs[0]->pos <<endl; 

	  if (Ntracks==1 && stubs.size()==1) (*trees).diffX_1tk_1stub_stubPos_TkAtDUT->Fill(xDUT[0]-xTk[0]);
	  if (Ntracks==1 && stubs.size()>1) (*trees).diffX_1tk_gt1StubBest_stubPos_TkAtDUT->Fill(xDUT[i1]-xTk[j1]);
	  if (Ntracks>1 && stubs.size()==1) (*trees).diffX_gt1tk_1Stub_stubPos_TkAtDUT->Fill(xDUT[i1]-xTk[j1]);
	  if (Ntracks>1 && stubs.size()>1) (*trees).diffX_gt1tk_gt1StubBest_stubPos_TkAtDUT->Fill(xDUT[i1]-xTk[j1]);
	  if (Ntracks>1 && stubs.size()>1)   (*trees).diffX_gt1tk_gt1StubBest2nd_stubPos_TkAtDUT->Fill(xDUT[i2]-xTk[j2]); 

	  double deltaZ;
	  if (duplicateOrigin) deltaZ = (xDUT[0]-(*trees).xPos->at(idFeI4)+dxCorr)/(*trees).dxdz->at(idFeI4);
	  else deltaZ = (xDUT[0]-(*trees).xPos->at(0)+dxCorr)/(*trees).dxdz->at(0);
	  //cout << "deltaZ="<<deltaZ<<endl;
	  if (Ntracks==1 && stubs.size()==1) (*trees).deltaZ_1tk_1stub_stubPos_TkOrigin->Fill(deltaZ);
	  deltaZ = (xDUT[0]-(*trees).xPos->at(j1)+dxCorr)/(*trees).dxdz->at(j1);
          if (Ntracks>=1 && stubs.size()==1) (*trees).deltaZ_gt1tk_1stub_stubPos_TkOrigin->Fill(deltaZ);

	  int iDUT_matched=-1, iTel_matched=-1;
	  double dzDUTscan = 0;
	  double diffX_matched = 900;
	  double offset = 0;
	  double sigma = 0;

	    for (int iz=0; iz<20; iz++){
	      dzDUTscan = 1000 + ((double)iz)*50;
	      diffX_matched = FindBestTrackMatchAtDUT(trees, stubs, dzDUTscan, 0, &iDUT_matched, &iTel_matched);
	      if (stubs.size()==1 && Ntracks==1) (*trees).diffX_1tk_1stub_stubPos_TkAtDUT_bestMatch_dZScan[iz]->Fill(diffX_matched);
              if (stubs.size()==1 && Ntracks>=1) (*trees).diffX_gt1tk_1stub_stubPos_TkAtDUT_bestMatch_dZscan[iz]->Fill(diffX_matched);
              if (stubs.size()>=1 && Ntracks>=1) (*trees).diffX_gt1tk_gt1stub_stubPos_TkAtDUT_bestMatch_dZscan[iz]->Fill(diffX_matched);
	    }

	    for (int ix=0; ix<20; ix++){
	      offset = 12.95 + ((double)ix)*0.0025;
	      deltaZ = FEI4_z + (xDUT[0]-(*trees).xPos->at(0)-offset)/(*trees).dxdz->at(0);
              if (Ntracks==1 && stubs.size()==1) (*trees).deltaZ_1tk_1stub_stubPos_TkOrigin_offsetScan[ix]->Fill(deltaZ);
	      deltaZ = (xDUT[0]-(*trees).xPos->at(j1)-offset)/(*trees).dxdz->at(j1);
              if (Ntracks>=1 && stubs.size()==1) (*trees).deltaZ_gt1tk_1stub_stubPos_TkOrigin_offsetScan[ix]->Fill(deltaZ);
	    }

	    for (int iz=0; iz<20; iz++){
	      dzDUTscan = 1200 + ((double)iz)*20;
	      if (Ntracks==1 && stubs.size()==1) {
		offset = -(-4.31565e-05*dzDUTscan + 1.30131e+01);
	 	sigma = sqrt(0.090*0.090/12. + 0.0085*0.0085);
                diffX_matched = FindBestTrackMatchAtDUT(trees, stubs, dzDUTscan, offset, &iDUT_matched, &iTel_matched);
		//cout << "dzDUTscan="<<dzDUTscan<< " offset="<<offset<<" sigma="<<sigma<<" diffX_matched="<<diffX_matched<<" dChi2="<< (diffX_matched/sigma) * (diffX_matched/sigma) <<endl;
		//if (fabs(diffX_matched) < 5*sigma) chi2[iz] += (diffX_matched) * (diffX_matched);
		if (fabs(diffX_matched) < 5*sigma) chi2[iz] += (diffX_matched/sigma) * (diffX_matched/sigma);
	      }
	    }

	}


  return;
}

#endif
