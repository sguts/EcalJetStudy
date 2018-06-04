#include "ECalHitProcessing.h"

void ECalJetInteractionProcessor::InitializeHistograms () {   
    InitializeEventAnalysisVariables();
    InitializeBarrelHistograms ();
    InitializeEndcapHistograms ();
    InitializeJetHistograms ();
    InitializeJetSplitHistograms ();

    std::cout << "Initialization complete" << std::endl;
    
}

int ECalJetInteractionProcessor::arrayCiPhi (int iphi) {
    //if (iphi>=barrelCrystaliPhiCount) return iphi-barrelCrystaliPhiCount;
    //if (iphi<0) return iphi+barrelCrystaliPhiCount;
    return iphi-1;
}

int ECalJetInteractionProcessor::arraySCiPhi (int iphi) {
    return iphi-1;
}

int ECalJetInteractionProcessor::arrayCiEta (int ieta) {
    return ieta + ((barrelCrystaliEtaCount-1)/2);
}

int ECalJetInteractionProcessor::arraySCiEta (int ieta) {
    return ieta + ((barrelSuperCrystaliEtaCount-1)/2);
}



void ECalJetInteractionProcessor::InitializeEventAnalysisVariables () {
    //setting event analysis variables
    for (double i=0; i<=20; i++) {
        listConeDR.push_back(i*0.05);
    }
    
    
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
    edm::Service<TFileService> analyzer_file_service;
    
    TFileDirectory mainSubDir = analyzer_file_service -> mkdir( "BarrelHists" );
    
    
    double miniEta=-100,maxiEta=100,miniPhi=-10,maxiPhi=370;
    int binsiEta=(int)(maxiEta-miniEta), binsiPhi=(int)(maxiPhi-miniPhi);
    
        
    EBHistCont.energyiPhiiEta  = mainSubDir.make <TH2D> ("energyiPhiiEta","Total E deposited at given coordinates for EB",binsiPhi,miniPhi,maxiPhi,binsiEta,miniEta,maxiEta);
    EBHistCont.energyTiPhiiEta  = mainSubDir.make <TH2D> ("energyTiPhiiEta","Total E_T deposited at given coordinates for EB",binsiPhi,miniPhi,maxiPhi,binsiEta,miniEta,maxiEta);
    EBHistCont.energyiPhiiEta->GetXaxis()->SetTitle("iPhi");
    EBHistCont.energyiPhiiEta->GetYaxis()->SetTitle("iEta");
    EBHistCont.energyTiPhiiEta->GetXaxis()->SetTitle("iPhi");
    EBHistCont.energyTiPhiiEta->GetYaxis()->SetTitle("iEta");
    
}

void ECalJetInteractionProcessor::InitializeEndcapHistograms () {
    //Endcap histograms
    edm::Service<TFileService> analyzer_file_service;
    
    TFileDirectory mainSubDir = analyzer_file_service -> mkdir( "EndcapHists" );
    
    double minCrystalEnergy=0.05, maxCrystalEnergy=2;
    int binsCrystalEnergy=500;

    double miniX=0, maxiX=100;
    int binsiX = 100;
    

    
    //Initializing histograms in container
    EEHistCont.energyTotalHitNumber = mainSubDir.make <TH1D> ("energyTotalHitNumber","Energy of individual hits", binsCrystalEnergy, minCrystalEnergy, maxCrystalEnergy);
    EEHistCont.energyTotalHitNumber->GetXaxis()->SetTitle("E, GeV");


    EEHistCont.XYTotalEnergyDepositionPosZ = mainSubDir.make <TH2D> ("XYTotalEnergyDepositionPosZ","Total energy deposited at given coordinates for EE+",binsiX,miniX,maxiX,binsiX,miniX,maxiX);
    EEHistCont.XYTotalHitsNumberPosZ = mainSubDir.make <TH2D> ("XYTotalHitsNumberPosZ","Total number of hits at given coordinates for EE+",binsiX,miniX,maxiX,binsiX,miniX,maxiX);

    EEHistCont.XYTotalEnergyDepositionPosZ->GetXaxis()->SetTitle("iX");
    EEHistCont.XYTotalEnergyDepositionPosZ->GetYaxis()->SetTitle("iY");

    EEHistCont.XYTotalEnergyDepositionNegZ = mainSubDir.make <TH2D> ("XYTotalEnergyDepositionNegZ","Total energy deposited at given coordinates for EE-",binsiX,miniX,maxiX,binsiX,miniX,maxiX);
    EEHistCont.XYTotalHitsNumberNegZ = mainSubDir.make <TH2D> ("XYTotalHitsNumberNegZ","Total number of hits at given coordinates for EE-",binsiX,miniX,maxiX,binsiX,miniX,maxiX);

    EEHistCont.XYTotalEnergyDepositionNegZ->GetXaxis()->SetTitle("iX");
    EEHistCont.XYTotalEnergyDepositionNegZ->GetYaxis()->SetTitle("iY");
    
    

    
}

