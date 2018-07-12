# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: Configuration/Generator/python/SingleElectronPt10_cfi.py
# -s GEN,SIM,DIGI,L1,DIGI2RAW,HLT:@frozen2016,RAW2DIGI,RECO --conditions 80X_mcRun2_asymptotic_2016_TrancheIV_v6 
# --era Run2_2016 --nThreads 4 --datatier AODSIM --eventcontent AODSIM -n 10 
# python_filename SingleElectronPt10_cfi_py_GEN_TEST.py
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('HLT',eras.Run2_2016)

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
process.load('HLTrigger.Configuration.HLT_25ns15e33_v4_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(500)
)
process.MessageLogger.cerr.FwkReport.reportEvery = 1

# Input source
process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('Configuration/Generator/python/SingleElectronPt10_cfi.py nevts:10'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.AODSIMEventContent.outputCommands.append('keep *')
process.AODSIMEventContent.outputCommands.append('drop *_*_*_*')
process.AODSIMEventContent.outputCommands.append('keep *_*_EcalHitsE*_*')
process.AODSIMEventContent.outputCommands.append('keep *_genParticles_*_*')
process.AODSIMEventContent.outputCommands.append('keep *_ak4GenJets_*_*')
process.AODSIMEventContent.outputCommands.append('keep *_ak4PFJets_*_*')
process.AODSIMEventContent.outputCommands.append('keep *_simEcalDigis_*_*')

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
    fileName = cms.untracked.string('file:///home/guts/CMS/CMSSW_8_0_21/src/ParticleGunStudy/StudyFileCollection/TestSamples/events_QCD_JetPy8.root'),
    outputCommands = process.AODSIMEventContent.outputCommands
)

# Additional output definition

# Other statements
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_mcRun2_asymptotic_2016_TrancheIV_v6', '')

from Configuration.Generator.Pythia8CommonSettings_cfi import *
from Configuration.Generator.Pythia8CUEP8M1Settings_cfi import *


###########################################################################

process.generator = cms.EDFilter("Pythia8GeneratorFilter",
        comEnergy = cms.double(13000.0),
        crossSection = cms.untracked.double(2.0221e+09),
        filterEfficiency = cms.untracked.double(1.0),
        maxEventsToPrint = cms.untracked.int32(1),
        pythiaHepMCVerbosity = cms.untracked.bool(False),
        pythiaPylistVerbosity = cms.untracked.int32(1),
        #reweightGen = cms.bool(True), #

        PythiaParameters = cms.PSet(
                pythia8CommonSettingsBlock,
                pythia8CUEP8M1SettingsBlock,
                processParameters = cms.vstring(
                        'HardQCD:all = on',
                        'PhaseSpace:pTHatMin = 15',
                        'PhaseSpace:pTHatMax = 500',
                        'PhaseSpace:bias2Selection = on',
                        'PhaseSpace:bias2SelectionPow = 4.5',
                        'PhaseSpace:bias2SelectionRef = 15.',

                ),
                parameterSets = cms.vstring('pythia8CommonSettings',
                                            'pythia8CUEP8M1Settings',
                                            'processParameters'
                                            )
        )
) 


###########################################################################



process.simEcalDigis = cms.EDProducer("EcalSelectiveReadoutProducer",
	# Let's see what parameters are nessesary? Comments on each paramter are take from 
	# cmssw/SimCalorimetry/EcalSelectiveReadoutProducers/python/ecalDigis_cfi.py

    # Label of input EB and EE digi collections
       # Label of input EB and EE digi collections
    digiProducer = cms.string('simEcalUnsuppressedDigis'),

    # Instance name of input EB digi collections
    EBdigiCollection = cms.string(''),

    # Instance name of input EB digi collections
    EEdigiCollection = cms.string(''),

    # Instance name of output EB SR flags collection
    EBSrFlagCollection = cms.string('ebSrFlags'),

    # Instance name of output EE SR flags collection
    EESrFlagCollection = cms.string('eeSrFlags'),

    # Instance name of output EB digis collection
    EBSRPdigiCollection = cms.string('ebDigis'),

    # Instance name of output EE digis collection
    EESRPdigiCollection = cms.string('eeDigis'),

    # Switch for reading SRP settings from condition database
    configFromCondDB = cms.bool(True),

    # Switch to turn off SRP altogether using special DB payload
    UseFullReadout = cms.bool(False),

    # ES label?
    # NZSLabel = cms.ESInputTag(' '),

    # Label name of input ECAL trigger primitive collection
    trigPrimProducer = cms.string('simEcalTriggerPrimitiveDigis'),

    # Instance name of ECAL trigger primitive collection
    trigPrimCollection = cms.string(''),

    #switch to run w/o trigger primitive. For debug use only
    trigPrimBypass = cms.bool(False),

    # Mode selection for "Trig bypass" mode
    # 0: TT thresholds applied on sum of crystal Et's
    # 1: TT thresholds applies on compressed Et from Trigger primitive
    # @ee trigPrimByPass_ switch
    trigPrimBypassMode = cms.int32(0),
                              
    #for debug mode only:
    trigPrimBypassLTH = cms.double(1.0),

    #for debug mode only:
    trigPrimBypassHTH = cms.double(1.0),

    #for debug mode only
    trigPrimBypassWithPeakFinder = cms.bool(True),
                              
    #number of events whose TT and SR flags must be dumped (for debug purpose):
    dumpFlags = cms.untracked.int32(0),
                              
    #logical flag to write out SrFlags
    writeSrFlags = cms.untracked.bool(True),

    #switch to apply selective readout decision on the digis and produce
    #the "suppressed" digis
    produceDigis = cms.untracked.bool(True),

    #Trigger Tower Flag to use when a flag is not found from the input
    #Trigger Primitive collection. Must be one of the following values:
    # 0: low interest, 1: mid interest, 3: high interest
    # 4: forced low interest, 5: forced mid interest, 7: forced high interest
    defaultTtf = cms.int32(4)

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
# Custom step

process.ecaltriggerobservation_step = cms.Path(process.simEcalDigis)


# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.digitisation_step,process.L1simulation_step,process.digi2raw_step,process.ecaltriggerobservation_step
	)
process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.raw2digi_step,process.reconstruction_step, # ##Put here?
process.endjob_step,process.AODSIMoutput_step])

#Setup FWK for multithreaded
process.options.numberOfThreads=cms.untracked.uint32(4)
process.options.numberOfStreams=cms.untracked.uint32(0)
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 

# customisation of the process.

# Automatic addition of the customisation function from HLTrigger.Configuration.customizeHLTforMC
from HLTrigger.Configuration.customizeHLTforMC import customizeHLTforFullSim 

#call to customisation function customizeHLTforFullSim imported from HLTrigger.Configuration.customizeHLTforMC
process = customizeHLTforFullSim(process)

# End of customisation functions

