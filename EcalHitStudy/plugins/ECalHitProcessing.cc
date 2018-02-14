#include "ECalHitProcessing.h"


void ECalHitHistogramContainer::InitializeHistograms (edm::Service<TFileService> input_file_service){
	analyzerIdSet = false;
	
	endcapPoszSupercrystalEnergyEvent = std::vector<double> (2*endcapSupercrystalCount,0);
	endcapNegzSupercrystalEnergyEvent = std::vector<double> (2*endcapSupercrystalCount,0);
	barrelSupercrystalEnergyEvent = std::vector<double> (2*barrelSupercrystalCount,0);
	

	double minCrystalEnergy=0.05, maxCrystalEnergy=2;
	int binsEnergy=500;
	double minRadius=0, maxRadius=60;
	int binsRadius = 120;
	double miniX=0, maxiX=100;
	int binsiX = 100;
	//Initializing histograms in container
	EEHistCont.energyTotalHitNumber = analyzer_file_service -> make <TH1D> ("energyTotalHitNumber","Energy of individual hits", binsEnergy, minCrystalEnergy, maxCrystalEnergy);
	EEHistCont.energyTotalHitNumber->GetXaxis()->SetTitle("E, GeV");

	EEHistCont.radiusTotalEnergyDeposition = analyzer_file_service -> make <TH1D> ("radiusTotalEnergyDeposition", "Total energy deposited at given radius", binsRadius, minRadius, maxRadius);
	EEHistCont.radiusTotalHitNumber = analyzer_file_service -> make <TH1D> ("radiusTotalHitNumber", "Total number of hits at given radius", binsRadius, minRadius, maxRadius);

	EEHistCont.XYTotalEnergyDepositionPosZ = analyzer_file_service -> make <TH2D> ("XYTotalEnergyDepositionPosZ","Total energy deposited at given coordinates for EE+",binsiX,miniX,maxiX,binsiX,miniX,maxiX);
	EEHistCont.XYTotalHitsNumberPosZ = analyzer_file_service -> make <TH2D> ("XYTotalHitsNumberPosZ","Total number of hits at given coordinates for EE+",binsiX,miniX,maxiX,binsiX,miniX,maxiX);

	EEHistCont.XYTotalEnergyDepositionPosZ->GetXaxis()->SetTitle("iX");
	EEHistCont.XYTotalEnergyDepositionPosZ->GetYaxis()->SetTitle("iY");

	EEHistCont.XYTotalEnergyDepositionNegZ = analyzer_file_service -> make <TH2D> ("XYTotalEnergyDepositionNegZ","Total energy deposited at given coordinates for EE-",binsiX,miniX,maxiX,binsiX,miniX,maxiX);
	EEHistCont.XYTotalHitsNumberNegZ = analyzer_file_service -> make <TH2D> ("XYTotalHitsNumberNegZ","Total number of hits at given coordinates for EE-",binsiX,miniX,maxiX,binsiX,miniX,maxiX);



	EEHistCont.eta1eta2 = analyzer_file_service -> make <TH2D> ("eta1eta2","Etas of both gen particles",60,-3,3,60,-3,-3);
	EEHistCont.isEEPlus = analyzer_file_service -> make <TH1D> ("isEEPlus", "Is positive z for each hit", 2,-0.5,1.5);


	double minJetsNum=-0.5, maxJetsNum=100.5;
	int binsJetsNum=101;

	double minJetsPt=0, maxJetsPt=25;
	int binsJetsPt=50;
	double minJetsEnergy=0, maxJetsEnergy=50;
	int binsJetsEnergy=100;

	EEHistCont.genJetsNumEvents = analyzer_file_service -> make <TH1D> ("genJetsNumEvents","Number of genJets", binsJetsNum, minJetsNum, maxJetsNum);
	EEHistCont.genJetsPt = analyzer_file_service -> make <TH1D> ("genJetsPt","p_T of genJets", binsJetsPt, minJetsPt, maxJetsPt);
	EEHistCont.genJetsEnergy = analyzer_file_service -> make <TH1D> ("genJets","Energy of genJets", binsJetsEnergy, minJetsEnergy, maxJetsEnergy);

	EEHistCont.ak4PFJetsNumEvents = analyzer_file_service -> make <TH1D> ("ak4PFJetsNumEvents","Number of ak4PFJets", binsJetsNum, minJetsNum, maxJetsNum);
	EEHistCont.ak4PFJetsPt = analyzer_file_service -> make <TH1D> ("ak4PFJetsPt","p_T of ak4PFJets", binsJetsPt, minJetsPt, maxJetsPt);

	/*double minEndcapSupercrystalNumber=0.5, maxEndcapSupercrystalNumber=(double)(2*endcapSupercrystalCount)+0.5;
	int binsEndcapSupercrystalNumber=endcapSupercrystalCount*2;*/
        //under reconstruction
        //doubled to check enumeration for EE+-

        //maybe tune limits?
	EEHistCont.supercrystalPoszEnergyHitNumber = analyzer_file_service -> make <TH1D> ("supercrystalEnergyHitNumberPosz","Energy of supercrystals EE+", binsEnergy, minCrystalEnergy, maxCrystalEnergy);
	EEHistCont.supercrystalNegzEnergyHitNumber = analyzer_file_service -> make <TH1D> ("supercrystalEnergyHitNumberNegz","Energy of supercrystals EE-", binsEnergy, minCrystalEnergy, maxCrystalEnergy);
	


}

