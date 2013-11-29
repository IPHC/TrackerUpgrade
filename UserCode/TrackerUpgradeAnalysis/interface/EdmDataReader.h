#ifndef TRACKERUPGRADEANALYSIS_EDMDATAREADER
#define TRACKERUPGRADEANALYSIS_EDMDATAREADER

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSet.h"
#include "DataFormats/SiPixelDigi/interface/PixelDigi.h"
#include "DataFormats/SiPixelDigi/interface/PixelDigiCollection.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/Provenance/interface/Timestamp.h"
#include "CondFormats/Common/interface/TimeConversions.h"

#include "Phase2TrackerDAQ/SiStripDigi/interface/SiStripCommissioningDigi.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include <vector>
#include <memory>
#include <string>
#include <iostream>

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "TTree.h"
#include "TSystem.h"
#include "TMath.h"

class EdmDataReader : public edm::EDAnalyzer
{
 public:
   
   explicit EdmDataReader( const edm::ParameterSet& );
   virtual ~EdmDataReader() {}
   
   virtual void beginJob() ;
   void run(const edm::DetSetVector<PixelDigi>&,
	    const edm::DetSet<SiStripCommissioningDigi>&);
   virtual void analyze(const edm::Event&, const edm::EventSetup&);
   virtual void endJob() ;
   
 private:
   
   edm::ParameterSet pset_;
   
   TTree *fTree;

   std::vector<int> *dutt;
   std::vector<int> *dutb;
   std::vector<int> *fixt;
   std::vector<int> *fixb;
   
   ULong64_t timestamp;
   int angle;
   
   const int NCHANMAX = 256;
   
};

DEFINE_FWK_MODULE(EdmDataReader);

#endif
