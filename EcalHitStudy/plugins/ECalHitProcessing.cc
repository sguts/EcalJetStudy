#include "ECalHitProcessing.h"

void ECalJetInteractionProcessor::InitializeHistograms (edm::Service<TFileService> file_service, const std::string &plotFolder)
{
    folderName = plotFolder;
    
    InitializeEventAnalysisVariables();
    InitializeBarrelHistograms ();
    InitializeEndcapHistograms ();
    InitializeJetHistograms ();
    
}

int ECalJetInteractionProcessor::arrayCiEta (int ieta) {
    return ieta + ((barrelCrystaliEtaCount-1)/2);
}

int ECalJetInteractionProcessor::arraySCiEta (int ieta) {
    return ieta + ((barrelSuperCrystaliEtaCount-1)/2);
}

void ECalJetInteractionProcessor::InitializeEventAnalysisVariables () {
    //setting event analysis variables
    //Barrel Crystal Coords: [arrayCiEta(iEta)][iPhi]
    //Barrel Supercrystal Coords: [arraySCiEta(iEta)][iPhi]

    for (int i=0; i<barrelSuperCrystaliEtaCount; i++) {
        std::vector<double> a(barrelSuperCrystaliPhiCount,0);
        barrelSuperCrystalEnergyEvent.push_back(a);
    }
    for (int i=0; i<barrelCrystaliEtaCount; i++) {
        std::vector<double> a(barrelCrystaliPhiCount,0);
        barrelCrystalEnergyEvent.push_back(a);
    }
    for (int i=0; i<barrelSuperCrystaliEtaCount; i++) {
        std::vector<double> a(barrelSuperCrystaliPhiCount,0);
        barrelSuperCrystalEnergyTEvent.push_back(a);
    }
    for (int i=0; i<barrelCrystaliEtaCount; i++) {
        std::vector<double> a(barrelCrystaliPhiCount,0);
        barrelCrystalEnergyTEvent.push_back(a);
    }

    //Endcap SC coords: just supercrystal id
    //Endcap Crystal coords: [iX][iY]
    endcapPoszSupercrystalEnergyEvent = std::vector<double> (endcapSupercrystalCount,0);
    endcapNegzSupercrystalEnergyEvent = std::vector<double> (endcapSupercrystalCount,0);
    for (int i=0; i<endcapiXCount; i++) {
        std::vector<double> a(endcapiXCount,0);
        endcapCrystalEnergyEventPosz.push_back(a);
    }

    for (int i=0; i<endcapiXCount; i++) {
        std::vector<double> a(endcapiXCount,0);
        endcapCrystalEnergyEventNegz.push_back(a);
    }

    //Digi under construction:
    /*for(EBDigiCollection::const_iterator digiItr = (*EventCollections.barrelDigiCollection).begin();
        digiItr != (*EventCollections.barrelDigiCollection).end(); ++digiItr) {
        //digiItr->id();
    } //No, doesn't work*/
}

void ECalJetInteractionProcessor::InitializeBarrelHistograms () {
    //Barrel histograms
    double miniEta=-100,maxiEta=100,miniPhi=-10,maxiPhi=370;
    int binsiEta=(int)(maxiEta-miniEta), binsiPhi=(int)(maxiPhi-miniPhi);

    double minSuperCrystalEnergy=0.5, maxSuperCrystalEnergy=20;
    int binsSuperCrystalEnergy=100;
        
    EBHistCont.energyiPhiiEta  = analyzer_file_service -> make <TH2D> ("energyiPhiiEta","Total E deposited at given coordinates for EB",binsiPhi,miniPhi,maxiPhi,binsiEta,miniEta,maxiEta);
    EBHistCont.energyTiPhiiEta  = analyzer_file_service -> make <TH2D> ("energyTiPhiiEta","Total E_T deposited at given coordinates for EB",binsiPhi,miniPhi,maxiPhi,binsiEta,miniEta,maxiEta);
    EBHistCont.energyiPhiiEta->GetXaxis()->SetTitle("iPhi");
    EBHistCont.energyiPhiiEta->GetYaxis()->SetTitle("iEta");
    EBHistCont.energyTiPhiiEta->GetXaxis()->SetTitle("iPhi");
    EBHistCont.energyTiPhiiEta->GetYaxis()->SetTitle("iEta");

    EBHistCont.energyTToweriPhiiEta = analyzer_file_service -> make <TH2D> ("energyTToweriPhiiEta","Total E deposited at given tower coordinates for EB",binsiPhi/5,miniPhi/5,maxiPhi/5,binsiEta/5,miniEta/5,maxiEta/5);
    EBHistCont.energyTToweriPhiiEta->GetXaxis()->SetTitle("iPhi");
    EBHistCont.energyTToweriPhiiEta->GetYaxis()->SetTitle("iEta");

    EBHistCont.towerEnergyHitNumber = analyzer_file_service -> make <TH1D> ("towerEnergyHitNumber","Energy of towers for barrel", binsSuperCrystalEnergy, minSuperCrystalEnergy, maxSuperCrystalEnergy);
    EBHistCont.towerEnergyTHitNumber = analyzer_file_service -> make <TH1D> ("towerEnergyTHitNumber","E_T of towers for barrel", binsSuperCrystalEnergy, minSuperCrystalEnergy, maxSuperCrystalEnergy);
    
}

