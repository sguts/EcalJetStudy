#ifndef ECalHitProcessing_H
#define ECalHitProcessing_H 
//Gen-level Objects of study
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"

//Sim-level objects
#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"

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

struct 	EndcapEcalCrystalHit {
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



struct EndcapEcalHistogramContainer {
  
	TH1D *energyTotalHitNumber;

	TH1D *radiusTotalEnergyDeposition;
	TH1D *radiusTotalHitNumber;

	TH2D *XYTotalEnergyDepositionPosZ;
	TH2D *XYTotalEnergyDepositionNegZ;
	TH2D *XYTotalHitsNumberPosZ;
	TH2D *XYTotalHitsNumberNegZ;
	
	TH1D *energyPosZTotalEvents;
	TH1D *energyNegZTotalEvents;

	TH1D *supercrystalPoszEnergyHitNumber;
	TH1D *supercrystalNegzEnergyHitNumber;

	TH2D *eta1eta2;
	TH1D *isEEPlus;

	TH1D *genJetsNumEvents;
	TH1D *genJetsPt;
	TH1D *genJetsEnergy;

	TH1D *ak4PFJetsNumEvents;
	TH1D *ak4PFJetsPt;
};

struct BarrelEcalHistogramContainer {

  
};

struct GeneralEcalHistogramContainer {
  
};

struct HistogramPrintInfo { 
	std::string analyzerIdentificator;

	std::string customHistName;
	std::string customXAxisName;
	std::string customYAxisName;

	double logYAxis;

	double scaleValue;

	HistogramPrintInfo (std::string inputAnalyzerIdentificator, double inputScaleValue = 1, std::string customHistName = "", std::string customXAxisName = "", std::string customYAxisName = "");


};

struct EventCollections {
    std::vector<reco::GenParticle> genParticles;
    std::vector<reco::GenJet> genJets;
    std::vector<reco::PFJet> ak4PFJets;
    
    edm::PCaloHitContainer endcapHitsContainer;
    edm::PCaloHitContainer barrelHitsContainer;

    const CaloSubdetectorGeometry *endcapGeometry;
    const CaloSubdetectorGeometry *barrelGeometry;//both were const at the place they were copied from
    
    //Results of digitization
    const EBDigiCollection barrelDigiCollection;
    const EEDigiCollection endcapDigiCollection;
    
    
};

class ECalHitHistogramContainer {
public:
	void InitializeHistograms (edm::Service<TFileService> file_service);
	void FillEventHistograms (EventCollections eventDataContainer);
	void DrawHistograms ();

private:
	void FillBarrelHitHistogramms (PCaloHit hit);
	bool IsInterestingBarrelHit (PCaloHit hit, EventCollections eventDataContainer);
	void FillEndcapHitHistogramms (PCaloHit hit);
	bool IsInterestingEndcapHit (PCaloHit hit, EventCollections eventDataContainer);

	edm::Service<TFileService> analyzer_file_service;
	BarrelEcalHistogramContainer EBHistCont;
	EndcapEcalHistogramContainer EEHistCont;
	GeneralEcalHistogramContainer GenHistCont;
	
	int barrelSupercrystalCount = 3000;
	std::vector<double> barrelSupercrystalEnergyEvent;
	int endcapSupercrystalCount = 137;
	std::vector<double> endcapPoszSupercrystalEnergyEvent;
	std::vector<double> endcapNegzSupercrystalEnergyEvent;
	

	template<class T> void DrawHistogram (T *hist, HistogramPrintInfo info);
	bool analyzerIdSet;
	std::string analyzerId;

};



#endif