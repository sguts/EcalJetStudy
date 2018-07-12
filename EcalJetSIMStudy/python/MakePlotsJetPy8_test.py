import FWCore.ParameterSet.Config as cms

process = cms.Process("EcalStudy")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.MessageLogger.categories.append('Looping timing')
#process.MessageLogger.cerr.threshold = 'DEBUG'

process.options = cms.untracked.PSet(
   wantSummary = cms.untracked.bool(False),
)



#new shit

process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc')
process.load("Configuration.StandardSequences.MagneticField_cff")

#process.load("Configuration.Geometry.GeometryRecoDB_cff")
#process.GlobalTag = GlobalTag(process.GlobalTag, '80X_mcRun2_asymptotic_2016_TrancheIV_v8')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )





process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/s/sguts/private/CMSSW_9_4_8/src/EcalJetStudy/Output/TestEvents.root')
)

process.ecalAnalyzer = cms.EDAnalyzer('ParticleGunEcalHitStudy',
)

process.TFileService = cms.Service("TFileService",
                                       fileName = cms.string("/afs/cern.ch/user/s/sguts/private/CMSSW_9_4_8/src/EcalJetStudy/Output/TestResults.root")
                                   )


process.p = cms.Path(process.ecalAnalyzer)

