import FWCore.ParameterSet.Config as cms

process = cms.Process("EcalStudy")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 20
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
    fileNames = cms.untracked.vstring(
        'file:/home/guts/CMS/CMSSW_8_0_21/src/ParticleGunStudy/StudyFileCollection/TestSamples/events_QCD_JetPy8.root'
    )
)

process.ecalAnalyzer = cms.EDAnalyzer('ParticleGunEcalHitStudy',
    plotSaveFolder = cms.string(
        "/home/guts/CMS/CMSSW_8_0_21/src/ParticleGunStudy/StudyFileCollection/JetPy8"
    ),
)

process.TFileService = cms.Service("TFileService",
                                       fileName = cms.string("file:///home/guts/CMS/CMSSW_8_0_21/src/ParticleGunStudy/StudyFileCollection/TestSamples/results_Jet_Py8_main.root")
                                   )


process.p = cms.Path(process.ecalAnalyzer)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string("file:///home/guts/CMS/CMSSW_8_0_21/src/ParticleGunStudy/StudyFileCollection/TestSamples/output_collections_Jet_Py8_main.root"),

)
process.outpath = cms.EndPath(process.out)
