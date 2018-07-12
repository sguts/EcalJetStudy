//So far it's a basic outline of pretty plotter script
#include "TLorentzVector.h"
#include "TMath.h"
#include <TFile.h>
#include <TStyle.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TProfile2D.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TPaletteAxis.h>
#include <TLegend.h>
#include <cmath>
#include <string>
#include <iostream>
#include <vector>
#include "ToStringWithPrecision.h"

class PrettyPlotRootScript
{
public:
    PrettyPlotRootScript();
    ~PrettyPlotRootScript();

private:
    bool OpenFile (std::string histFileName);
    void PlotHistograms ();
    void PlotTestHistograms ();
    void CloseFile ();

    TCanvas* CanvasSetup (TH1 *hist, std::string filename, std::string title, std::string Xtitle, std::string Ytitle, int Xsize, int Ysize, bool logY=false, double minY=-1, double maxY=-1);
        
    void Plot1D (TH1 *hist, std::string filename, std::string title, std::string Xtitle, std::string Ytitle, int Xsize, int Ysize, int color, bool logY=false, double maxY=-1, double minY=-1);

    void Plot2D (TH2 *hist, std::string filename, std::string title, std::string Xtitle, std::string Ytitle, int Xsize, int Ysize);


    //3D hists coordinates: variable, eta, pt
    void Plot3DCollapseEtaSplitPt (TH3 *hist, std::string filename, std::string title, std::string Xtitle, std::string Ytitle, int Xsize, int Ysize, bool logY=false, double maxY=-1); 
    void Plot3DSplitEtaCollapsePt (TH3 *hist, std::string filename, std::string title, std::string Xtitle, std::string Ytitle, int Xsize, int Ysize, bool logY=false, double maxY=-1);
    
    void Plot3DChooseEtaChoosePt (TH3 *hist, int etaMinBin, int etaMaxBin, int ptMinBin, int ptMaxBin,  std::string filename, std::string title, std::string Xtitle, std::string Ytitle, int Xsize, int Ysize, int color, bool logY=false);

    void Plot3DMake2D (TH3 *hist, std::string filename, std::string title, std::string Xtitle, std::string Ytitle, int Xsize, int Ysize, bool mean=true, bool logY=false);
    void MakePartonPlot ();
    

    TFile *inputFile;

    std::vector <double> etaBinning = {0,0.5,1,1.5,2,2.5,3};
    std::vector <double> ptBinning = {15,20,30,50,80,150};

    std::vector <double> stepsDR = {0,0.1,0.2,0.3,0.4,0.5};

    std::vector <int> colorArray = {1,2,4,8,28,46,49};

	
};

PrettyPlotRootScript::PrettyPlotRootScript() {
	//PlotTestHistograms ();
	if (!(OpenFile ("FIX IT!"))) {
		std::cout << "Failed to open the file" << std::endl;
	} else {
		PlotHistograms ();
	}
	CloseFile();
}

PrettyPlotRootScript::~PrettyPlotRootScript() {
    
}

bool PrettyPlotRootScript::OpenFile (std::string histFileName) {
	inputFile = new TFile ("/afs/cern.ch/user/s/sguts/public/EcalJetSIMStudy/JetEcalSampleHistIteration1.root","READ");
        if (!inputFile) return false;
        return true;
}

