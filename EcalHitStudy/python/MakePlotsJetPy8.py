import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

#Geometry stuff
process.load('Configuration.EventContent.EventContent_cff')
#process.load('SimGeneral.MixingModule.mixNoPU_cfi')
#process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
#process.load('Configuration.StandardSequences.MagneticField_38T_cff')

#process.load("Geometry.EcalCommonData.EcalOnly_cfi")
#process.load("Geometry.EcalMapping.EcalMapping_cfi")
#process.load("Geometry.EcalMapping.EcalMappingRecord_cfi")
#process.load("Geometry.CaloEventSetup.CaloGeometry_cff")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:///home/guts/CMS/CMSSW_8_0_21/src/ParticleGunStudy/StudyFileCollection/TestSamples/events_JetPy8.root'
    )
)

process.demo = cms.EDAnalyzer('ParticleGunEcalHitStudy'
)

process.TFileService = cms.Service("TFileService",
                                       fileName = cms.string("file:///home/guts/CMS/CMSSW_8_0_21/src/ParticleGunStudy/StudyFileCollection/TestSamples/results_JetPy8.root")
                                   )


process.p = cms.Path(process.demo)
