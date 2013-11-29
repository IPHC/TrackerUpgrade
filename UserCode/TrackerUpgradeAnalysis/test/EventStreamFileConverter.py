import FWCore.ParameterSet.Config as cms 

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('python')

options.register ('inputFile',
                  'file:input.dat',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.string,
                  'Input files')

options.register ('outFile',
                  'edm.root',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.string,
                  'Output filename')
                  
options.parseArguments()                  

iFile = options.inputFile
flist = iFile.split(',')
oFile = options.outFile

# process declaration
process = cms.Process("NEW") 


# message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi") 
#process.MessageLogger.cerr.FwkReport.reportEvery = 10000
#process.MessageLogger = cms.Service("MessageLogger", 
#  destinations = cms.untracked.vstring('cout','log4cplus'), 
#  cout = cms.untracked.PSet(threshold = cms.untracked.string('WARNING')), 
#  log4cplus = cms.untracked.PSet(INFO = cms.untracked.PSet(reportEvery = cms.untracked.int32(10000)),threshold = cms.untracked.string('WARNING'))
#)

 
process.source = cms.Source("NewEventStreamFileReader",
     fileNames =cms.untracked.vstring( flist ) 
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


## EventContentAnalyzer
process.ana = cms.EDAnalyzer("EventContentAnalyzer")


## Output module
process.out = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring(
#        'drop *',
        'keep *'
    ),
    fileName = cms.untracked.string( oFile )
)

process.p1 = cms.Path(process.ana)
process.p2 = cms.EndPath(process.out)