void ECalJetInteractionProcessor::InitializeJetHistograms (){
    edm::Service<TFileService> analyzer_file_service;
    
    TFileDirectory jetSubDir = analyzer_file_service -> mkdir( "JetHist" );
    
    TFileDirectory jetEDepSubDir = jetSubDir.mkdir( "JetEDep" );
    TFileDirectory jetEDep_fracE_SubDir = jetSubDir.mkdir( "JetEDep_fracE" );
    
    TFileDirectory jetSplitHistSubDir = jetSubDir.mkdir("JetSplit_eta_pt_hist");
    
    double minJetsNum=-0.5, maxJetsNum=12.5;
    int binsJetsNum=(int)(maxJetsNum-minJetsNum);

    double minJetsPt=1, maxJetsPt=251;
    int binsJetsPt=250;
    double minJetsEnergy=15, maxJetsEnergy=100;
    int binsJetsEnergy=85;
    
    double minDR=-0.01, maxDR=1.99;
    int binsDR=100;
    
    double minJetDR=0, maxJetDR=0.55;
    int binsJetDR=55;
    
    double minEcalJetsEnergy=0, maxEcalJetsEnergy=100;
    int binsEcalJetsEnergy=100;

    JetHistCont.genJetsNumEvents = jetSubDir.make <TH1D> ("genJetsNumEvents","Number of hard genJets", binsJetsNum, minJetsNum, maxJetsNum);
    JetHistCont.genJetsPt = jetSubDir.make <TH1D> ("genJetsPt","p_T of all genJets", binsJetsPt, minJetsPt, maxJetsPt);
    JetHistCont.genJetsEnergy = jetSubDir.make <TH1D> ("genJetsEnergy","Energy of hard genJets", binsJetsEnergy, minJetsEnergy, maxJetsEnergy);

    JetHistCont.PFJetsNumEvents = jetSubDir.make <TH1D> ("PFJetsNumEvents","Number of hard PFJets", binsJetsNum, minJetsNum, maxJetsNum);
    JetHistCont.PFJetsPt = jetSubDir.make <TH1D> ("PFJetsPt","p_T of hard PFJets", binsJetsPt, minJetsPt, maxJetsPt);
    JetHistCont.PFJetsEnergy = jetSubDir.make <TH1D> ("PFJetsEnergy","Energy of hard PFJets", binsJetsEnergy, minJetsEnergy, maxJetsEnergy);

    JetHistCont.interestingGenPFJetsNumEvents = jetSubDir.make <TH2D> ("interestingGenPFJetsNumEvents","Number of hard genJets vs number of hard PFJets", binsJetsNum, minJetsNum, maxJetsNum, binsJetsNum, minJetsNum, maxJetsNum);
    JetHistCont.interestingGenPFJetsNumEvents->GetXaxis()->SetTitle("genJets");
    JetHistCont.interestingGenPFJetsNumEvents->GetYaxis()->SetTitle("PFJets");
    
    JetHistCont.genGenJetsClosestDR = jetSubDir.make <TH1D> ("genGenJetsClosestDR","DR between GenJet and closest hard GenJet", binsDR, minDR, maxDR);
    JetHistCont.genPFJetsClosestDR  = jetSubDir.make <TH1D> ("genPFJetsClosestDR","DR between GenJet and closest hard PFJet", binsDR, minDR, maxDR);
    
    JetHistCont.genJetsEmE_fracE = jetSubDir.make <TH1D> ("genJetsEmE_fracE","Ratio of electromagnetic energy to total energy for genJets", 120,-0.005,1.195);
    JetHistCont.genJetsHadE_fracE = jetSubDir.make <TH1D> ("genJetsHadE_fracE","Ratio of hadronic energy to total energy for genJets", 120,-0.005,1.195);
    JetHistCont.genJetsInvE_fracE = jetSubDir.make <TH1D> ("genJetsInvE_fracE","Ratio of invisible energy to total energy for genJets", 120,-0.005,1.195);
    JetHistCont.genJetsAuxE_fracE = jetSubDir.make <TH1D> ("genJetsAuxE_fracE","Ratio of auxilary energy to total energy for genJets", 120,-0.005,1.195);
    JetHistCont.genJetsSumE_fracE = jetSubDir.make <TH1D> ("genJetsSumE_fracE","Ratio of electromagnetic, hadronic, invisible and auxilary energy to total energy for genJets", 120,-0.005,1.195);

    for (auto &coneDR : listConeDR) {
        JetHistCont.genJetsEDepEndc_Map[coneDR] = jetEDepSubDir.make <TH1D> (("genJetsEDepECAL_endcap_DR"+to_string_with_precision(coneDR,3)).c_str(),
            ("Energy deposited in endcap by jet in a DR < " + to_string_with_precision(coneDR,3) + " cone").c_str(), 
            binsEcalJetsEnergy, minEcalJetsEnergy, maxEcalJetsEnergy);
        JetHistCont.genJetsEDepEndc_Map[coneDR]->GetXaxis()->SetTitle("E, GeV");
    }
    for (auto &coneDR : listConeDR) {
        JetHistCont.genJetsEDepEndc_fracE_Map[coneDR] = jetEDep_fracE_SubDir.make <TH1D> (("genJetsEDepECAL_endcap_DR"+to_string_with_precision(coneDR,3)+"_fracE").c_str(),("Ratio of energy deposited in ECAL endcap in a DR < "+to_string_with_precision(coneDR,3)+" cone to total energy").c_str(), 120,-0.005,1.195);
    }
    for (auto &coneDR : listConeDR) {
        JetHistCont.genJetsEDepBar_Map[coneDR] = jetEDepSubDir.make <TH1D> (("genJetsEDepECAL_barrel_DR"+to_string_with_precision(coneDR,3)).c_str(),
            ("Energy deposited in barrel by jet in a DR < " + to_string_with_precision(coneDR,3) + " cone").c_str(), 
            binsEcalJetsEnergy, minEcalJetsEnergy, maxEcalJetsEnergy);
        JetHistCont.genJetsEDepBar_Map[coneDR]->GetXaxis()->SetTitle("E, GeV");
    }
    for (auto &coneDR : listConeDR) {
        JetHistCont.genJetsEDepBar_fracE_Map[coneDR] = jetEDep_fracE_SubDir.make <TH1D> (("genJetsEDepECAL_barrel_DR"+to_string_with_precision(coneDR,3)+"_fracE").c_str(),("Ratio of energy deposited in ECAL barrel in a DR < "+to_string_with_precision(coneDR,3)+" cone to total energy").c_str(), 120,-0.005,1.195);
    }
    
    JetHistCont.genPartonsJetDR_pions = jetSubDir.make <TH1D> ("genPartonsJetDR_pions","Energy of pion partons as function of DR between them and their Gen Jet",binsJetDR,minJetDR,maxJetDR);
    JetHistCont.genPartonsJetDR_photons = jetSubDir.make <TH1D> ("genPartonsJetDR_photons","Energy of EM partons as function of DR between them and their Gen Jet",binsJetDR,minJetDR,maxJetDR);
    JetHistCont.genPartonsJetDR_hadrons = jetSubDir.make <TH1D> ("genPartonsJetDR_hadrons","Energy of heavy hadronic partons as function of DR between them and their Gen Jet",binsJetDR,minJetDR,maxJetDR);
    JetHistCont.genPartonsJetDR_other = jetSubDir.make <TH1D> ("genPartonsJetDR_other","Energy of other partons as function of DR between them and their Gen Jet",binsJetDR,minJetDR,maxJetDR);
    
    
    JetHistCont.genJetsAvEDepBar_fracE = jetSubDir.make <TH1D> ("genJetsAvEDepBar_fracE","Average #frac{E_{sim}^{DR}}{E_{gen}} for hard jets in barrel",listConeDR.size()-1,listConeDR.data());
    JetHistCont.genJetsAvEDepBar_fracE->Sumw2();
    
    JetHistCont.genJetsAvEDepEnd_fracE = jetSubDir.make <TH1D> ("genJetsAvEDepEnd_fracE","Average #frac{E_{sim}^{DR}}{E_{gen}} for hard jets in endcap",listConeDR.size()-1,listConeDR.data());
    JetHistCont.genJetsAvEDepEnd_fracE->Sumw2();
    
    JetHistCont.genJetsAvSoftJetE_Bar_fracE = jetSubDir.make <TH1D> ("genJetsAvSoftJetE_Bar_fracE","Average #frac{E_{soft}^{DR}}{E_{gen}} for hard jets in barrel",listConeDR.size()-1,listConeDR.data());
    
    JetHistCont.genJetsAvSoftJetE_End_fracE = jetSubDir.make <TH1D> ("genJetsAvSoftJetE_End_fracE","Average #frac{E_{soft}^{DR}}{E_{gen}} for hard jets in endcap",listConeDR.size()-1,listConeDR.data());
    
    JetHistCont.hardBarrelJetCount= jetSubDir.make <TH1D> ("hardBarrelJetCount","Number of hard isolated jets in barrel",1,0,5);
    JetHistCont.hardEndcapJetCount= jetSubDir.make <TH1D> ("hardEndcapJetCount","Number of hard isolated jets in barrel",1,0,5);
    
    double minAllJetEnergy=0, maxAllJetEnergy=200;
    int binsAllJetEnergy = 100;
    
    double minAllJetEta=-5,maxAllJetEta=5;
    int binsAllJetEta=100;
    
    JetHistCont.genJetsAllEnergyEta = jetSubDir.make <TH2D> ("genJetsAllEnergyEta","energy of all Gen Jets vs eta", binsAllJetEnergy, minAllJetEnergy, maxAllJetEnergy, binsAllJetEta, minAllJetEta, maxAllJetEta);
    JetHistCont.genJetsAllEnergyEta->GetXaxis()->SetTitle("Energy of Gen Jets");
    JetHistCont.genJetsAllEnergyEta->GetYaxis()->SetTitle("eta of Gen Jets");
    
    
}