void ECalJetInteractionProcessor::InitializeEndcapHistograms () {
    //Endcap histograms
    
    double minCrystalEnergy=0.05, maxCrystalEnergy=2;
    int binsEnergy=500;

    double minSuperCrystalEnergy=0.5, maxSuperCrystalEnergy=20;
    int binsSuperCrystalEnergy=100;

    double miniX=0, maxiX=100;
    int binsiX = 100;
    //Initializing histograms in container
    EEHistCont.energyTotalHitNumber = analyzer_file_service -> make <TH1D> ("energyTotalHitNumber","Energy of individual hits", binsEnergy, minCrystalEnergy, maxCrystalEnergy);
    EEHistCont.energyTotalHitNumber->GetXaxis()->SetTitle("E, GeV");


    EEHistCont.XYTotalEnergyDepositionPosZ = analyzer_file_service -> make <TH2D> ("XYTotalEnergyDepositionPosZ","Total energy deposited at given coordinates for EE+",binsiX,miniX,maxiX,binsiX,miniX,maxiX);
    EEHistCont.XYTotalHitsNumberPosZ = analyzer_file_service -> make <TH2D> ("XYTotalHitsNumberPosZ","Total number of hits at given coordinates for EE+",binsiX,miniX,maxiX,binsiX,miniX,maxiX);

    EEHistCont.XYTotalEnergyDepositionPosZ->GetXaxis()->SetTitle("iX");
    EEHistCont.XYTotalEnergyDepositionPosZ->GetYaxis()->SetTitle("iY");

    EEHistCont.XYTotalEnergyDepositionNegZ = analyzer_file_service -> make <TH2D> ("XYTotalEnergyDepositionNegZ","Total energy deposited at given coordinates for EE-",binsiX,miniX,maxiX,binsiX,miniX,maxiX);
    EEHistCont.XYTotalHitsNumberNegZ = analyzer_file_service -> make <TH2D> ("XYTotalHitsNumberNegZ","Total number of hits at given coordinates for EE-",binsiX,miniX,maxiX,binsiX,miniX,maxiX);

    EEHistCont.XYTotalEnergyDepositionNegZ->GetXaxis()->SetTitle("iX");
    EEHistCont.XYTotalEnergyDepositionNegZ->GetYaxis()->SetTitle("iY");


    //maybe tune limits?
    EEHistCont.supercrystalPoszEnergyHitNumber = analyzer_file_service -> make <TH1D> ("supercrystalEnergyHitNumberPosz","Energy of supercrystals EE+", binsSuperCrystalEnergy, minSuperCrystalEnergy, maxSuperCrystalEnergy);
    EEHistCont.supercrystalNegzEnergyHitNumber = analyzer_file_service -> make <TH1D> ("supercrystalEnergyHitNumberNegz","Energy of supercrystals EE-", binsSuperCrystalEnergy, minSuperCrystalEnergy, maxSuperCrystalEnergy);
}