void PrettyPlotRootScript::PlotHistograms() {
    //gROOT->SetBatch(kTRUE); //Figure out how it works
    TH1D *genJetPtHist = (TH1D *) inputFile->Get("ecalAnalyzer/JetHist/genJetsPt");
    Plot1D (genJetPtHist,"genJetPtPlot.pdf","p_{T} of Gen Jets","p_{T}, GeV", "N_{jets}", 2400,2000,4,true);


    TH2D *genJetPFJetHist = (TH2D *) inputFile->Get("ecalAnalyzer/JetHist/interestingGenPFJetsNumEvents");
    Plot2D (genJetPFJetHist,"genJetPFJetPlot.pdf","Gen Jets vs PF Jets","Number of hard Gen Jets","Number of hard PF Jets",2400,2000);


    TH3D *numCrystal_ESim =(TH3D *) inputFile->Get("ecalAnalyzer/JetSplitHist/numCrystal_ESim_Eta_Pt_DRIncAr5");
    TH3D *numCrystal_ESimT =(TH3D *) inputFile->Get("ecalAnalyzer/JetSplitHist/numCrystal_ESimT_Eta_Pt_DRIncAr5");

    Plot3DCollapseEtaSplitPt (numCrystal_ESim,"SimCrystal_E_PtSplit.pdf","Energy of individual ECAL crystal split by p_{T} of Gen jet","E, GeV","",2400,2000,true,1000000);
    Plot3DCollapseEtaSplitPt (numCrystal_ESimT,"SimCrystal_ET_PtSplit.pdf","Transversal energy of individual ECAL crystal split by p_{T} of Gen jet","E_{T}, GeV","",2400,2000,true,1000000);
    
    Plot3DSplitEtaCollapsePt (numCrystal_ESim,"SimCrystal_E_EtaSplit.pdf","Energy of individual ECAL crystal split by |#eta| of Gen jet","E, GeV","",900,750,true);
    Plot3DSplitEtaCollapsePt (numCrystal_ESimT,"SimCrystal_ET_EtaSplit.pdf","Transversal energy of individual ECAL crystal split by |#eta| of Gen jet","E_{T}, GeV","",2400,2000,true);
    
    TH3D *numJet_ESimAv =(TH3D *) inputFile->Get("ecalAnalyzer/JetSplitHist/numJet_ESimDivNcr_Eta_Pt_DRIncAr5");
    
    Plot3DSplitEtaCollapsePt (numJet_ESimAv,"Jet_ESimAv_EtaSplit.pdf","Average energy of individual ECAL crystal in DR < 0.5 for jet split by |#eta| of Gen jet","E, GeV","",2400,2000,true);

    MakePartonPlot ();
    
    TH3D *EDivEJet = (TH3D *) inputFile->Get("ecalAnalyzer/JetSplitHist/numJet_ESimDivEjet_Eta_Pt_DRIncAr5");
    
    Plot3DMake2D (EDivEJet, "EDivEJetAvMean.pdf", "Average of E_{sim}/E_{gen}", "|#eta|", "p_{T}, GeV", 2400,2000);
    Plot3DMake2D (EDivEJet, "EDivEJetAvRMS.pdf", "RMS of  E_{sim}/E_{gen}", "|#eta|", "p_{T}, GeV", 2400, 2000, false);
    
    Plot3DChooseEtaChoosePt (EDivEJet,1,1,1,1, "EDivEJetBin1_1.pdf", "E_{sim}/E_{gen}", "E_{sim}/E_{gen}","",2400, 2000,colorArray[2]);
    Plot3DChooseEtaChoosePt (EDivEJet,1,1,3,3, "EDivEJetBin1_3.pdf", "E_{sim}/E_{gen}", "E_{sim}/E_{gen}","",2400, 2000,colorArray[2]);
    
    
    TH1D *genJetsAvEDepBar_fracE = (TH1D *) inputFile->Get("ecalAnalyzer/JetHist/genJetsAvEDepBar_fracE");
    TH1D *genJetsAvEDepEnd_fracE = (TH1D *) inputFile->Get("ecalAnalyzer/JetHist/genJetsAvEDepEnd_fracE");
    
    TH1D *genJetsAvSoftJetE_Bar_fracE = (TH1D *) inputFile->Get("ecalAnalyzer/JetHist/genJetsAvSoftJetE_Bar_fracE");
    TH1D *genJetsAvSoftJetE_End_fracE = (TH1D *) inputFile->Get("ecalAnalyzer/JetHist/genJetsAvSoftJetE_End_fracE");
    
    TH1D *hardBarrelJetCount = (TH1D *) inputFile->Get("ecalAnalyzer/JetHist/hardBarrelJetCount");
    TH1D *hardEndcapJetCount = (TH1D *) inputFile->Get("ecalAnalyzer/JetHist/hardEndcapJetCount");
    
    
    genJetsAvEDepBar_fracE->Scale(1/hardBarrelJetCount->GetEntries());
    genJetsAvEDepEnd_fracE->Scale(1/hardEndcapJetCount->GetEntries());

    genJetsAvSoftJetE_Bar_fracE->Scale(1/hardBarrelJetCount->GetEntries());
    genJetsAvSoftJetE_End_fracE->Scale(1/hardEndcapJetCount->GetEntries());
    
    Plot1D(genJetsAvEDepBar_fracE,"genJetsAvEDepBar_fracE.pdf","Average #frac{E_{sim}^{DR}}{E_{gen}} for hard jets in barrel","DR","",2400,2000,4,false,0,0.6);
    Plot1D(genJetsAvEDepEnd_fracE,"genJetsAvEDepEnd_fracE.pdf","Average #frac{E_{sim}^{DR}}{E_{gen}} for hard jets in endcap","DR","",2400,2000,4,false,0,0.6);
    
    Plot1D(genJetsAvSoftJetE_Bar_fracE,"genJetsAvSoftJetE_Bar_fracE.pdf","Average #frac{E_{soft}^{DR}}{E_{gen}} for hard jets in barrel","DR","",2400,2000,4,false,0,0.1);
    Plot1D(genJetsAvSoftJetE_End_fracE,"genJetsAvSoftJetE_End_fracE.pdf","Average #frac{E_{soft}^{DR}}{E_{gen}} for hard jets in endcap","DR","",2400,2000,4,false,0,0.1);
}