void ECalHitHistogramContainer::FillEventHistograms (EventCollections eventDataContainer){
	std::fill (endcapPoszSupercrystalEnergyEvent.begin(),endcapPoszSupercrystalEnergyEvent.end(),0);
	std::fill (endcapNegzSupercrystalEnergyEvent.begin(),endcapNegzSupercrystalEnergyEvent.end(),0);
        
        /*for(EBDigiCollection::const_iterator digiItr = (*EventCollections.barrelDigiCollection).begin();
                digiItr != (*EventCollections.barrelDigiCollection).end(); ++digiItr) {
            //digiItr->id();
        } //No, doesn't work*/
        if (!analyzerIdSet && eventDataContainer.genParticles.size()<= 2) {
                int roundedpt = (int)(eventDataContainer.genParticles[0].pt()+0.5);
                analyzerId = "pdgId";
                analyzerId += std::to_string(eventDataContainer.genParticles[0].pdgId());
                analyzerId += "pt";
                analyzerId += std::to_string(roundedpt);
                std::cout << "roundedpt = " << roundedpt << " analyzerId " << analyzerId <<std::endl;
                std::cout << "\n\n--------------------------------\n\n Number of Gen Particles " << 
                    eventDataContainer.genParticles.size() << "\n Number of PCaloHits " << 
                    eventDataContainer.endcapHitsContainer.size() << "\n p_T of particle 1 is " << 
                    eventDataContainer.genParticles[0].pt() << "\n p_T of particle 2 is " << 
                    eventDataContainer.genParticles[1].pt() << "\n pdgId of particle 1 is " << 
                    eventDataContainer.genParticles[0].pdgId() << "\n pdgId of particle 2 is " << 
                    eventDataContainer.genParticles[1].pdgId() << "\n\n -------------------------------- \n\n " << std::endl;
                analyzerIdSet = true;
        } else if (!analyzerIdSet){
		analyzerId += "JetPy8";
		analyzerIdSet = true;
	} 
	if (eventDataContainer.genParticles.size()>1) {
		EEHistCont.eta1eta2 -> Fill (eventDataContainer.genParticles[0].eta(),eventDataContainer.genParticles[1].eta());
	}
	//loop over barrel hits
	for (auto &currentHit : eventDataContainer.barrelHitsContainer) {
		if (IsInterestingBarrelHit(currentHit, eventDataContainer)) {
			FillBarrelHitHistogramms (currentHit);
		}
	}
	//loop over endcap hits
	for (auto &currentHit : eventDataContainer.endcapHitsContainer) {
		if (IsInterestingEndcapHit(currentHit, eventDataContainer)) {
			FillEndcapHitHistogramms (currentHit);
		}
	}
	//Fill over-event histograms
	EEHistCont.genJetsNumEvents -> Fill (eventDataContainer.genJets.size());
	EEHistCont.ak4PFJetsNumEvents ->Fill (eventDataContainer.ak4PFJets.size());
	for (auto &currentGenJet:eventDataContainer.genJets) {
	  EEHistCont.genJetsPt->Fill(currentGenJet.pt());
	  EEHistCont.genJetsEnergy->Fill(currentGenJet.energy());
	}
	for (auto &currentak4PFJet:eventDataContainer.ak4PFJets) {
	  EEHistCont.ak4PFJetsPt->Fill(currentak4PFJet.pt());
	}
	/*for (auto &currentSupercrystalEnergy:supercrystalEnergyEvent){
		EEHistCont.supercrystalEnergyHitNumber->Fill(currentSupercrystalEnergy);
	}*/
	for (auto &currentEndcapPoszSupercrystalEnergy:endcapPoszSupercrystalEnergyEvent){
            EEHistCont.supercrystalPoszEnergyHitNumber->Fill(currentEndcapPoszSupercrystalEnergy);
	}
	for (auto &currentEndcapPoszSupercrystalEnergy:endcapPoszSupercrystalEnergyEvent){
            EEHistCont.supercrystalPoszEnergyHitNumber->Fill(currentEndcapPoszSupercrystalEnergy);
        }


	//std::cout << "Positive energy is " << totalEnergyPos << " Negative energy is " << totalEnergyNeg << std::endl;

}

