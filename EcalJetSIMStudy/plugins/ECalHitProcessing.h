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

//General Jets for matching
#include "DataFormats/JetReco/interface/Jet.h"

//deltaR function?
//#include "DataFormats/Math/interface/deltaR.h"

//Maybe results of Selectivbe readout?

#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"


#include <vector>
#include <math.h>
#include "DataFormats/Math/interface/deltaR.h"
//Histogram creation tools
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TCanvas.h"
#include <TROOT.h>
#include <TStyle.h>
#include <TPad.h>
//#include <TFileDirectory.h>
//#include "TVirtualPad.h"

//File services
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//custom to_string_with_precision
#include "ToStringWithPrecision.h"


struct BarrelEcalHistogramContainer {
    TH2D *energyiPhiiEta;
    TH2D *energyTiPhiiEta;

    
    
  
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



struct JetEcalHistogramContainer {
    TH1D *genJetsNumEvents;
    TH1D *genJetsPt;
    TH1D *genJetsEnergy;

    TH1D *PFJetsNumEvents;
    TH1D *PFJetsPt;
    TH1D *PFJetsEnergy;
    
    TH2D *genJetsAllEnergyEta;

    TH2D *interestingGenPFJetsNumEvents;
    
    TH1D *genGenJetsClosestDR;
    TH1D *genPFJetsClosestDR;
    
    TH1D *genJetsEmE_fracE;
    TH1D *genJetsHadE_fracE;
    TH1D *genJetsInvE_fracE;
    TH1D *genJetsAuxE_fracE;
    TH1D *genJetsSumE_fracE;
    
    TH1D *genPartonsJetDR_pions;
    TH1D *genPartonsJetDR_photons;
    TH1D *genPartonsJetDR_hadrons;
    TH1D *genPartonsJetDR_other;
    
    std::map <double,TH1D*> genJetsEDepBar_Map;
    std::map <double,TH1D*> genJetsEDepBar_fracE_Map;
    
    std::map <double,TH1D*> genJetsEDepEndc_Map;
    std::map <double,TH1D*> genJetsEDepEndc_fracE_Map;
    //Where the fuck should we put it?

    TH1D *simEcal_BarDepEcryst_hardJets;
    TH1D *simEcal_BarDepEsupcryst_hardJets;
    TH1D *simEcal_EndDepEcryst_hardJets;
    
    TH1D *simEcal_BarDepETcryst_hardJets;
    TH1D *simEcal_BarDepETsupcryst_hardJets;
    TH1D *simEcal_EndDepETcryst_hardJets;
    
    
    TH1D *genJetsAvEDepBar_fracE;
    TH1D *genJetsAvEDepEnd_fracE;
    
    TH1D *genJetsAvSoftJetE_Bar_fracE;
    TH1D *genJetsAvSoftJetE_End_fracE;
    
    TH1D *hardBarrelJetCount;
    TH1D *hardEndcapJetCount;
    
};

struct JetEcalSplitHistogramContainer {
    std::vector <TH3D*> numCrystal_ESim_Eta_Pt_DRIncAr;
    std::vector <TH3D*> numCrystal_ESimT_Eta_Pt_DRIncAr;

    std::vector <TH3D*> numJet_ESimDivNcr_Eta_Pt_DRIncAr;
    std::vector <TH3D*> numJet_ESimDivNcr_Eta_Pt_DRDifAr;

    std::vector <TH3D*> numJet_ESimDivEjet_Eta_Pt_DRIncAr;
    std::vector <TH3D*> numJet_ESimDivEjet_Eta_Pt_DRDifAr;

    //I think it's better to shift those declarations here? DR is defenetily going to be used. Sigh.
    std::vector <double> etaBinning = {0,0.5,1,1.5,2,2.5,3};
    std::vector <double> ptBinning = {15,20,30,50,80,150};

    std::vector <double> stepsDR = {0,0.1,0.2,0.3,0.4,0.5};

};


struct EventCollections {
    std::vector<reco::GenParticle> genParticles;
    std::vector<reco::GenJet> genJets;
    std::vector<reco::PFJet> PFJets;
    
    edm::PCaloHitContainer endcapHitsContainer;
    edm::PCaloHitContainer barrelHitsContainer;