void ECalJetInteractionProcessor::InitializeJetSplitHistograms (){
    edm::Service<TFileService> analyzer_file_service;
    
    TFileDirectory jetSplitSubDir = analyzer_file_service -> mkdir( "JetSplitHist" );
    std::vector <double> bins_Esim;
    std::vector <double> bins_EsimT;

    std::vector <double> bins_EsimDivNcr;
    std::vector <double> bins_EsimDivNcr_Dif;

    std::vector <double> bins_EsimDivEjet;
    std::vector <double> bins_EsimDivEjet_Dif;

    for (int i=0; i<=100;i++) {
        bins_Esim.push_back(i*0.02);
        //change binning for 20 MeV, max to 2 GeV
    }
    for (int i=0; i<=100;i++) {
        bins_EsimT.push_back(i*0.02);
    }
    for (int i=0; i<=50;i++) {
        bins_EsimDivNcr.push_back(i*0.01);
    }
    for (int i=0; i<=50;i++) {
        bins_EsimDivNcr_Dif.push_back(i*0.01);
    }
    for (int i=0; i<=100;i++) {
        bins_EsimDivEjet.push_back(i*0.01);
    }
    for (int i=0; i<=100;i++) {
        bins_EsimDivEjet_Dif.push_back(i*0.01);
    }

    InitializeJetSplitHistogramsArray(jetSplitSubDir, JetSplitHistCont.numCrystal_ESim_Eta_Pt_DRIncAr,bins_Esim,"numCrystal_ESim_Eta_Pt_DRIncAr","Energy of individual crystals","E, GeV");
    InitializeJetSplitHistogramsArray(jetSplitSubDir, JetSplitHistCont.numCrystal_ESimT_Eta_Pt_DRIncAr,bins_EsimT,"numCrystal_ESimT_Eta_Pt_DRIncAr","Transversal energy of individual crystals","E_{T}, GeV");

    InitializeJetSplitHistogramsArray(jetSplitSubDir, JetSplitHistCont.numJet_ESimDivNcr_Eta_Pt_DRIncAr,bins_EsimDivNcr,"numJet_ESimDivNcr_Eta_Pt_DRIncAr","Average crystal energy for jet","$frac{E_{sim}}{n}, GeV");
    
    
    InitializeJetSplitHistogramsArray(jetSplitSubDir, JetSplitHistCont.numJet_ESimDivNcr_Eta_Pt_DRDifAr,bins_EsimDivNcr_Dif,"numJet_ESimDivNcr_Eta_Pt_DRDifAr","Average crystal energy for jet","$frac{E_{sim}}{n}, GeV",false);

    InitializeJetSplitHistogramsArray(jetSplitSubDir, JetSplitHistCont.numJet_ESimDivEjet_Eta_Pt_DRIncAr,bins_EsimDivEjet,"numJet_ESimDivEjet_Eta_Pt_DRIncAr","Sim energy over Gen energy for Gen jets","#frac{E_{sim}}{E_{gen}}, GeV");
    InitializeJetSplitHistogramsArray(jetSplitSubDir, JetSplitHistCont.numJet_ESimDivEjet_Eta_Pt_DRDifAr,bins_EsimDivEjet_Dif,"numJet_ESimDivEjet_Eta_Pt_DRDifAr","Sim energy over Gen energy for Gen jets","#frac{E_{sim}}{E_{gen}}, GeV",false);

}