void ECalHitHistogramContainer::FillBarrelHitHistogramms (PCaloHit hit){

}


void ECalHitHistogramContainer::FillEndcapHitHistogramms (PCaloHit hit){
	//Geting info from the hit
	EndcapEcalCrystalHit hitInfo;

	EEDetId iCoordClass(hit.id());

	hitInfo.posz = iCoordClass.positiveZ();
	hitInfo.ix = iCoordClass.ix();
	hitInfo.iy = iCoordClass.iy();

	hitInfo.isc = iCoordClass.isc();

	hitInfo.ir = sqrt(pow(hitInfo.ix-50,2)+pow(hitInfo.iy-50,2));
	hitInfo.energy = hit.energy();
	hitInfo.depth = hit.depth();

	//Filling supercrystal energy

	if (hitInfo.posz) {endcapPoszSupercrystalEnergyEvent[hitInfo.isc] += hitInfo.energy;}
	else endcapNegzSupercrystalEnergyEvent[hitInfo.isc] += hitInfo.energy;

	//Filling histograms
	EEHistCont.energyTotalHitNumber->Fill(hitInfo.energy);

	EEHistCont.radiusTotalHitNumber->Fill(hitInfo.ir);
	EEHistCont.radiusTotalEnergyDeposition->Fill(hitInfo.ir, hitInfo.energy);

	//TODO: remake initialization and drawing, split supercrystals between EE+ and EE-
	//EEHistCont.supercrystalHitNumber->Fill(hitInfo.isc);


	if (hitInfo.posz) {
		EEHistCont.XYTotalHitsNumberPosZ->Fill(hitInfo.ix,hitInfo.iy);
		EEHistCont.XYTotalEnergyDepositionPosZ->Fill(hitInfo.ix,hitInfo.iy,hitInfo.energy);
		EEHistCont.isEEPlus->Fill(1);
	} else {
		EEHistCont.XYTotalHitsNumberNegZ->Fill(hitInfo.ix,hitInfo.iy);
		EEHistCont.XYTotalEnergyDepositionNegZ->Fill(hitInfo.ix,hitInfo.iy,hitInfo.energy);
		EEHistCont.isEEPlus->Fill(0);
	}

}