    const CaloSubdetectorGeometry *endcapGeometry = nullptr;
    const CaloSubdetectorGeometry *barrelGeometry = nullptr;
    
    //Results of digitization
    const EBDigiCollection barrelDigiCollection;
    const EEDigiCollection endcapDigiCollection;
    
    
};

struct FilteredEventCollections {
    std::vector<const reco::GenJet*> hardGenJetPointers;
    std::vector<const reco::PFJet*> hardPFJetPointers;
};

class ECalJetInteractionProcessor {
public:
    void InitializeHistograms ();
    void FillEventHistograms (const EventCollections &eventDataContainer);
    void FinalizeHistograms ();

private:
    FilteredEventCollections filteredCollections;
    
    void InitializeEventAnalysisVariables ();
    void InitializeBarrelHistograms ();
    void InitializeEndcapHistograms ();
    void InitializeJetHistograms ();
    void InitializeJetSplitHistograms ();

    void InitializeJetSplitHistogramsArray (TFileDirectory &jetSplitSubDir, std::vector <TH3D*> &targetArray, std::vector <double> binXvar, std::string histNaming, std::string histTitling, std::string titlingXaxis, bool DRinclusive=true) ;
    
    int arrayCiPhi (int iphi);
    int arraySCiPhi (int iphi);
    int arrayCiEta (int ieta);
    int arraySCiEta (int ieta);
    
    
    
    void FillEventData (const EventCollections &eventDataContainer);
    void FillHitInfo (const PCaloHit &currentHit, bool isBarrel, const EventCollections &eventDataContainer);
    
    bool IsInterestingBarrelHit (int ieta, int iphi);
    bool IsInterestingEndcapHit (int ix, int iy, bool Posz);
    
    void FillBarrelHitHistogramms (const EventCollections &eventDataContainer);
    void FillEndcapHitHistogramms (const EventCollections &eventDataContainer);
    void FillJetsHistograms (const EventCollections &eventDataContainer);
    void FillJetsSplitHistograms (const EventCollections &eventDataContainer);
    
    bool IsHardGenJet (const reco::GenJet &jet, const EventCollections &eventDataContainer) ;
    bool IsHardPFJet (const reco::PFJet &jet, const EventCollections &eventDataContainer);
    
    const reco::GenJet* FindClosestNonequalJet (const reco::GenJet *target, std::vector<const reco::GenJet*> collection);
    const reco::PFJet* FindClosestPFJet (const reco::GenJet *target, std::vector<const reco::PFJet*> collection);
    
    double EcalEDepJet_DR (const reco::Jet &jet, double lowCutoffDR, double highCutoffDR, const EventCollections &eventDataContainer);
    double EcalNDepJet_DR (const reco::Jet &jet, double lowCutoffDR, double highCutoffDR, const EventCollections &eventDataContainer);
    double genJetSoftE_DR (const reco::Jet &jet, double cutoffDR, const EventCollections &eventDataContainer);
    void FillJetConstituentsHist (const reco::GenJet jet);
    
    bool  IsBarrelJet (const reco::Jet &jet, const EventCollections &eventDataContainer);
    bool  IsEndcapJet (const reco::Jet &jet, const EventCollections &eventDataContainer);
    
    void  FillJetCrystalSplitHistograms (const reco::Jet &jet, int number, double lowCutoffDR, double highCutoffDR, const EventCollections &eventDataContainer);
    
    BarrelEcalHistogramContainer EBHistCont;
    EndcapEcalHistogramContainer EEHistCont;
    JetEcalHistogramContainer JetHistCont;
    JetEcalSplitHistogramContainer JetSplitHistCont;
    
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
    
    int endcapSupercrystalCount = 317;
    std::vector<double> endcapPoszSupercrystalEnergyEvent;
    std::vector<double> endcapNegzSupercrystalEnergyEvent;
    
    int endcapiXCount=101;
    std::vector<std::vector<double>> endcapCrystalEnergyEventPosz;
    std::vector<std::vector<double>> endcapCrystalEnergyEventNegz;
    
    double jetsSeparationDR = 2;
    std::vector<double> listConeDR;
};



#endif