void ECalJetInteractionProcessor::InitializeJetSplitHistogramsArray (TFileDirectory &jetSplitSubDir, std::vector <TH3D*> &targetArray, std::vector <double> binXvar, std::string histNaming, std::string histTitling, std::string titlingXaxis, bool DRinclusive) {
    TH3D *tempHist;
    std::string histName;
    std::string histTitle;
    //We have one more thing in the DR array than we have hists.
    for (unsigned int i=1; i<JetSplitHistCont.stepsDR.size();i++) {

        histName = histNaming+std::to_string(i);
        if (DRinclusive) histTitle = histTitling + ", DR to "+to_string_with_precision(JetSplitHistCont.stepsDR.at(i),3);
        else histTitle = histTitle = histTitling + ", DR from "+to_string_with_precision(JetSplitHistCont.stepsDR.at(i-1),3)+" to "+to_string_with_precision(JetSplitHistCont.stepsDR.at(i),3);

        tempHist = jetSplitSubDir.make <TH3D> (histName.c_str(),
            histTitle.c_str(),
            binXvar.size()-1,
            binXvar.data(),
            JetSplitHistCont.etaBinning.size()-1,
            JetSplitHistCont.etaBinning.data(),
            JetSplitHistCont.ptBinning.size()-1,
            JetSplitHistCont.ptBinning.data()
        );

        tempHist->GetXaxis()->SetTitle(titlingXaxis.c_str());
        tempHist->GetYaxis()->SetTitle("#eta");
        tempHist->GetZaxis()->SetTitle("p_{T}, GeV");

        targetArray.push_back(tempHist);
    }
}

void ECalJetInteractionProcessor::FillEventHistograms (const EventCollections &eventDataContainer){;
    FillEventData (eventDataContainer);
    
    //Fill crystal histograms
    FillBarrelHitHistogramms (eventDataContainer);
    FillEndcapHitHistogramms (eventDataContainer);

    //Working with sorted Jets
    FillJetsHistograms (eventDataContainer);
    //Filling 3d histograms
    FillJetsSplitHistograms (eventDataContainer);
    
    //Fill over-event histograms

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
    
    //loop over barrel hits
    for (auto &currentHit : eventDataContainer.barrelHitsContainer) {
        FillHitInfo(currentHit,true,eventDataContainer);
        
    }
    //std::cout << "endcap enter" << std::endl;
    //loop over endcap hits
    for (auto &currentHit : eventDataContainer.endcapHitsContainer) {
       FillHitInfo(currentHit,false,eventDataContainer);
        
        
    }
    //Cleaning hard jet pointer collection
    filteredCollections.hardGenJetPointers.clear();
    filteredCollections.hardPFJetPointers.clear();
    
    //Filling hard jet pointer collection
    for (auto &currentGenJet:eventDataContainer.genJets) {
        if (IsHardGenJet(currentGenJet, eventDataContainer)) {
            filteredCollections.hardGenJetPointers.push_back(&currentGenJet);
        }
    }
    for (auto &currentPFJet:eventDataContainer.PFJets) {
        if (IsHardPFJet(currentPFJet, eventDataContainer)) {
            filteredCollections.hardPFJetPointers.push_back(&currentPFJet);
        }
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
        
        //Barrel Crystal Coords: [arrayCiEta(iEta)][arrayCiPhi(iPhi)]
        //Barrel Supercrystal Coords: [arraySCiEta(iEta)][arraySCiPhi(iPhi)]
        barrelCrystalEnergyEvent.at(arrayCiEta(infoHitBarrel.ieta())).at(arrayCiPhi(infoHitBarrel.iphi()))+=energy;
        barrelCrystalEnergyTEvent.at(arrayCiEta(infoHitBarrel.ieta())).at(arrayCiPhi(infoHitBarrel.iphi()))+=energyT;
        
        //std::cout << "supercrystal coordinates are "<< infoHitBarrel.tower().ieta() << " " << arraySCiEta(infoHitBarrel.tower().ieta())<< " " << infoHitBarrel.tower().iphi() << std::endl;
        barrelSuperCrystalEnergyEvent.at(arraySCiEta(infoHitBarrel.tower().ieta())).at(arraySCiPhi(infoHitBarrel.tower().iphi()))+=energy;
        barrelSuperCrystalEnergyTEvent.at(arraySCiEta(infoHitBarrel.tower().ieta())).at(arraySCiPhi(infoHitBarrel.tower().iphi()))+=energyT;
        
    }else {
        double energy;
        EEDetId infoHitEndcap (currentHit.id());
        energy = currentHit.energy();
        
        //Endcap SC coords: just supercrystal id
        //Endcap Crystal coords: [iX][iY]
        
        
        if (infoHitEndcap.positiveZ()) {
            //std::cout << "Z+ coords: "<< infoHitEndcap.ix() << " " << infoHitEndcap.iy()  << " " << infoHitEndcap.isc()  << std::endl;
            endcapCrystalEnergyEventPosz.at(infoHitEndcap.ix()).at(infoHitEndcap.iy()) += energy;
            endcapPoszSupercrystalEnergyEvent.at(infoHitEndcap.isc())+=energy;

        }
        else {
            //std::cout << "Z- coords: "<< infoHitEndcap.ix() << " " << infoHitEndcap.iy()  << " " << infoHitEndcap.isc()  << std::endl;
            endcapCrystalEnergyEventNegz.at(infoHitEndcap.ix()).at(infoHitEndcap.iy()) += energy;
            endcapNegzSupercrystalEnergyEvent.at(infoHitEndcap.isc())+=energy;
        }
    }
}


void ECalJetInteractionProcessor::FillBarrelHitHistogramms (const EventCollections &eventDataContainer){
    //EBDetId a (ieta,iphi);
    //double eta = eventDataContainer.barrelGeometry->getGeometry(a.rawId)->getPosition().eta();
    
    for (int ieta = barrelCrystalMiniEta; arrayCiEta(ieta) < barrelCrystaliEtaCount; ieta++) {
        for (int iphi=1; arrayCiPhi(iphi) < barrelCrystaliPhiCount; iphi++) {
            //std::cout << "barrel crystal coordinates are " << ieta << " " <<  arrayCiEta(ieta) << " " << iphi << std::endl;
            if (IsInterestingBarrelHit(ieta,iphi)) {
                EBHistCont.energyiPhiiEta->Fill(iphi,ieta,barrelCrystalEnergyEvent.at(arrayCiEta(ieta)).at(arrayCiPhi(iphi)));
                EBHistCont.energyTiPhiiEta->Fill(iphi,ieta,barrelCrystalEnergyTEvent.at(arrayCiEta(ieta)).at(arrayCiPhi(iphi)));
            }
        }
    }
}