TCanvas* PrettyPlotRootScript::CanvasSetup (TH1 *hist, std::string filename, std::string title, std::string Xtitle, std::string Ytitle, int Xsize, int Ysize,  bool logY, double maxY, double minY) {
    if (!hist) {
        std::cout << "Error, histogram named " << title << " doesn't exist" << std::endl;
    }
    TCanvas* c = new TCanvas(hist->GetName(),hist->GetName(),Xsize,Ysize);
    gStyle->SetOptStat(0);
    hist->SetTitle(title.c_str());
    hist->GetXaxis()->SetTitle(Xtitle.c_str());
    hist->GetYaxis()->SetTitle(Ytitle.c_str());
    
    if (maxY != -1 && minY == -1) hist->GetYaxis()->SetRangeUser(1, maxY);
    if (maxY != -1 && minY != -1) hist->GetYaxis()->SetRangeUser(minY, maxY);
    
    
    if (logY) c->SetLogy();

    return c;

}

void PrettyPlotRootScript::Plot1D (TH1 *hist, std::string filename, std::string title, std::string Xtitle, std::string Ytitle, int Xsize, int Ysize, int color, bool logY, double minY, double maxY) {
    auto c=CanvasSetup (hist,filename,title,Xtitle,Ytitle,Xsize,Ysize,logY,maxY,minY);
    
    hist->SetLineColor(color);
    hist->SetLineWidth(2);
    hist->Draw();
    c->SaveAs(filename.c_str());
    
    delete c;
}

void PrettyPlotRootScript::Plot2D (TH2 *hist, std::string filename, std::string title, std::string Xtitle, std::string Ytitle, int Xsize, int Ysize) {
    auto c=CanvasSetup (hist,filename,title,Xtitle,Ytitle,Xsize,Ysize);
    
    /*TPad *pad1 = new TPad("pad1", "",  0, 0.05, 0.95 , 0.95);
    c->cd();
    pad1->Draw();
    pad1->cd();
    pad1->SetRightMargin(1);
    */
    
    hist->Draw("colz");
    
    c->Update();
    
    TPaletteAxis *palette = (TPaletteAxis*)hist->GetListOfFunctions()->FindObject("palette");
    if (!palette) std::cout << "Oh noes! No palette!" <<std::endl;
    palette->SetTitleSize (0.1);
    palette->SetLabelSize (0.1);
    
    palette->SetX1NDC(0.9);
    palette->SetX2NDC(0.92);
    palette->SetY1NDC(0.1);
    palette->SetY2NDC(0.9);
    c->Modified();
    
    
    c->SaveAs(filename.c_str());

    delete c;
}

void  PrettyPlotRootScript::Plot3DCollapseEtaSplitPt (TH3 *hist, std::string filename, std::string title, std::string Xtitle, std::string Ytitle, int Xsize, int Ysize, bool logY, double maxY) {
    std::vector <TH1D*> histVect = {}; 
    for (unsigned int i=0; i<ptBinning.size()-1; i++){
    	TH1D *tempHist = (TH1D *) hist->ProjectionX("_px",0,-1,i+1,i+1)->Clone();
        histVect.push_back(tempHist);
        //std::cout << "i = " << i  << " mean is " << histVect.at(i)->GetMean() <<std::endl;
    }
    auto c=CanvasSetup (histVect.at(0),filename,title,Xtitle,Ytitle,Xsize,Ysize,logY, maxY);
    auto legend = new TLegend(0.6,0.6,0.9,0.9);
    //legend->SetHeader("The Legend Title","C"); // option "C" allows to center the header
    for (unsigned int i=0; i<histVect.size(); i++){
    	//std::cout << "i = " << i << " colour is " << colorArray.at(i) << " mean is " << histVect.at(i)->GetMean() <<std::endl;
        histVect.at(i)->SetLineColor(colorArray.at(i));
        histVect.at(i)->SetLineWidth(2);
        histVect.at(i)->Draw("same");
        legend->AddEntry(histVect.at(i),(to_string_with_precision(ptBinning.at(i),3)+" < p_{T} < "+to_string_with_precision(ptBinning.at(i+1),3)+"  #mu = "+ to_string_with_precision(histVect.at(i)->GetMean(),2) + " #sigma = " + to_string_with_precision(histVect.at(i)->GetRMS(),2)).c_str(),"L");
    }
    legend->Draw("same");
    
    c->SaveAs(filename.c_str());

    delete c;
}

