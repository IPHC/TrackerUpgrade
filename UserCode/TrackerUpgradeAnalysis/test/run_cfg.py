import FWCore.ParameterSet.Config as cms

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('python')

options.register ('inputFile',
                  'file:input.root',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.string,
                  'Input files')

options.register ('outFile',
                  'output.root',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.string,
                  'Output filename')
                  
options.parseArguments()                  

process = cms.Process("Analysis")

#process.load("Configuration.Geometry.GeometryIdeal_cff")
#process.load("Configuration.StandardSequences.MagneticField_38T_cff")

#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = "GR_P_V28::All"

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

iFile = options.inputFile
flist = iFile.split(',')
oFile = options.outFile

process.source = cms.Source( "PoolSource",
                             fileNames = cms.untracked.vstring(
                             flist
			     )
                           )

process.TFileService = cms.Service("TFileService",
        fileName = cms.string(oFile)
)

process.EdmDataReader = cms.EDAnalyzer( "EdmDataReader" )

process.p1 = cms.Path( process.EdmDataReader )

