# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: Configuration/Generator/python/SingleElectronPt10_cfi.py -s GEN,SIM,DIGI,L1,DIGI2RAW,HLT:@frozen2016,RAW2DIGI,RECO --runUnscheduled --conditions 80X_mcRun2_asymptotic_2016_TrancheIV_v6 --era Run2_2016 --nThreads 4 --datatier AODSIM --eventcontent AODSIM -n 100 --python_filename SingleElectronPt10_cfi_py_GEN_IDEAL.py
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('HLT',eras.Run2_2017)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.Geometry.GeometrySimDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic50ns13TeVCollision_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
#process.load('HLTrigger.Configuration.HLT_25ns15e33_v4_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.load("IOMC.RandomEngine.IOMC_cff")
#process.RandomNumberGeneratorService.g4SimHits.initialSeed = 9876
process.RandomNumberGeneratorService.generator.initialSeed = 9879 
#process.RandomNumberGeneratorService.VtxSmeared.initialSeed = 79736

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(25)
)

process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

# Input source
process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('Configuration/Generator/python/SingleElectronPt10_cfi.py nevts:100'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition
#process.AODSIMEventContent.outputCommands.append('drop *_*_*_*')
process.AODSIMEventContent.outputCommands.append('keep *_*_EcalHitsE*_*')
process.AODSIMEventContent.outputCommands.append('keep *_gedPhotons_*_*')
process.AODSIMEventContent.outputCommands.append('keep *_photons_*_*')
process.AODSIMEventContent.outputCommands.append('keep *_gedPhotonCore_*_*')
process.AODSIMEventContent.outputCommands.append('keep *_photonCore_*_*')

process.AODSIMEventContent.outputCommands.append('keep *_gedGsfElectrons_*_*')
process.AODSIMEventContent.outputCommands.append('keep *_gedGsfElectronCores_*_*')
process.AODSIMEventContent.outputCommands.append('keep *_uncleanedOnlyGsfElectronCores_*_*')
process.AODSIMEventContent.outputCommands.append('keep *_electronGsfTracks_*_*')

process.AODSIMEventContent.outputCommands.append('keep *_genParticles_*_*')
process.AODSIMEventContent.outputCommands.append('keep *_addPileupInfo_*_*')

process.AODSIMoutput = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    ),
    compressionAlgorithm = cms.untracked.string('LZMA'),
    compressionLevel = cms.untracked.int32(4),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('AODSIM'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
    fileName = cms.untracked.string('Generate_Prompt_Electron_000.root'),
    outputCommands = process.AODSIMEventContent.outputCommands
)

# Additional output definition

# Other statements
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '94X_mc2017_realistic_v14', '')


process.generator = cms.EDProducer("FlatRandomPtGunProducer",
    AddAntiParticle = cms.bool(True),
    PGunParameters = cms.PSet(
        MaxEta = cms.double(0.41),
        MaxPhi = cms.double(3.14159265359),
        MaxPt = cms.double(46),
        MinEta = cms.double(0.4),
        MinPhi = cms.double(-3.14159265359),
        MinPt = cms.double(45),
        PartID = cms.vint32(11)
    ),
    Verbosity = cms.untracked.int32(0),
    firstRun = cms.untracked.uint32(1),
    psethack = cms.string('single electron pt 10')
)


# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim)
process.digitisation_step = cms.Path(process.pdigi)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.AODSIMoutput_step = cms.EndPath(process.AODSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.digitisation_step,process.L1simulation_step,process.digi2raw_step)
#process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.raw2digi_step,process.reconstruction_step,process.endjob_step,process.AODSIMoutput_step])

#Setup FWK for multithreaded
process.options.numberOfThreads=cms.untracked.uint32(4)
process.options.numberOfStreams=cms.untracked.uint32(0)
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 

# customisation of the process.

# Automatic addition of the customisation function from HLTrigger.Configuration.customizeHLTforMC
#from HLTrigger.Configuration.customizeHLTforMC import customizeHLTforFullSim 

#call to customisation function customizeHLTforFullSim imported from HLTrigger.Configuration.customizeHLTforMC
#process = customizeHLTforFullSim(process)

# End of customisation functions
#do not add changes to your config after this point (unless you know what you are doing)
from FWCore.ParameterSet.Utilities import convertToUnscheduled
process=convertToUnscheduled(process)
from FWCore.ParameterSet.Utilities import cleanUnscheduled
process=cleanUnscheduled(process)