void ECalJetInteractionProcessor::InitializeJetHistograms (){
    double minJetsNum=-0.5, maxJetsNum=50.5;
    int binsJetsNum=(int)(maxJetsNum-minJetsNum);

    double minJetsPt=0, maxJetsPt=25;
    int binsJetsPt=50;
    double minJetsEnergy=0, maxJetsEnergy=50;
    int binsJetsEnergy=100;

    GenHistCont.genJetsNumEvents = analyzer_file_service -> make <TH1D> ("genJetsNumEvents","Number of genJets", binsJetsNum, minJetsNum, maxJetsNum);
    GenHistCont.genJetsPt = analyzer_file_service -> make <TH1D> ("genJetsPt","p_T of genJets", binsJetsPt, minJetsPt, maxJetsPt);
    GenHistCont.genJetsEnergy = analyzer_file_service -> make <TH1D> ("genJetsEnergy","Energy of genJets", binsJetsEnergy, minJetsEnergy, maxJetsEnergy);

    GenHistCont.ak4PFJetsNumEvents = analyzer_file_service -> make <TH1D> ("ak4PFJetsNumEvents","Number of ak4PFJets", binsJetsNum, minJetsNum, maxJetsNum);
    GenHistCont.ak4PFJetsPt = analyzer_file_service -> make <TH1D> ("ak4PFJetsPt","p_T of ak4PFJets", binsJetsPt, minJetsPt, maxJetsPt);
    GenHistCont.ak4PFJetsEnergy = analyzer_file_service -> make <TH1D> ("ak4PFJetsEnergy","Energy of ak4PFJets", binsJetsEnergy, minJetsEnergy, maxJetsEnergy);

    GenHistCont.genak4PFJetsNumEvents = analyzer_file_service -> make <TH2D> ("genak4PFJetsNumEvents","Number of genJets vs number of ak4PFJets", binsJetsNum, minJetsNum, maxJetsNum, binsJetsNum, minJetsNum, maxJetsNum);
    GenHistCont.genak4PFJetsNumEvents->GetXaxis()->SetTitle("genJets");
    GenHistCont.genak4PFJetsNumEvents->GetYaxis()->SetTitle("ak4PFJets");
}

void ECalJetInteractionProcessor::FillEventHistograms (const EventCollections &eventDataContainer){
    FillEventData (eventDataContainer);
    
    //Fill hit histograms under reconstruction
    //std::cout << "Barrel fill enter" << std::endl;
    FillBarrelHitHistogramms (eventDataContainer);
    //std::cout << "Endcap fill enter" << std::endl;
    FillEndcapHitHistogramms (eventDataContainer);

    //std::cout << "Going to jets" << std::endl;
    for (auto &currentGenJet:eventDataContainer.genJets) {
        GenHistCont.genJetsPt->Fill(currentGenJet.pt());
        GenHistCont.genJetsEnergy->Fill(currentGenJet.energy());
    }
    for (auto &currentak4PFJet:eventDataContainer.ak4PFJets) {
        GenHistCont.ak4PFJetsPt->Fill(currentak4PFJet.pt());
        GenHistCont.ak4PFJetsEnergy->Fill(currentak4PFJet.energy());
    }   
    
    //Fill over-event histograms
    GenHistCont.genJetsNumEvents -> Fill (eventDataContainer.genJets.size());
    GenHistCont.ak4PFJetsNumEvents ->Fill (eventDataContainer.ak4PFJets.size());
    GenHistCont.genak4PFJetsNumEvents -> Fill (eventDataContainer.genJets.size(),eventDataContainer.ak4PFJets.size());    
    //std::cout << "Filling for this event complete" << std::endl;
}

void ECalJetInteractionProcessor::FillEventData (const EventCollections &eventDataContainer) {
    //Resetting event storage
    //std::cout << "reset" << std::endl;
    for (auto &currentiPhiList : barrelCrystalEnergyEvent) {
        std::fill (currentiPhiList.begin(),currentiPhiList.end(),0);
    }
    for (auto &currentiPhiList : barrelSuperCrystalEnergyEvent) {
        std::fill (currentiPhiList.begin(),currentiPhiList.end(),0);
    }
    for (auto &currentiPhiList : barrelCrystalEnergyTEvent) {
        std::fill (currentiPhiList.begin(),currentiPhiList.end(),0);
    }
    for (auto &currentiPhiList : barrelSuperCrystalEnergyTEvent) {
        std::fill (currentiPhiList.begin(),currentiPhiList.end(),0);
    }
    
    for (auto &currentiYList : endcapCrystalEnergyEventPosz) {
        std::fill (currentiYList.begin(),currentiYList.end(),0);
    }
    for (auto &currentiYList : endcapCrystalEnergyEventNegz) {
        std::fill (currentiYList.begin(),currentiYList.end(),0);
    }
    std::fill (endcapPoszSupercrystalEnergyEvent.begin(),endcapPoszSupercrystalEnergyEvent.end(),0);
    std::fill (endcapNegzSupercrystalEnergyEvent.begin(),endcapNegzSupercrystalEnergyEvent.end(),0);
    
    
    
    //Filling event storage
    
    
    //std::cout << "barrel enter" << std::endl;
    //loop over barrel hits
    for (auto &currentHit : eventDataContainer.barrelHitsContainer) {
        FillHitInfo(currentHit,true,eventDataContainer);
        
    }
    //std::cout << "endcap enter" << std::endl;
    //loop over endcap hits
    for (auto &currentHit : eventDataContainer.endcapHitsContainer) {
       FillHitInfo(currentHit,false,eventDataContainer);
        
        
    }
    //std::cout << "FillEventData exit" << std::endl;
}

