#include "UserCode/TrackerUpgradeAnalysis/interface/EdmDataReader.h"
#include <time.h>
#include <ctime>

EdmDataReader::EdmDataReader( const edm::ParameterSet& pset )
:
pset_(pset)
{
}

void EdmDataReader::beginJob( )
{
   dutt = new std::vector<int>;
   dutb = new std::vector<int>;
   fixt = new std::vector<int>;
   fixb = new std::vector<int>;
   
   edm::Service<TFileService> fs;
   
   fTree  = fs->make<TTree>("DATA","CBC data");
   fTree->Branch("DUTT","std::vector<int>",(dutt));
   fTree->Branch("DUTB","std::vector<int>",(dutb));
   fTree->Branch("FIXT","std::vector<int>",(fixt));
   fTree->Branch("FIXB","std::vector<int>",(fixb));
   fTree->Branch("timestamp",&timestamp,"timestamp/l");
   fTree->Branch("angle",&angle,"angle/I");
}

void EdmDataReader::endJob()
{
   delete dutt;
   delete dutb;
   delete fixt;
   delete fixb;
   
   return;
}

void EdmDataReader::analyze(const edm::Event& e, const edm::EventSetup& es)
{
   dutt->clear();
   dutb->clear();
   fixt->clear();
   fixb->clear();
   
   dutt->reserve(NCHANMAX);
   dutb->reserve(NCHANMAX);
   fixt->reserve(NCHANMAX);
   fixb->reserve(NCHANMAX);

   edm::Timestamp itime = e.time();
   time_t timestamp_t = itime.unixTime();
   timestamp = (ULong64_t)timestamp_t;
   struct tm *tm = localtime(&timestamp_t);
   char date[20];
   strftime(date, sizeof(date), "%Y-%m-%d", tm);

   edm::Handle< edm::DetSetVector<PixelDigi> > input;
   e.getByLabel( "SiStripDigitestproducer", "ProcessedRaw", input);
   
   if( !input.isValid() )
     {
	std::cout << "PixelDigis not found" << std::endl;
	exit(1);
     }
   
   edm::Handle< edm::DetSet<SiStripCommissioningDigi> > inputCond;
   e.getByLabel("SiStripDigiCondDataproducer","ConditionData", inputCond);

   if( !inputCond.isValid() )
     {
	std::cout << "ConditionData not found" << std::endl;
	exit(1);
     }
   
//   edm::DetSet<SiStripCommissioningDigi>::const_iterator it = cond_data_input->data.begin();
//   for(; it != cond_data_input->data.end(); it++)
//     {
/*	if( it->getKey() == 0x0b0000ff ) 
	  {
	     int value = it->getValue();
	     int stubs_FE0 = value & 0x00000003;
	     int stubs_FE1 = (value >> 2 ) & 0x00000003;
	  }
	
	 if( it->getKey() == 0x070000ff ) 
	  {
	     int value = it->getValue();
	     int pipe_1 = value & 0x000000FF;
	     int pipe_2 = (value >> 8) & 0x000000FF;
	     int pipe_3 = (value >> 16) & 0x000000FF;
	     int pipe_4 = (value >> 24) & 0x000000FF;
	  }
*/	
	// Phase2TrackerAnalysis/OnlineAnalysis/src/TBeamTest.cc
//	if( it->getKey() == 0x030000ff )
//	  {
//	     // tdc
//	     int value = it->getValue() & 0x000000FF;
//	     std::cout << value << std::endl;
//	  }	
/*	if( it->getKey() == 0x060000ff ) 
	  {
	     // cbcErrorCode
	     int value = it->getValue() & 0x0000000F;	     
	  }*/
  //   }

//   edm::Handle<edm::TriggerResults> HLTR;
//   e.getByLabel(edm::InputTag("TriggerResults","","HLT"),HLTR);
//   const edm::TriggerNames& trigNames = e.triggerNames(*HLTR);
//   std::cout << trigNames.triggerNames() << std::endl;
   
//   HLTConfigProvider hltConfig;
//   hltConfig.init("HLT");
//   unsigned int triggerIndex( hltConfig.triggerIndex("<a string containing a path name>") );
//   bool isFired = false;
//   if (triggerIndex < HLTR->size()) isFired = HLTR->accept(triggerIndex);
   
//   edm::ESHandle<TrackerGeometry> geom;
//   es.get<TrackerDigiGeometryRecord>().get( geom );
   
//   run(*input,geom);
   run(*input,*inputCond);
}