void PrettyPlotRootScript::Plot3DSplitEtaCollapsePt (TH3 *hist, std::string filename, std::string title, std::string Xtitle, std::string Ytitle, int Xsize, int Ysize, bool logY, double maxY) {
    std::vector <TH1D*> histVect = {}; 
    for (unsigned int i=0; i<etaBinning.size()-1; i++){
        TH1D *tempHist = (TH1D *) hist->ProjectionX("_px",i+1,i+1,0,-1)->Clone();
        histVect.push_back(tempHist);
        std::cout << "i = " << i  << " mean is " << histVect.at(i)->GetMean() <<std::endl;
    }
    auto c=CanvasSetup (histVect.at(0),filename,title,Xtitle,Ytitle,Xsize,Ysize,logY, maxY);
    auto legend = new TLegend(0.6,0.6,0.9,0.9);
    //legend->SetHeader("The Legend Title","C"); // option "C" allows to center the header
    for (unsigned int i=0; i<histVect.size(); i++){
        std::cout << "i = " << i << " colour is " << colorArray.at(i) << " mean is " << histVect.at(i)->GetMean() <<std::endl;
        if (histVect.at(i)->GetMean() != 0) {
            histVect.at(i)->SetLineColor(colorArray.at(i));
            histVect.at(i)->SetLineWidth(2);
            histVect.at(i)->Draw("same");
            legend->AddEntry(histVect.at(i),(to_string_with_precision(etaBinning.at(i),3)+" < |#eta| < "+to_string_with_precision(etaBinning.at(i+1),3)+"  #mu = "+ to_string_with_precision(histVect.at(i)->GetMean(),2) + " #sigma = " + to_string_with_precision(histVect.at(i)->GetRMS(),2)).c_str(),"L");
        }
    }
    legend->Draw("same");
    
    c->SaveAs(filename.c_str());

    delete c;
    
}

void PrettyPlotRootScript::Plot3DChooseEtaChoosePt (TH3 *hist, int etaMinBin, int etaMaxBin, int ptMinBin, int ptMaxBin,  std::string filename, std::string title, std::string Xtitle, std::string Ytitle, int Xsize, int Ysize, int color, bool logY){
    std::cout << "Hello there!" <<std::endl;
    TH1D *tempHist = (TH1D *) hist->ProjectionX("_px",etaMinBin,etaMaxBin,ptMinBin,ptMaxBin)->Clone();
    auto c=CanvasSetup (tempHist,filename,title,Xtitle,Ytitle,Xsize,Ysize,logY);
    auto legend = new TLegend(0.5,0.8,0.9,0.9);
    
    tempHist->SetLineColor(color);
    tempHist->SetLineWidth(2);
    tempHist->Draw("same");
    
    legend->AddEntry(tempHist,(
        to_string_with_precision(etaBinning.at(etaMinBin-1),3)+" < |#eta| < "+to_string_with_precision(etaBinning.at(etaMaxBin),3)+" "+to_string_with_precision(ptBinning.at(ptMinBin-1),3)+" < p_{T} < "+to_string_with_precision(ptBinning.at(ptMaxBin),3)+"  #mu = "+ to_string_with_precision(tempHist->GetMean(),4) + " #sigma = " + to_string_with_precision(tempHist->GetRMS(),4)
    ).c_str(),"L");
    legend->Draw("same");
    
    c->SaveAs(filename.c_str());
}


