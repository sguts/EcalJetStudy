// -*- C++ -*-
//
// Package:    ParticleGunStudy/ParticleGunEcalHitStudy
// Class:      ParticleGunEcalHitStudy
// 
/**\class ParticleGunEcalHitStudy ParticleGunEcalHitStudy.cc ParticleGunStudy/ParticleGunEcalHitStudy/plugins/ParticleGunEcalHitStudy.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Sergey Guts
//         Created:  Tue, 26 Dec 2017 20:20:08 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//Let's make a reasonable system of includes, ok?

//Subjects of study
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"

#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"

#include "DataFormats/JetReco/interface/PFJet.h"


//Detector geometry
#include "Calibration/Tools/interface/EcalRingCalibrationTools.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "FWCore/Framework/interface/ESHandle.h"//nessesary to get geometry


//And now the main iclude:
#include "ECalHitProcessing.h"

//File services: are they nessesary?
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class ParticleGunEcalHitStudy : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit ParticleGunEcalHitStudy(const edm::ParameterSet& iConfig);
      ~ParticleGunEcalHitStudy();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      edm::ESHandle<CaloGeometry> pG;
      EventCollections currentEventCollections;

      edm::Handle<std::vector<reco::GenParticle> > genParticleHandle;
      edm::EDGetTokenT<std::vector<reco::GenParticle> > genParticleToken;
      edm::Handle<std::vector<reco::GenJet>> genJetsHandle;
      edm::EDGetTokenT<std::vector<reco::GenJet>> genJetsToken;

      edm::Handle<edm::PCaloHitContainer> pCaloHitsEBHandle;
      edm::EDGetTokenT<edm::PCaloHitContainer> pCaloHitsEBToken;
      edm::Handle<edm::PCaloHitContainer> pCaloHitsEEHandle;
      edm::EDGetTokenT<edm::PCaloHitContainer> pCaloHitsEEToken;
      edm::Handle<edm::PCaloHitContainer> pCaloHitsESHandle;
      edm::EDGetTokenT<edm::PCaloHitContainer> pCaloHitsESToken;

      edm::Handle<std::vector<reco::PFJet>> ak4PFJetsHandle;
      edm::EDGetTokenT<std::vector<reco::PFJet>> ak4PFJetsToken;

      double q;
      ECalJetInteractionProcessor HistogramContainer;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
ParticleGunEcalHitStudy::ParticleGunEcalHitStudy(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
   //std::cout << "Hi! number 2" << std::endl;
   usesResource("TFileService");
   edm::Service<TFileService> file_service;
 
   genParticleToken = consumes< std::vector < reco::GenParticle> >(edm::InputTag("genParticles"));
   genJetsToken = consumes <std::vector <reco::GenJet>>(edm::InputTag("ak4GenJets"));
   ak4PFJetsToken = consumes <std::vector<reco::PFJet>>(edm::InputTag("ak4PFJets"));
   
   pCaloHitsEBToken = consumes<edm::PCaloHitContainer>(edm::InputTag("g4SimHits", "EcalHitsEB", "HLT"));
   pCaloHitsEEToken = consumes<edm::PCaloHitContainer>(edm::InputTag("g4SimHits", "EcalHitsEE", "HLT"));
   pCaloHitsESToken = consumes<edm::PCaloHitContainer>(edm::InputTag("g4SimHits", "EcalHitsES", "HLT"));

   

   
   HistogramContainer.InitializeHistograms(file_service, iConfig.getParameter<std::string>("plotSaveFolder"));
   

}


ParticleGunEcalHitStudy::~ParticleGunEcalHitStudy()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
ParticleGunEcalHitStudy::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;
   

   
    //Geometry under construction?
    
    if ( currentEventCollections.endcapGeometry == nullptr || currentEventCollections.barrelGeometry == nullptr) {
        iSetup.get<CaloGeometryRecord>().get(pG);
        EcalRingCalibrationTools::setCaloGeometry(&(*pG));
    
        currentEventCollections.endcapGeometry =  pG->getSubdetectorGeometry(DetId::Ecal, EcalEndcap);
        currentEventCollections.barrelGeometry =  pG->getSubdetectorGeometry(DetId::Ecal, EcalBarrel);
    }
   
   //getting digi shit?
   
   //std::cout << "Getting by token" << std::endl;


   iEvent.getByToken(genParticleToken, genParticleHandle); //GenParticle
   iEvent.getByToken(genJetsToken, genJetsHandle); //GenJets
   iEvent.getByToken(ak4PFJetsToken, ak4PFJetsHandle); //PF Jets

   //std::cout << "Grabbing Ecal" << std::endl;
   iEvent.getByToken(pCaloHitsEBToken, pCaloHitsEBHandle);     //PCaloHit Barrel
   iEvent.getByToken(pCaloHitsEEToken, pCaloHitsEEHandle);     //PCaloHit Endcap
   iEvent.getByToken(pCaloHitsESToken, pCaloHitsESHandle);     //PCaloHit Preshower

   

   //std::cout << "Assigning" << std::endl;

   currentEventCollections.genParticles = *genParticleHandle;
   //std::cout << "1" << std::endl;
   currentEventCollections.genJets = *genJetsHandle;
   //std::cout << "2" << std::endl;
   currentEventCollections.ak4PFJets = *ak4PFJetsHandle;
   //std::cout << "3" << std::endl;
   currentEventCollections.endcapHitsContainer = *pCaloHitsEEHandle;
   //std::cout << "4" << std::endl;
   currentEventCollections.barrelHitsContainer = *pCaloHitsEBHandle;
   //std::cout << "5" << std::endl;

   //std::cout << "going to FillEventHistograms" << std::endl;
   HistogramContainer.FillEventHistograms(currentEventCollections);
}


// ------------ method called once each job just before starting event loop  ------------
void 
ParticleGunEcalHitStudy::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ParticleGunEcalHitStudy::endJob() 
{
  HistogramContainer.DrawHistograms();
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ParticleGunEcalHitStudy::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ParticleGunEcalHitStudy);