void ECalJetInteractionProcessor::FillHitInfo (const PCaloHit &currentHit, bool isBarrel, const EventCollections &eventDataContainer) {
    if (isBarrel) {
        double eta;
        double energy;
        double energyT;
        EBDetId infoHitBarrel (currentHit.id());
        eta = eventDataContainer.barrelGeometry->getGeometry(currentHit.id())->getPosition().eta();
        energy = currentHit.energy();
        energyT = currentHit.energy()/cosh(eta);
        
        //Barrel Crystal Coords: [arrayCiEta(iEta)][iPhi]
        //Barrel Supercrystal Coords: [arraySCiEta(iEta)][iPhi]
        //std::cout << "crystal coordinates are "<< infoHitBarrel.ieta() << " " << arrayCiEta(infoHitBarrel.ieta())<< " " << infoHitBarrel.iphi() << std::endl;
        barrelCrystalEnergyEvent[arrayCiEta(infoHitBarrel.ieta())][infoHitBarrel.iphi()]=energy;
        barrelCrystalEnergyTEvent[arrayCiEta(infoHitBarrel.ieta())][infoHitBarrel.iphi()]=energyT;
        
        //std::cout << "supercrystal coordinates are "<< infoHitBarrel.tower().ieta() << " " << arraySCiEta(infoHitBarrel.tower().ieta())<< " " << infoHitBarrel.tower().iphi() << std::endl;
        barrelSuperCrystalEnergyEvent[arraySCiEta(infoHitBarrel.tower().ieta())][infoHitBarrel.tower().iphi()]+=energy;
        barrelSuperCrystalEnergyTEvent[arraySCiEta(infoHitBarrel.tower().ieta())][infoHitBarrel.tower().iphi()]+=energyT;
    }else {
        double energy;
        EEDetId infoHitEndcap (currentHit.id());
        energy = currentHit.energy();
        
        //Endcap SC coords: just supercrystal id
        //Endcap Crystal coords: [iX][iY]
        
        
        if (infoHitEndcap.positiveZ()) {
            //std::cout << "Z+ coords: "<< infoHitEndcap.ix() << " " << infoHitEndcap.iy()  << std::endl;
            endcapCrystalEnergyEventPosz[infoHitEndcap.ix()][infoHitEndcap.iy()] = energy;
            endcapPoszSupercrystalEnergyEvent[infoHitEndcap.isc()]+=energy;

        }
        else {
            //std::cout << "Z- coords: "<< infoHitEndcap.ix() << " " << infoHitEndcap.iy()  << std::endl;
            endcapCrystalEnergyEventNegz[infoHitEndcap.ix()][infoHitEndcap.iy()] = energy;
            endcapNegzSupercrystalEnergyEvent[infoHitEndcap.isc()]+=energy;
        }
    }
}


void ECalJetInteractionProcessor::FillBarrelHitHistogramms (const EventCollections &eventDataContainer){
    //EBDetId a (ieta,iphi);
    //double eta = eventDataContainer.barrelGeometry->getGeometry(a.rawId)->getPosition().eta();
    
    for (int ieta = barrelCrystalMiniEta; arrayCiEta(ieta) < barrelCrystaliEtaCount; ieta++) {
        for (int iphi=0; iphi < barrelCrystaliPhiCount; iphi++) {
            //std::cout << "barrel crystal coordinates are " << ieta << " " <<  arrayCiEta(ieta) << " " << iphi << std::endl;
            if (IsInterestingBarrelHit(ieta,iphi)) {
                EBHistCont.energyiPhiiEta->Fill(iphi,ieta,barrelCrystalEnergyEvent[arrayCiEta(ieta)][iphi]);
                EBHistCont.energyTiPhiiEta->Fill(iphi,ieta,barrelCrystalEnergyTEvent[arrayCiEta(ieta)][iphi]);
            }
        }
    }
}