void PrettyPlotRootScript::MakePartonPlot () {
    std::cout << "Making constituents plot" << std::endl;
    TH1D *pions =(TH1D *) inputFile->Get("ecalAnalyzer/JetHist/genPartonsJetDR_pions");
    TH1D *EM =(TH1D *) inputFile->Get("ecalAnalyzer/JetHist/genPartonsJetDR_photons");
    TH1D *hadrons =(TH1D *) inputFile->Get("ecalAnalyzer/JetHist/genPartonsJetDR_hadrons");
    TH1D *other =(TH1D *) inputFile->Get("ecalAnalyzer/JetHist/genPartonsJetDR_other");
    auto c=CanvasSetup (pions,"ConstituentEnergyDR.pdf","Energy of jet constituents as fuction of #Delta R","#Delta R = #sqrt{#Delta #phi^{2} + #Delta #eta^{2}}","E, GeV",900,750,false);
    auto legend = new TLegend(0.55,0.6,0.9,0.9);

    c->SetLogy();
    
    pions->SetLineColor(colorArray.at(1));
    pions->SetLineWidth(2);
    pions->Draw("same");
    legend->AddEntry(pions,"Charged pions","L");
    
    EM->SetLineColor(colorArray.at(2));
    EM->SetLineWidth(2);
    EM->Draw("same");
    legend->AddEntry(EM,"Electromagnetic constituents (#gamma, e^{#pm})","L");
    
    hadrons->SetLineColor(colorArray.at(3));
    hadrons->SetLineWidth(2);
    hadrons->Draw("same");
    legend->AddEntry(hadrons,"Heavy hadronic constituents (p,n,K,#Sigma,#Lambda)","L");
    
    other->SetLineColor(colorArray.at(4));
    other->SetLineWidth(2);
    other->Draw("same");
    legend->AddEntry(other,"Other constituents","L");
    
    legend->Draw("same");
    c->SaveAs("ConstituentEnergyDR.pdf");
    
    delete c;
    delete legend;
}

void PrettyPlotRootScript::Plot3DMake2D (TH3 *hist, std::string filename, std::string title, std::string Xtitle, std::string Ytitle, int Xsize, int Ysize, bool mean, bool logY){
    std::cout << "Profile s option applied" << mean << std::endl;
    TProfile2D *profile= hist->Project3DProfile("zy");
    profile->SetErrorOption("s");
    std::string meanOption;
    if (mean) {
        meanOption=std::string("E");
        
    }
        else {
            meanOption=std::string("C=E");
        }
    std::cout << "Entering procedure , mean is " << mean << std::endl;
    TH2D *plotHist = profile->ProjectionXY("_pxy",meanOption.c_str());
    auto c=CanvasSetup (plotHist ,filename,title,Xtitle,Ytitle,Xsize,Ysize);
    
    
    if (mean) std::cout << "For going through TProfile2D bin value is " << plotHist ->GetBinContent(1,1) << " bin error for 1,1 is " << plotHist ->GetBinError(1,1) << std::endl;
    
    plotHist ->SetTitle(title.c_str());
    plotHist ->GetXaxis()->SetTitle(Xtitle.c_str());
    plotHist ->GetYaxis()->SetTitle(Ytitle.c_str());
    
    gStyle->SetPaintTextFormat("1.4f");
    
    plotHist ->GetYaxis()->SetRangeUser(15,165);
    
    std::cout << "First drawing , mean is " << mean << std::endl;
    plotHist ->Draw("colz same");
    if (mean) {
        plotHist ->GetZaxis()->SetRangeUser(0.4, 0.5);
    } else  {
        plotHist ->GetZaxis()->SetRangeUser(0.12, 0.16);
    }
    c->Update();
    
    std::cout << "Pallete , mean is " << mean << std::endl;
    TPaletteAxis *palette = (TPaletteAxis*)plotHist ->GetListOfFunctions()->FindObject("palette");
    if (!palette) std::cout << "Oh noes! No palette!" <<std::endl;
    palette->SetTitleSize (0.1);
    palette->SetLabelSize (0.1);
    
    palette->SetX1NDC(0.9);
    palette->SetX2NDC(0.92);
    palette->SetY1NDC(0.1);
    palette->SetY2NDC(0.9);
    c->Modified();
    
    plotHist ->Draw("TEXT same");
    
    c->SaveAs(filename.c_str());
    
    delete c;
}

void PrettyPlotRootScript::PlotTestHistograms () {
    
}

void PrettyPlotRootScript::CloseFile () {
    inputFile->Close();
}


//TO DO: rms plot, mean and rms on legend.
