#ifndef ECalHitProcessing_H
#define ECalHitProcessing_H 
//Gen-level Objects of study
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"

//Sim-level objects
#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"

#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

//Detector geometry
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"

//Reconstructed Jets

#include "DataFormats/JetReco/interface/PFJet.h"

//Maybe results of Selectivbe readout?

#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"

#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"

#include <vector>
#include <math.h>
//Histogram creation tools
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include <TROOT.h>
#include <TStyle.h>
#include <TPad.h>
//#include "TVirtualPad.h"

//File services: are they nessesary?
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//timed cout
#include <ctime>

struct  EndcapEcalCrystalHit {
double rawId;

    bool posz;
    double ix, iy;
    double isc;
    double ir;

    double energy;
    double depth;

};

struct BarrelEcalCrystalHit {
    double rawId;

    int ieta, iphi;

    double energy;
    double depth;
};


struct BarrelEcalHistogramContainer {
    TH2D *energyiPhiiEta;
    TH2D *energyTiPhiiEta;

    TH2D *energyTToweriPhiiEta;

    TH1D *towerEnergyHitNumber;
    TH1D *towerEnergyTHitNumber;
  
};

struct EndcapEcalHistogramContainer {
  
    TH1D *energyTotalHitNumber;


    TH2D *XYTotalEnergyDepositionPosZ;
    TH2D *XYTotalEnergyDepositionNegZ;
    TH2D *XYTotalHitsNumberPosZ;
    TH2D *XYTotalHitsNumberNegZ;
    
    TH1D *energyPosZTotalEvents;
    TH1D *energyNegZTotalEvents;

    TH1D *supercrystalPoszEnergyHitNumber;
    TH1D *supercrystalNegzEnergyHitNumber;

    TH1D *isEEPlus;

};



struct GeneralEcalHistogramContainer {
    TH1D *genJetsNumEvents;
    TH1D *genJetsPt;
    TH1D *genJetsEnergy;

    TH1D *ak4PFJetsNumEvents;
    TH1D *ak4PFJetsPt;
    TH1D *ak4PFJetsEnergy;

    TH2D *genak4PFJetsNumEvents;
};

struct HistogramPrintInfo { 
    std::string analyzerIdentificator;

    std::string customHistName;
    std::string customXAxisName;
    std::string customYAxisName;

    double logYAxis;

    double scaleValue;

    HistogramPrintInfo (double inputScaleValue = 1, std::string customHistName = "", std::string customXAxisName = "", std::string customYAxisName = "");


};

struct EventCollections {
    std::vector<reco::GenParticle> genParticles;
    std::vector<reco::GenJet> genJets;
    std::vector<reco::PFJet> ak4PFJets;
    
    edm::PCaloHitContainer endcapHitsContainer;
    edm::PCaloHitContainer barrelHitsContainer;

    const CaloSubdetectorGeometry *endcapGeometry = nullptr;
    const CaloSubdetectorGeometry *barrelGeometry = nullptr;//both were const at the place they were copied from
    
    //Results of digitization
    const EBDigiCollection barrelDigiCollection;
    const EEDigiCollection endcapDigiCollection;
    
    
};

class ECalJetInteractionProcessor {
public:
    void InitializeHistograms (edm::Service<TFileService> file_service, const std::string &plotFolder);
    void FillEventHistograms (const EventCollections &eventDataContainer);
    void DrawHistograms ();

private:
    void InitializeEventAnalysisVariables ();
    void InitializeBarrelHistograms ();
    void InitializeEndcapHistograms ();
    void InitializeJetHistograms ();
    
    int arrayCiEta (int ieta);
    int arraySCiEta (int ieta);
    
    void FillEventData (const EventCollections &eventDataContainer);
    void FillHitInfo (const PCaloHit &currentHit, bool isBarrel, const EventCollections &eventDataContainer);
    
    bool IsInterestingBarrelHit (int ieta, int iphi);
    bool IsInterestingEndcapHit (int ix, int iy, bool Posz);
    
    void FillBarrelHitHistogramms (const EventCollections &eventDataContainer);
    void FillEndcapHitHistogramms (const EventCollections &eventDataContainer);
    
    edm::Service<TFileService> analyzer_file_service;
    BarrelEcalHistogramContainer EBHistCont;
    EndcapEcalHistogramContainer EEHistCont;
    GeneralEcalHistogramContainer GenHistCont;
    
    double minCrystalInterestingEnergy = 0.001;
    
    int barrelCrystaliEtaCount = 85*2+1;
    int barrelCrystaliPhiCount = 360;
    int barrelCrystalMiniEta = -85;
    
    std::vector<std::vector<double>> barrelCrystalEnergyEvent;
    std::vector<std::vector<double>> barrelCrystalEnergyTEvent;
    
    int barrelSuperCrystaliEtaCount = 17*2+1;
    int barrelSuperCrystaliPhiCount = 72;
    std::vector<std::vector<double>> barrelSuperCrystalEnergyEvent;
    std::vector<std::vector<double>> barrelSuperCrystalEnergyTEvent;
    
    int endcapSupercrystalCount = 137;
    std::vector<double> endcapPoszSupercrystalEnergyEvent;
    std::vector<double> endcapNegzSupercrystalEnergyEvent;
    
    int endcapiXCount=101;
    std::vector<std::vector<double>> endcapCrystalEnergyEventPosz;
    std::vector<std::vector<double>> endcapCrystalEnergyEventNegz;
    
    
    

    template<class T> void DrawHistogram (T *hist, HistogramPrintInfo info);
    
    bool analyzerIdSet;
    std::string folderName;
    time_t now;

};



#endif