void ECalJetInteractionProcessor::FillEndcapHitHistogramms (const EventCollections &eventDataContainer){
    for (int ix=0; ix<endcapiXCount; ix++) {
        for (int iy=0; iy<endcapiXCount; iy++) {
            if (IsInterestingEndcapHit(ix,iy,true)) {
                EEHistCont.energyTotalHitNumber->Fill(endcapCrystalEnergyEventPosz.at(ix).at(iy));
                EEHistCont.XYTotalHitsNumberPosZ->Fill(ix,iy);
                EEHistCont.XYTotalEnergyDepositionPosZ->Fill(ix,iy,endcapCrystalEnergyEventPosz.at(ix).at(iy));
            }
            if (IsInterestingEndcapHit(ix,iy,false)) {
                EEHistCont.energyTotalHitNumber->Fill(endcapCrystalEnergyEventNegz.at(ix).at(iy));
                EEHistCont.XYTotalHitsNumberNegZ->Fill(ix,iy);
                EEHistCont.XYTotalEnergyDepositionNegZ->Fill(ix,iy,endcapCrystalEnergyEventNegz.at(ix).at(iy));
            }
        }
    }

}

bool ECalJetInteractionProcessor::IsInterestingBarrelHit (int ieta, int iphi){
    if (barrelCrystalEnergyEvent.at(arrayCiEta(ieta)).at(arrayCiPhi(iphi))>minCrystalInterestingEnergy) return true; 
    return false;
}

bool ECalJetInteractionProcessor::IsInterestingEndcapHit (int ix, int iy, bool Posz){
    if (Posz && endcapCrystalEnergyEventPosz.at(ix).at(iy)>minCrystalInterestingEnergy) return true; 
    if (!Posz && endcapCrystalEnergyEventNegz.at(ix).at(iy)>minCrystalInterestingEnergy) return true;
    return false;
}

bool ECalJetInteractionProcessor::IsHardGenJet (const reco::GenJet &jet, const EventCollections &eventDataContainer) {
    if (jet.pt()>=15) return true;
    return false;
}

bool ECalJetInteractionProcessor::IsHardPFJet (const reco::PFJet &jet, const EventCollections &eventDataContainer) {
    if (jet.pt()>=15) return true;
    return false;
}

void ECalJetInteractionProcessor::FillJetsHistograms (const EventCollections &eventDataContainer) {
    //Just looping over all jets that passed the sorting
    for (auto &currentGenJetPointer:filteredCollections.hardGenJetPointers) {
        JetHistCont.genJetsEnergy->Fill(currentGenJetPointer->energy());
    }
    for (auto &currentPFJetPointer:filteredCollections.hardPFJetPointers) {
        JetHistCont.PFJetsPt->Fill(currentPFJetPointer->pt());
        JetHistCont.PFJetsEnergy->Fill(currentPFJetPointer->energy());
    }
    //looping over just all jets
    for (auto &currentGenJet:eventDataContainer.genJets) {
        JetHistCont.genJetsPt->Fill(currentGenJet.pt());
        JetHistCont.genJetsAllEnergyEta->Fill(currentGenJet.energy(),currentGenJet.eta());
    }
    //General over-events jets info
    
    JetHistCont.genJetsNumEvents -> Fill (filteredCollections.hardGenJetPointers.size());
    JetHistCont.PFJetsNumEvents ->Fill (filteredCollections.hardPFJetPointers.size());
    JetHistCont.interestingGenPFJetsNumEvents -> Fill (filteredCollections.hardGenJetPointers.size(),filteredCollections.hardPFJetPointers.size());
    
    //cheking interesting events
    if (filteredCollections.hardGenJetPointers.size()<=1) return;
    if (filteredCollections.hardGenJetPointers.size() != filteredCollections.hardPFJetPointers.size()) return; 
    
    for (auto &curGenJetP:filteredCollections.hardGenJetPointers) {
        const reco::GenJet *closestGenJet;
        const reco::PFJet *closestPFJet;
        closestGenJet = FindClosestNonequalJet (curGenJetP,filteredCollections.hardGenJetPointers);
        closestPFJet = FindClosestPFJet (curGenJetP,filteredCollections.hardPFJetPointers);
        JetHistCont.genGenJetsClosestDR -> Fill(deltaR(curGenJetP->p4(),closestGenJet->p4()));
        JetHistCont.genPFJetsClosestDR -> Fill (deltaR(curGenJetP->p4(),closestPFJet->p4()));
        
        JetHistCont.genJetsEmE_fracE->Fill(curGenJetP->emEnergy()/curGenJetP->energy());
        JetHistCont.genJetsHadE_fracE->Fill(curGenJetP->hadEnergy()/curGenJetP->energy());
        JetHistCont.genJetsInvE_fracE->Fill(curGenJetP->invisibleEnergy()/curGenJetP->energy());
        JetHistCont.genJetsAuxE_fracE->Fill(curGenJetP->auxiliaryEnergy()/curGenJetP->energy());
        JetHistCont.genJetsSumE_fracE->Fill((curGenJetP->emEnergy()+curGenJetP->hadEnergy()+curGenJetP->invisibleEnergy()+curGenJetP->auxiliaryEnergy())/curGenJetP->energy());
        
        //cheking if it goes to ECAL barrel and is kinda well-isolated, closest DR subject to change?
        if ( IsBarrelJet(*curGenJetP,eventDataContainer) && deltaR(curGenJetP->p4(),closestGenJet->p4()) > jetsSeparationDR) {
            // let's try doing a map
            
            for(auto& mappair : JetHistCont.genJetsEDepBar_Map) {
                mappair.second->Fill(EcalEDepJet_DR (*curGenJetP, 0, mappair.first, eventDataContainer));
            }
            for(auto& mappair : JetHistCont.genJetsEDepBar_fracE_Map) {
                mappair.second->Fill(EcalEDepJet_DR (*curGenJetP, 0, mappair.first, eventDataContainer)/curGenJetP->energy());
            }
            FillJetConstituentsHist (*curGenJetP);
            double epsilon = 0.0001;
            // we start at i=1 as we skip the first border;
            for (unsigned int i=1; i<listConeDR.size(); i++) {
                JetHistCont.genJetsAvEDepBar_fracE->Fill(listConeDR.at(i) - epsilon,EcalEDepJet_DR (*curGenJetP, 0, listConeDR.at(i), eventDataContainer)/curGenJetP->energy());
            }
            for (unsigned int i=1; i<listConeDR.size(); i++) {
                JetHistCont.genJetsAvSoftJetE_Bar_fracE->Fill(listConeDR.at(i) - epsilon,genJetSoftE_DR (*curGenJetP, listConeDR.at(i), eventDataContainer)/curGenJetP->energy());
            }
            JetHistCont.hardBarrelJetCount->Fill(1);
            //FillJetCrystalHistograms(*curGenJetP,0.5,eventDataContainer);
            
        }
        //cheking if it goes to ECAL endcap and is kinda well-isolated 
        if (IsEndcapJet(*curGenJetP,eventDataContainer) && deltaR(curGenJetP->p4(),closestGenJet->p4()) > jetsSeparationDR) {
            // let's try doing a map
            
            for(auto& mappair : JetHistCont.genJetsEDepEndc_Map) {
                mappair.second->Fill(EcalEDepJet_DR (*curGenJetP, 0, mappair.first, eventDataContainer));
            }
            for(auto& mappair : JetHistCont.genJetsEDepEndc_fracE_Map) {
                mappair.second->Fill(EcalEDepJet_DR (*curGenJetP, 0, mappair.first, eventDataContainer)/curGenJetP->energy());
            }
            FillJetConstituentsHist (*curGenJetP);
            double epsilon = 0.0001;
            // we start at i=1 as we skip the first border;
            for (unsigned int i=1; i<listConeDR.size(); i++) {
                JetHistCont.genJetsAvEDepEnd_fracE->Fill(listConeDR.at(i) - epsilon,EcalEDepJet_DR (*curGenJetP, 0, listConeDR.at(i), eventDataContainer)/curGenJetP->energy());
            }
            for (unsigned int i=1; i<listConeDR.size(); i++) {
                JetHistCont.genJetsAvSoftJetE_End_fracE->Fill(listConeDR.at(i) - epsilon,genJetSoftE_DR (*curGenJetP, listConeDR.at(i), eventDataContainer)/curGenJetP->energy());
            }
            JetHistCont.hardEndcapJetCount->Fill(1);
            //FillJetCrystalHistograms(*curGenJetP,0.5,eventDataContainer);
            
        }
        
    }
}