void ECalJetInteractionProcessor::FillEndcapHitHistogramms (const EventCollections &eventDataContainer){
    for (int ix=0; ix<endcapiXCount; ix++) {
        for (int iy=0; iy<endcapiXCount; iy++) {
            if (IsInterestingEndcapHit(ix,iy,true)) {
                EEHistCont.energyTotalHitNumber->Fill(endcapCrystalEnergyEventPosz[ix][iy]);
                EEHistCont.XYTotalHitsNumberPosZ->Fill(ix,iy);
                EEHistCont.XYTotalEnergyDepositionPosZ->Fill(ix,iy,endcapCrystalEnergyEventPosz[ix][iy]);
            }
            if (IsInterestingEndcapHit(ix,iy,false)) {
                EEHistCont.energyTotalHitNumber->Fill(endcapCrystalEnergyEventNegz[ix][iy]);
                EEHistCont.XYTotalHitsNumberNegZ->Fill(ix,iy);
                EEHistCont.XYTotalEnergyDepositionNegZ->Fill(ix,iy,endcapCrystalEnergyEventNegz[ix][iy]);
            }
        }
    }

}

bool ECalJetInteractionProcessor::IsInterestingBarrelHit (int ieta, int iphi){
    if (barrelCrystalEnergyEvent[arrayCiEta(ieta)][iphi]>minCrystalInterestingEnergy) return true; 
    return false;
}

bool ECalJetInteractionProcessor::IsInterestingEndcapHit (int ix, int iy, bool Posz){
    if (Posz && endcapCrystalEnergyEventPosz[ix][iy]>minCrystalInterestingEnergy) return true; 
    if (!Posz && endcapCrystalEnergyEventNegz[ix][iy]>minCrystalInterestingEnergy) return true;
    return false;
}

HistogramPrintInfo::HistogramPrintInfo ( double inputScaleValue, std::string customHistName, std::string customXAxisName, std::string customYAxisName){
    scaleValue = inputScaleValue;
    logYAxis=false;
}

template<class T> void ECalJetInteractionProcessor::DrawHistogram (T *hist, HistogramPrintInfo info){
    //int canvasX = 750, canvasY =700;
    //TLegend* leg;
    std::string fileName(folderName+"/"+hist->GetName()+".png");
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



void ECalJetInteractionProcessor::DrawHistograms (){
      /*
    HistogramPrintInfo HistInfo (1);

    HistInfo.logYAxis=true;

    DrawHistogram<TH1D>(EEHistCont.energyTotalHitNumber, HistInfo);
    DrawHistogram<TH1D>(EEHistCont.supercrystalPoszEnergyHitNumber,HistInfo);
    DrawHistogram<TH1D>(EEHistCont.supercrystalNegzEnergyHitNumber,HistInfo);

    DrawHistogram<TH1D>(EBHistCont.towerEnergyHitNumber,HistInfo);
    DrawHistogram<TH1D>(EBHistCont.towerEnergyTHitNumber,HistInfo);
    

    HistInfo.logYAxis=false;

    DrawHistogram<TH2D>(EEHistCont.XYTotalEnergyDepositionPosZ, HistInfo);
    DrawHistogram<TH2D>(EEHistCont.XYTotalEnergyDepositionNegZ, HistInfo);
    DrawHistogram<TH2D>(EEHistCont.XYTotalHitsNumberPosZ, HistInfo);
    DrawHistogram<TH2D>(EEHistCont.XYTotalHitsNumberNegZ, HistInfo);




    DrawHistogram<TH1D>(GenHistCont.genJetsNumEvents,HistInfo);
    DrawHistogram<TH1D>(GenHistCont.genJetsPt,HistInfo);
    DrawHistogram<TH1D>(GenHistCont.genJetsEnergy,HistInfo);

    DrawHistogram<TH1D>(GenHistCont.ak4PFJetsNumEvents,HistInfo);
    DrawHistogram<TH1D>(GenHistCont.ak4PFJetsPt,HistInfo);
    DrawHistogram<TH1D>(GenHistCont.ak4PFJetsEnergy,HistInfo);

    DrawHistogram<TH2D>(GenHistCont.genak4PFJetsNumEvents,HistInfo);


    DrawHistogram<TH2D>(EBHistCont.energyiPhiiEta,HistInfo);
    DrawHistogram<TH2D>(EBHistCont.energyTiPhiiEta,HistInfo);
    DrawHistogram<TH2D>(EBHistCont.energyTToweriPhiiEta,HistInfo);
    */



}

//Trigger towers: is 
//int EcalElectronicsId::channelId    (      )    const
// possibly the thing we want?