bool ECalHitHistogramContainer::IsInterestingBarrelHit (PCaloHit hit, EventCollections eventDataContainer){
        return true;
}

bool ECalHitHistogramContainer::IsInterestingEndcapHit (PCaloHit hit, EventCollections eventDataContainer){
	return true;
}

HistogramPrintInfo::HistogramPrintInfo (std::string inputAnalyzerIdentificator, double inputScaleValue, std::string customHistName, std::string customXAxisName, std::string customYAxisName){
	analyzerIdentificator = inputAnalyzerIdentificator;
	scaleValue = inputScaleValue;
	logYAxis=false;
}

template<class T> void ECalHitHistogramContainer::DrawHistogram (T *hist, HistogramPrintInfo info){
	//int canvasX = 750, canvasY =700;
	//TLegend* leg;
	std::string fileName("/home/guts/CMS/CMSSW_8_0_21/src/ParticleGunStudy/StudyFileCollection/"+info.analyzerIdentificator+"/"+hist->GetName()+".png");
	TCanvas *canvasCurrent = new TCanvas((std::string("Canvas for ")+std::string(hist->GetTitle())).c_str(),"Title",1400,1000);
  	canvasCurrent->cd();
  	gStyle->SetOptStat(0);
  	if (info.logYAxis) gPad->SetLogy();

  
  	hist->SetLineWidth(2);
  	hist->SetLineColor(2);
  	//h1->SetTitle(canvasTitle.c_str());
  	//h1->GetXaxis()->SetTitle("p_T, GeV, 1/4 GeV bins");
  	hist->GetYaxis()->SetTitle("");
  	hist->Scale(info.scaleValue);
  
  	hist->Draw("colz");
  
  	canvasCurrent->Update();

  	std::cout << "Saving file " << fileName << std::endl;

  
  	canvasCurrent->SaveAs(fileName.c_str(),"recreate");
}



void ECalHitHistogramContainer::DrawHistograms (){
	HistogramPrintInfo HistInfo (analyzerId, 1);
	std::cout << HistInfo.scaleValue << std::endl;

	HistInfo.logYAxis=true;

	DrawHistogram<TH1D>(EEHistCont.energyTotalHitNumber, HistInfo);
	DrawHistogram<TH1D>(EEHistCont.supercrystalPoszEnergyHitNumber,HistInfo);
        DrawHistogram<TH1D>(EEHistCont.supercrystalNegzEnergyHitNumber,HistInfo);

	HistInfo.logYAxis=false;

	DrawHistogram<TH2D>(EEHistCont.XYTotalEnergyDepositionPosZ, HistInfo);
	DrawHistogram<TH2D>(EEHistCont.XYTotalEnergyDepositionNegZ, HistInfo);
	DrawHistogram<TH2D>(EEHistCont.XYTotalHitsNumberPosZ, HistInfo);
	DrawHistogram<TH2D>(EEHistCont.XYTotalHitsNumberNegZ, HistInfo);

	DrawHistogram<TH2D>(EEHistCont.eta1eta2, HistInfo);
	DrawHistogram<TH1D>(EEHistCont.isEEPlus, HistInfo);


	DrawHistogram<TH1D>(EEHistCont.genJetsNumEvents,HistInfo);
	DrawHistogram<TH1D>(EEHistCont.genJetsPt,HistInfo);
	DrawHistogram<TH1D>(EEHistCont.genJetsEnergy,HistInfo);

	DrawHistogram<TH1D>(EEHistCont.ak4PFJetsNumEvents,HistInfo);
	DrawHistogram<TH1D>(EEHistCont.ak4PFJetsPt,HistInfo);




}

//Trigger towers: is 
//int EcalElectronicsId::channelId	(		)	const
// possibly the thing we want?