void ECalJetInteractionProcessor::FillJetsSplitHistograms (const EventCollections &eventDataContainer) {
    if (filteredCollections.hardGenJetPointers.size()<=1) return;
    if (filteredCollections.hardGenJetPointers.size() != filteredCollections.hardPFJetPointers.size()) return; 
    for (auto &curGenJetP:filteredCollections.hardGenJetPointers) {
        const reco::GenJet *closestGenJet;
        closestGenJet = FindClosestNonequalJet (curGenJetP,filteredCollections.hardGenJetPointers);
        if ((deltaR(curGenJetP->p4(),closestGenJet->p4()) > jetsSeparationDR) && (IsEndcapJet(*curGenJetP,eventDataContainer) || IsBarrelJet(*curGenJetP,eventDataContainer))){
            for (unsigned int i=0; i<JetSplitHistCont.stepsDR.size()-1;i++) {
                //1 and 2 require an extra cycle, right?
                //right a new procedure
                //UNDER RECONSTRUCTION
                
                FillJetCrystalSplitHistograms (*curGenJetP, i, JetSplitHistCont.stepsDR.at(i),JetSplitHistCont.stepsDR.at(i+1),eventDataContainer);

                
                JetSplitHistCont.numJet_ESimDivNcr_Eta_Pt_DRIncAr.at(i)->Fill(
                    EcalEDepJet_DR (*curGenJetP, 0, JetSplitHistCont.stepsDR.at(i+1), eventDataContainer)/
                    EcalNDepJet_DR (*curGenJetP, 0, JetSplitHistCont.stepsDR.at(i+1), eventDataContainer),
                    abs(curGenJetP->eta()),curGenJetP->pt());
                JetSplitHistCont.numJet_ESimDivNcr_Eta_Pt_DRDifAr.at(i)->Fill(
                    EcalEDepJet_DR (*curGenJetP, JetSplitHistCont.stepsDR.at(i), JetSplitHistCont.stepsDR.at(i+1), eventDataContainer)/
                    EcalNDepJet_DR (*curGenJetP, JetSplitHistCont.stepsDR.at(i), JetSplitHistCont.stepsDR.at(i+1), eventDataContainer),
                    abs(curGenJetP->eta()),curGenJetP->pt());

                JetSplitHistCont.numJet_ESimDivEjet_Eta_Pt_DRIncAr.at(i)->Fill(
                    EcalEDepJet_DR (*curGenJetP, 0, JetSplitHistCont.stepsDR.at(i+1), eventDataContainer)/curGenJetP->energy(),
                    abs(curGenJetP->eta()),curGenJetP->pt());
                JetSplitHistCont.numJet_ESimDivEjet_Eta_Pt_DRDifAr.at(i)->Fill(
                    EcalEDepJet_DR (*curGenJetP, JetSplitHistCont.stepsDR.at(i), JetSplitHistCont.stepsDR.at(i+1), eventDataContainer)/curGenJetP->energy(),
                    abs(curGenJetP->eta()),curGenJetP->pt());

            }
        }
    }


}

const reco::GenJet* ECalJetInteractionProcessor::FindClosestNonequalJet (const reco::GenJet *target, std::vector<const reco::GenJet*> collection){
    double currentDR = 999;
    const reco::GenJet *candidate = nullptr;
    for (auto &newCandidate : collection) {
        if (deltaR(newCandidate->p4(),target->p4()) < currentDR && newCandidate != target) {
            currentDR = deltaR(newCandidate->p4(),target->p4());
            candidate = newCandidate;
        }
    }
    if (nullptr == candidate) std::cout << "Jet finding error" << std::endl;
    return candidate;
}

