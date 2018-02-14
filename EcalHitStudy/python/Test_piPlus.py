import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 500

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:///home/guts/CMS/CMSSW_8_0_21/src/ParticleGunStudy/StudyFileCollection/TestSamples/events_piPlus.root'
    )
)

process.demo = cms.EDAnalyzer('ParticleGunEcalHitStudy'
)

process.TFileService = cms.Service("TFileService",
                                       fileName = cms.string("file:///home/guts/CMS/CMSSW_8_0_21/src/ParticleGunStudy/StudyFileCollection/TestSamples/results_piPlus.root")
                                   )


process.p = cms.Path(process.demo)