void EdmDataReader::run(const edm::DetSetVector<PixelDigi>   & input,
			const edm::DetSet<SiStripCommissioningDigi> & inputCond)
{
   for(unsigned int iv=0;iv<inputCond.data.size();iv++)
     {
//	if( inputCond.data[iv].getKey() == 0x030000ff )
//	  {
//	     int value = inputCond.data[iv].getValue() & 0x000000FF;
//	  }
	
/*	if( inputCond.data[iv].getKey() == 0x0b0000ff ) 
	  {		     
	     int value = inputCond.data[iv].getValue();
	     int stubs_DUT = value & 0x00000003;
	     int stubs_FIX = (value >> 2 ) & 0x00000003;
	     std::cout << stubs_DUT << " " << stubs_FIX << std::endl;
	  }*/

	if( inputCond.data[iv].getKey() == 0x040000ff )
	  {
	     // angle
	     int value = inputCond.data[iv].getValue();
	     angle = value;
	  }

//	if( inputCond.data[iv].getKey() == 0x0b0000ff )
//	  {
//	     int value = inputCond.data[iv].getValue();
//	     int latency = value & 0x00004000;
//	     std::cout << latency << std::endl;
//	  }
     }   
//   std::cout << inputCond.data.size() << std::endl;
//   edm::DetSet<SiStripCommissioningDigi>::const_iterator it = inputCond.data.begin();
/*   for(; it != inputCond.data.end(); it++)
     {
	if( it->getKey() == 0x030000ff )
	  {
	     int value = it->getValue() & 0x000000FF;
	     std::cout << value << std::endl;
	  }	
     }*/
   
   edm::DetSetVector<PixelDigi>::const_iterator it = input.begin();
   // loop over modules
   for( ; it != input.end(); it++) 
     {
	int detId = it->id;
	int id = -1;
	
	switch( detId ) {
	 case 51001: id = 0; break; // DUT top
	 case 51002: id = 1; break; // DUT bottom
	 case 51011: id = 2; break; // FIX top
	 case 51012: id = 3; break; // FIX bottom
	}

//	if( it->data.size() == 0 )
//	  {
//	     return;
//	  }
	if( id < 0 )
	  {
	     std::cout << "Unknown detector ID" << std::endl;
	     exit(1);
	  }	
	
	// loop over hits in the module
	std::vector<int> cbcc;
	cbcc.clear();	
	cbcc.reserve(NCHANMAX);
	
	for(edm::DetSet<PixelDigi>::const_iterator hit = it->begin(); hit!=it->end(); hit++ ) 
	  {
	     if( hit->adc() <= 250 ) continue;
	     
	     cbcc.push_back(hit->row());
	     // https://cmssdt.cern.ch/SDT/lxr/source/DataFormats/SiPixelDigi/interface/PixelDigi.h
		  /*	     std::cout << "channel=" << hit->channel()
		   << " adc=" << hit->adc()
		   << " packedData=" << hit->packedData()
		   << " row=" << hit->row()
		   << " column=" << hit->column()
		   << std::endl;*/
	  }
	
	for(int i=0;i<NCHANMAX;i++)
	  {
	     int isFired = (std::find(cbcc.begin(),cbcc.end(),i) != cbcc.end());
	     if( id == 0 ) dutt->push_back(isFired);
	     if( id == 1 ) dutb->push_back(isFired);
	     if( id == 3 ) fixt->push_back(isFired);
	     if( id == 4 ) fixb->push_back(isFired);
	  }	
	
//	const GeomDetUnit      * geoUnit = geom->idToDetUnit( detIdObject );
     }
   
   fTree->Fill();
   
//   for(unsigned int ip=0;ip<input.size();ip++)
//     {
//	edm::DetSet<PixelDigi> ipd = input[ip];
//	DetId detIdObject(ipd.detId());

//	const GeomDetUnit      * geoUnit = geom->idToDetUnit( detIdObject );
//	const PixelGeomDetUnit * pixDet  = dynamic_cast<const PixelGeomDetUnit*>(geoUnit);
//	if (! pixDet) 
//	  {
//	     assert(0);
//	  }	
//     }   
}