const reco::PFJet* ECalJetInteractionProcessor::FindClosestPFJet (const reco::GenJet *target, std::vector<const reco::PFJet*> collection) {
    double currentDR = 999;
    const reco::PFJet *candidate = nullptr;
    for (auto &newCandidate : collection) {
        if (deltaR(newCandidate->p4(),target->p4()) < currentDR) {
            currentDR = deltaR(newCandidate->p4(),target->p4());
            candidate = newCandidate;
        }
    }
    return candidate;
}

double ECalJetInteractionProcessor::EcalEDepJet_DR (const reco::Jet &jet, double lowCutoffDR, double highCutoffDR, const EventCollections &eventDataContainer) {
    if ( lowCutoffDR > highCutoffDR ) std::cout << "lowCutoffDR " << lowCutoffDR  << " > highCutoffDR " << highCutoffDR << std::endl;
    double totalEnergy=0;
    for (int ieta = barrelCrystalMiniEta; arrayCiEta(ieta) < barrelCrystaliEtaCount; ieta++) {
        for (int iphi=1; arrayCiPhi(iphi) < barrelCrystaliPhiCount; iphi++) {
            if (IsInterestingBarrelHit(ieta,iphi)) {
                EBDetId crystalId = EBDetId(ieta,iphi);
                double etaCr = eventDataContainer.barrelGeometry->getGeometry(crystalId)->getPosition().eta();
                double phiCr = eventDataContainer.barrelGeometry->getGeometry(crystalId)->getPosition().phi();
                double DR = deltaR (jet.eta(),jet.phi(),etaCr,phiCr);
                if (lowCutoffDR < DR && DR < highCutoffDR) 
                   totalEnergy+=barrelCrystalEnergyEvent.at(arrayCiEta(ieta)).at(arrayCiPhi(iphi));
            }
        }
    }
    for (int ix=0; ix<endcapiXCount; ix++) {
        for (int iy=0; iy<endcapiXCount; iy++) {
            if (IsInterestingEndcapHit(ix,iy,true)) {
                EEDetId crystalId = EEDetId(ix,iy,1);
                double etaCr = eventDataContainer.endcapGeometry->getGeometry(crystalId)->getPosition().eta();
                double phiCr = eventDataContainer.endcapGeometry->getGeometry(crystalId)->getPosition().phi();
                double DR = deltaR (jet.eta(),jet.phi(),etaCr,phiCr);
                if (lowCutoffDR < DR && DR < highCutoffDR) 
                   totalEnergy+=endcapCrystalEnergyEventPosz.at(ix).at(iy);
            }
            if (IsInterestingEndcapHit(ix,iy,false)) {
                EEDetId crystalId = EEDetId(ix,iy,-1);
                double etaCr = eventDataContainer.endcapGeometry->getGeometry(crystalId)->getPosition().eta();
                double phiCr = eventDataContainer.endcapGeometry->getGeometry(crystalId)->getPosition().phi();
                double DR = deltaR (jet.eta(),jet.phi(),etaCr,phiCr);
                if (lowCutoffDR < DR && DR < highCutoffDR) 
                   totalEnergy+=endcapCrystalEnergyEventNegz.at(ix).at(iy);
            }
        }
    }
    return totalEnergy;
}

double ECalJetInteractionProcessor::EcalNDepJet_DR (const reco::Jet &jet, double lowCutoffDR, double highCutoffDR, const EventCollections &eventDataContainer) {
    if ( lowCutoffDR > highCutoffDR ) std::cout << "lowCutoffDR " << lowCutoffDR  << " > highCutoffDR " << highCutoffDR << std::endl;
    double totalCrystals=0;
    double DR, etaCr, phiCr;
    for (int ieta = barrelCrystalMiniEta; arrayCiEta(ieta) < barrelCrystaliEtaCount; ieta++) {
        for (int iphi=1; arrayCiPhi(iphi) < barrelCrystaliPhiCount; iphi++) {
                EBDetId crystalId = EBDetId(ieta,iphi);
                etaCr = eventDataContainer.barrelGeometry->getGeometry(crystalId)->getPosition().eta();
                phiCr = eventDataContainer.barrelGeometry->getGeometry(crystalId)->getPosition().phi();
                DR = deltaR (jet.eta(),jet.phi(),etaCr,phiCr);
                if (lowCutoffDR < DR && DR < highCutoffDR) totalCrystals++;
            
        }
    }
    for (int ix=1; ix<endcapiXCount; ix++) {
        for (int iy=1; iy<endcapiXCount; iy++) {
            if (IsInterestingEndcapHit(ix,iy,true)) {
                EEDetId crystalIdplus = EEDetId(ix,iy,1);
                etaCr = eventDataContainer.endcapGeometry->getGeometry(crystalIdplus)->getPosition().eta();
                phiCr = eventDataContainer.endcapGeometry->getGeometry(crystalIdplus)->getPosition().phi();
                DR = deltaR (jet.eta(),jet.phi(),etaCr,phiCr);
                if (lowCutoffDR < DR && DR < highCutoffDR) totalCrystals++;
            }
                
        }
    }
    for (int ix=1; ix<endcapiXCount; ix++) {
        for (int iy=1; iy<endcapiXCount; iy++) {
            if (IsInterestingEndcapHit(ix,iy,false)) {
                EEDetId crystalIdminus = EEDetId(ix,iy,-1);
                etaCr = eventDataContainer.endcapGeometry->getGeometry(crystalIdminus)->getPosition().eta();
                phiCr = eventDataContainer.endcapGeometry->getGeometry(crystalIdminus)->getPosition().phi();
                DR = deltaR (jet.eta(),jet.phi(),etaCr,phiCr);
                if (lowCutoffDR < DR && DR < highCutoffDR) totalCrystals++;
            }
        }
    }
    return totalCrystals;
}



double ECalJetInteractionProcessor::genJetSoftE_DR (const reco::Jet &jet,  double cutoffDR, const EventCollections &eventDataContainer) {
    double softJetEnergy=0;
    double epsilon = 0.001;
    double currentDR;
    for (auto &currentSoftJetCandidate : eventDataContainer.genJets) {
        currentDR = deltaR(currentSoftJetCandidate.p4(),jet.p4());
        if (!IsHardGenJet(currentSoftJetCandidate,eventDataContainer) && currentDR < cutoffDR && currentDR > epsilon ){
            softJetEnergy+=currentSoftJetCandidate.energy();
        }
    }
    return softJetEnergy;
}    

void ECalJetInteractionProcessor::FillJetConstituentsHist (const reco::GenJet jet) {
     for (auto curPart:jet.getGenConstituents()) {
        switch (abs(curPart->pdgId())){
            case 22: case 11: //photon, e-
                JetHistCont.genPartonsJetDR_photons->Fill(deltaR(jet.p4(),curPart->p4()),curPart->energy());
                break;
            case 211: //pi+-
                JetHistCont.genPartonsJetDR_pions->Fill(deltaR(jet.p4(),curPart->p4()),curPart->energy());
                break;
            case 321: case 310: case 130: case 2212: case 2112: //K+- K^0 p n 
                JetHistCont.genPartonsJetDR_hadrons->Fill(deltaR(jet.p4(),curPart->p4()),curPart->energy());
                break;    
            default: { //other
                //std::cout << "constituent pdgid is " << abs(curPart->pdgId()) << std::endl;
                JetHistCont.genPartonsJetDR_other->Fill(deltaR(jet.p4(),curPart->p4()),curPart->energy());
            }
        }
    }                
}

bool  ECalJetInteractionProcessor::IsBarrelJet (const reco::Jet &jet, const EventCollections &eventDataContainer) {
    return abs(jet.eta())<(1.442-0.5);
}

bool  ECalJetInteractionProcessor::IsEndcapJet (const reco::Jet &jet, const EventCollections &eventDataContainer) {
    return (1.566+0.5)<abs(jet.eta()) && abs(jet.eta())<(3 - .5);
}


void  ECalJetInteractionProcessor::FillJetCrystalSplitHistograms (const reco::Jet &jet, int number, double lowCutoffDR, double highCutoffDR, const EventCollections &eventDataContainer) {
    for (int ieta = barrelCrystalMiniEta; arrayCiEta(ieta) < barrelCrystaliEtaCount; ieta++) {
        for (int iphi=1; arrayCiPhi(iphi) < barrelCrystaliPhiCount; iphi++) {
            if (IsInterestingBarrelHit(ieta,iphi)) {
                EBDetId crystalId = EBDetId(ieta,iphi);
                double etaCr = eventDataContainer.barrelGeometry->getGeometry(crystalId)->getPosition().eta();
                double phiCr = eventDataContainer.barrelGeometry->getGeometry(crystalId)->getPosition().phi();
                double DR = deltaR (jet.eta(),jet.phi(),etaCr,phiCr);
                if (DR < highCutoffDR) {
                    JetSplitHistCont.numCrystal_ESim_Eta_Pt_DRIncAr.at(number)->Fill( barrelCrystalEnergyEvent.at(arrayCiEta(ieta)).at(arrayCiPhi(iphi)),
                    abs(jet.eta()),jet.pt());
                    
                    JetSplitHistCont.numCrystal_ESimT_Eta_Pt_DRIncAr.at(number)->Fill(
                    barrelCrystalEnergyEvent.at(arrayCiEta(ieta)).at(arrayCiPhi(iphi))/cosh(etaCr),
                    abs(jet.eta()),jet.pt());
                }
                   //fill hist with 
            }
        }
    }
    for (int ix=0; ix<endcapiXCount; ix++) {
        for (int iy=0; iy<endcapiXCount; iy++) {
            if (IsInterestingEndcapHit(ix,iy,true)) {
                EEDetId crystalId = EEDetId(ix,iy,1);
                double etaCr = eventDataContainer.endcapGeometry->getGeometry(crystalId)->getPosition().eta();
                double phiCr = eventDataContainer.endcapGeometry->getGeometry(crystalId)->getPosition().phi();
                double DR = deltaR (jet.eta(),jet.phi(),etaCr,phiCr);
                if (DR < highCutoffDR) {
                    JetSplitHistCont.numCrystal_ESim_Eta_Pt_DRIncAr.at(number)->Fill( endcapCrystalEnergyEventPosz.at(ix).at(iy),
                    abs(jet.eta()),jet.pt());
                    
                    JetSplitHistCont.numCrystal_ESimT_Eta_Pt_DRIncAr.at(number)->Fill(
                    endcapCrystalEnergyEventPosz.at(ix).at(iy)/cosh(etaCr),
                    abs(jet.eta()),jet.pt());
                }
            }
            if (IsInterestingEndcapHit(ix,iy,false)) {
                EEDetId crystalId = EEDetId(ix,iy,-1);
                double etaCr = eventDataContainer.endcapGeometry->getGeometry(crystalId)->getPosition().eta();
                double phiCr = eventDataContainer.endcapGeometry->getGeometry(crystalId)->getPosition().phi();
                double DR = deltaR (jet.eta(),jet.phi(),etaCr,phiCr);
                if (DR < highCutoffDR) {
                    JetSplitHistCont.numCrystal_ESim_Eta_Pt_DRIncAr.at(number)->Fill( endcapCrystalEnergyEventNegz.at(ix).at(iy),
                    abs(jet.eta()),jet.pt());
                    
                    JetSplitHistCont.numCrystal_ESimT_Eta_Pt_DRIncAr.at(number)->Fill(
                    endcapCrystalEnergyEventNegz.at(ix).at(iy)/cosh(etaCr),
                    abs(jet.eta()),jet.pt());
                }
            }
        }
    }
    
    
    
    /*
    std::vector <TH3D*> numCrystal_ESim_Eta_Pt_DRIncAr;
    std::vector <TH3D*> numCrystal_ESimT_Eta_Pt_DRIncAr;
    */
}



void ECalJetInteractionProcessor::FinalizeHistograms (){
    // the nessesary pt adjustments, an extra 1.4037 powers of pt up - already done
}


