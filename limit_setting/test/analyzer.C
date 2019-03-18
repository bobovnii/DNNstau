#define analyzer_cxx
#include "analyzer.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TString.h>
//#include "plots.h"
#include <cmath>
#include <sstream>
#include <iomanip>
#include "TChain.h"
#include "TH1.h"
#include "TTree.h"
#include "TKey.h"
#include "Riostream.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TFrame.h"
#include "TGaxis.h"
#include "TStyle.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include "TList.h"
#include <string>
#include "TObject.h"
#include "TBranch.h"
#include <functional>
#include "TAxis.h"
#include "TChain.h"
#include "TMath.h"
#include "Riostream.h"
#include <iostream>
#include <fstream>




using namespace std;



void analyzer::Loop(){


	if (fChain == 0) return;


	CutList.clear();
	CutList.push_back("Nocut");
	CutList.push_back("PU");
	CutList.push_back("#mu#tau");
	CutList.push_back("2ndlepV");
	CutList.push_back("3rdlepV");
	CutList.push_back("TriggerSF");
	CutList.push_back("LeptonSF");
	CutList.push_back("Tau-id");
	CutList.push_back("TransferR");
	CutList.push_back("LeptonTauFakeRate");
	CutList.push_back("METrecoil");
	CutList.push_back("ZpTcorrection");
	CutList.push_back("topPtRwgt");
	CutList.push_back("Jets<2 ");
	CutList.push_back("btag=0");
	CutList.push_back("MTwindow"); //15
	CutList.push_back("Jets<1 ");
	CutList.push_back("BDThigh");
	CutList.push_back("BDTlow");
	CutList.push_back("StauCuts");
	CutList.push_back("StauCuts2");
	CutList.push_back("MET>20");
	CutList.push_back("MET>30");
	CutList.push_back("MET>40");
	CutList.push_back("MTsum>120");


	string channel = "mutau";
	string Channel = "mutau";
	string region = "REGION";

	//ChiMass = 0;
	ChiMass = 0.;
	//cout<<" THE MASS IS ========================= "<<ChiMass<<endl;
	 int CutNumb = int(CutList.size());
         int iCFCounter[CutNumb];
	 double CFCounter[CutNumb];
	 for (int i=0;i < CutNumb; i++){
                        CFCounter[i] = 0;
			CFCounter_[i] = 0;
         	        iCFCounter[i] = 0;
        }
	fChain->GetEntry(0);  
	string old = datasetName->c_str();
	int counter = 0;
		
	//char hist_[100];
	bool isData = false;
	bool isTT = false;
	bool isWJ = false;
	bool isDY = false;
	bool isDYee = false;
	bool isDYmm = false;
	bool isDYnunu = false;
	bool isDYhigh = false;
	bool isDYlow = false;
	bool isZTT = false;
	bool isDYNJ = false;
	bool isWNJ = false;
	bool isNJ = false;
	bool isSignal = false;
	bool isSUSY = false;
	bool cut_btag = true;
	
	bool t = true;
	bool f = false;
	bool InvertLeptonIso = false;
	//bool HighInvertLeptonIso = HIGHISO;
	string Sign = "SIGNHERE";
	
	
	string systematic="SYSTEMATICHERE";

	string cut = to_string(t);
	cout <<"  "<<cut.c_str()<<endl;
	char plots_[100];
	string data = "Single";
	string tt = "TT_TuneCUETP8M1_13TeV-powheg-pythia8";
	string wj = "WJetsToLNu";
	string wj1 = "W1JetsToLNu";
	string wj2 = "W2JetsToLNu";
	string wj3 = "W3JetsToLNu";
	string wj4 = "W4JetsToLNu";
	string dyj = "DYJetsToLL";
	string dyjhigh = "DYJetsToLL_M-50";
	string dyjlow = "DYJetsToLL_M-5to";
	string dyjlow2 = "DYJetsToLL_M-10to";
	string dyj1 = "DY1JetsToLL";
	string dyj2 = "DY2JetsToLL";
	string dyj3 = "DY3JetsToLL";
	string dyj4 = "DY4JetsToLL";
	string ztt = "isZTT";
	string ttw = "TTWJetsToLNu";
	string signal = "stau";
	string signalC1 = "Stau";
	string signalChi = "LSP";
	string N0part = "0Parton";
	string N1part = "1Parton";
	string N2part = "2Parton";
	string N3part = "3Parton";
	string N4part = "4Parton";
	string HT0to70 = "0To70";
	string HT70to100 = "70To100";
	string HT100to200 = "100To200";
	string HT200to400 = "200To400";
	string HT400to600 = "400To600";
	string HT600to800 = "600To800";
	string HT800to1200 = "800To1200";

	bool isW0Part = false;
	bool isW1Part = false;
	bool isW2Part = false;
	bool isW3Part = false;
	bool isW4Part = false;
	bool isDY0Part = false;
	bool isDY1Part = false;
	bool isDY2Part = false;
	bool isDY3Part = false;
	bool isDY4Part = false;
	bool isHTz = false;
	bool isHTa = false;
	bool isHTb = false;
	bool isHTc = false;
	bool isHTd = false;
	bool isHTe = false;
	bool isHTf = false;

	if (string::npos != old.find(data)) isData= true;
	if (string::npos != old.find(signal) && string::npos != systematic.find("genMET")) isSignal= true;
	if (string::npos != old.find(signalC1) && string::npos != systematic.find("genMET")) isSignal= true;
	if (string::npos != old.find(signalChi) && string::npos != systematic.find("genMET")) isSignal= true;

	if (string::npos != old.find(signal)) isSUSY = true;
	if (string::npos != old.find(signalC1)) isSUSY = true;
	if (string::npos != old.find(signalChi)) isSUSY = true;

	if (string::npos != old.find(tt)) isTT= true;
	if (string::npos != old.find(wj)) isWJ= true;
	if (string::npos != old.find(ttw)) isWJ= false;
	if (string::npos != old.find(dyj)) isDY= true;
	if (string::npos != old.find(dyjhigh)) isDYhigh= true;
	if (string::npos != old.find(dyjlow) || string::npos != old.find(dyjlow2)) isDYlow= true;
	if (string::npos != old.find(dyj1) || string::npos != old.find(dyj2) || string::npos != old.find(dyj3) || string::npos != old.find(dyj4)) isDYNJ = true;
	if ((string::npos != old.find(wj1) || string::npos != old.find(wj2) || string::npos != old.find(wj3) || string::npos != old.find(wj4)) && string::npos == old.find(ttw)) isWNJ= true;
	isNJ = isDYNJ || isWNJ;

	
	if (isSignal) cout<<" YOU HAVE FOUND ITTTTTTTTT is it signal ???  "<<isSignal<<" systematic "<<systematic<<endl;
	if (isData) cout<<" YOU HAVE FOUND a DATA "<<endl;


//	TString inputRootFile = "/nfs/dust/cms/user/alkaloge/TauAnalysis/new/new/StauAnalysis/CMSSW_8_0_20/src/DesyTauAnalyses/NTupleMaker/data/HIP_mediumID.root";
//	TFile * fileInHIP = new TFile(inputRootFile, "read");

//TGraphAsymmErrors * TGr_hip;
//TGr_hip = new TGraphAsymmErrors();
//TGr_hip = (TGraphAsymmErrors*)fileInHIP->Get("ratio_eta");


    TString fileinName = "/nfs/dust/cms/user/dydukhle/stau2016_for_training/NTupleMaker/test/mutau/Stau100_LSP1_right.root";


	TFile *filein = new TFile(fileinName,"read");
	string fln = filein->GetName();
	//if (string::npos != fln.find(ztt)) isZTT= true;
	//if (string::npos == fln.find(ztt)) isZTT= false;

	TH1D * histW   , *histPt;
	if (systematic == "ScalesUp" ) histW = (TH1D*)filein->Get("mutau/histWeightsScalesUp");
	else if (systematic == "ScalesDown" ) histW = (TH1D*)filein->Get("mutau/histWeightsScalesDown");
	else if (systematic == "PDFUp" ) histW = (TH1D*)filein->Get("mutau/histWeightsPDFUp");
	else if (systematic == "PDFDown" ) histW = (TH1D*)filein->Get("mutau/histWeightsPDFDown");
	else  {histW = (TH1D*)filein->Get("mutau/histWeightsH");}

	if (string::npos != fln.find(N0part) && (isWJ || isWNJ)) isW0Part= true;
	if (string::npos != fln.find(N1part) && (isWJ || isWNJ)) isW1Part= true;
	if (string::npos != fln.find(N2part) && (isWJ || isWNJ)) isW2Part= true;
	if (string::npos != fln.find(N3part) && (isWJ || isWNJ)) isW3Part= true;
	if (string::npos != fln.find(N4part) && (isWJ || isWNJ)) isW4Part= true;
	if (string::npos != fln.find(N0part) && isDYhigh) isDY0Part= true;
	if (string::npos != fln.find(N1part) && isDYhigh) isDY1Part= true;
	if (string::npos != fln.find(N2part) && isDYhigh) isDY2Part= true;
	if (string::npos != fln.find(N3part) && isDYhigh) isDY3Part= true;
	if (string::npos != fln.find(N4part) && isDYhigh) isDY4Part= true;

	if (string::npos != fln.find(HT0to70)) isHTz= true;
	if (string::npos != fln.find(HT70to100)) isHTa= true;
	if (string::npos != fln.find(HT100to200)) isHTb= true;
	if (string::npos != fln.find(HT200to400)) isHTc= true;
	if (string::npos != fln.find(HT400to600)) isHTd= true;
	if (string::npos != fln.find(HT600to800)) isHTe= true;
	if (string::npos != fln.find(HT800to1200)) isHTf= true;

	cout<<" check =============================== "<<fln<<" isWJ "<<isWJ<<" isW1 "<<isW1Part<<" isWNJ "<<isWNJ<<" is HT ? "<<isHTa<<"  "<<isHTb<<endl;

	histW->SetName("histWeightsH");
	histW->SetTitle("histWeightsH");

	if (isSUSY) histPt = (TH1D*)filein->Get("mutau/histPt");
	float Nw = 1;
	//if (isSUSY) { Nw = 1/histPt->GetSumOfWeights(); Nw *=histW->GetSumOfWeights();}

	cout<<" The Systematic "<<systematic<<" histW "<<histW->GetSumOfWeights()<<endl;

	TH1D * histTop , *histTopSq;
	if (isTT) histTop = (TH1D*)filein->Get("mutau/histTopPt");
	if (isTT && systematic == "TopPtUp" ) histTopSq = (TH1D*)filein->Get("mutau/histTopPtSq");

     double MetV_Pt, Tau_Pt, MuV_Pt;
     double dEta, mttotal, dzeta, DiL_M, Dr, MT, mcta, Mt2as, dPhiMuMET,dPhiTauMET;

     TFile *F = new TFile("test.root","recreate");
     TTree *t3f = new TTree("t3f","a friend Tree");
     t3f->Branch("MetV_Pt",&MetV_Pt,"MetV_Pt/I");
     t3f->Branch("MuV_Pt",&MuV_Pt,"MuV_Pt/I");
     t3f->Branch("Tau_Pt",&Tau_Pt,"Tau_Pt/I");

     t3f->Branch("dEta",&dEta,"dEta/I");
     t3f->Branch("mttotal",&mttotal,"mttotal/I");
     t3f->Branch("dzeta",&dzeta,"dzeta/I");

     t3f->Branch("DiL_M",&DiL_M,"DiL_M/I");
     t3f->Branch("Dr",&Dr,"Dr/I");
     t3f->Branch("MT",&MT,"MT/I");

     t3f->Branch("mcta",&mcta,"mcta/I");
     t3f->Branch("Mt2as",&Mt2as,"Mt2as/I");
     t3f->Branch("dPhiMuMET",&dPhiMuMET,"dPhiMuMET/I");

     t3f->Branch("dPhiTauMET",&dPhiTauMET,"dPhiTauMET/I");

	//TH1D * CutFlow = (TH1D*)filein->Get("mutau/CutFlowUnW");
	TFile * file = new TFile("plots.root","recreate");

	//cout<<"  "<<old<<"  "<<datasetName->c_str()<<"  "<<hist_<<"  "<<CutFlow->GetName()<<"  "<<CutFlow->GetBinContent(1)<<endl;

	file->mkdir(Channel.c_str());
	file->cd(Channel.c_str());
	float top_w = 1.;
	Long64_t nentries = fChain->GetEntriesFast();
	SetupHists(CutNumb,"");
//	SetupHistsnSR(CutNumb);

		cout<<" will process "<<nentries<<" dataset "<<datasetName->c_str()<< endl;

	Long64_t nbytes = 0, nb = 0;

	int genHTa=0;
	int genHTb=0;
	int genHTc=0;
	int genHTd=0;
	int genHTlow=0;
	int is0partons=0;
	int is1partons=0;
	int is2partons=0;
	int is3partons=0;
	int is4partons=0;

	SetupTreeTp();

	cout <<isSUSY<<"   "<<old <<endl;
	Long64_t FirstEntrie =0;


	//TFile * Prefiring = new TFile("/nfs/dust/cms/user/bobovnii/MVAstau/CMSSW_9_4_0_patch1/src/DesyTauAnalyses/NTupleMaker/data/Map_Jet_L1IsoEG30eff_bxm1_looseJet_SingleMuon_Run2017F.root","read");
	//TEfficiency* pEff = (TEfficiency*)Prefiring->Get("prefireEfficiencyMap");
	//TH2D* hist2 = (TH2D*) pEff->GetTotalHistogram();

	for (Long64_t jentry=0; jentry<nentries;jentry++) {
	   // if (jentry>1000) break;
		Long64_t ientry = LoadTree(jentry);
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		float all_weights = 1.;

		float charge_ =mu_charge[muon_index]  * ta_charge[taus_index];

		if ( charge_ < 0. && Sign == "SS") continue;
		if ( charge_ > 0. && Sign == "OS") continue;

		if ( jentry%100000 ==0 ) cout<<" working for you...processed " <<jentry<<" events from "<<nentries<<" dataset "<<datasetName->c_str()<<" weight "<<all_weights<< endl;

		if (isWJ && (npartons>0 && npartons<5)) continue;

		//if (isDY && (npartons>0 && npartons<5)) continue;
		if ( isDYhigh  && (npartons>0 && npartons<5)) continue;

		if (isData && met_flag<0.5) continue;	

		JetsMV.clear();
		MuMV.clear();
		ElMV.clear();
		TauMV.clear();
		btag_index.clear();

			JetsV.SetPxPyPzE(0.,0.,0.,0.);
			MuV.SetPxPyPzE(0.,0.,0.,0.);
			ElV.SetPxPyPzE(0.,0.,0.,0.);
			TauV.SetPxPyPzE(0.,0.,0.,0.);

	for (int nj=0;nj<njets;++nj) {
	int jet_index = jets_cleaned[nj];
	JetsV.SetPxPyPzE(0.,0.,0.,0.);
	JetsV.SetPxPyPzE(jet_px[jet_index], jet_py[jet_index],jet_pz[jet_index],jet_e[jet_index]);
	JetsMV.push_back(JetsV);
	}

	float met = -1;

		METV.SetPx(met_ex);
		METV.SetPy(met_ey);
		met= sqrt(met_ex*met_ex + met_ey*met_ey);  

	if (systematic =="nominal")
		{
		METV.SetPx(met_ex);
		METV.SetPy(met_ey);
		met = sqrt(met_ex*met_ex + met_ey*met_ey);  
		}

//JetEnUp JetEnDown UnclEnUp UnclEnDown
	if (systematic =="JetEnUp")
		{
		METV.SetPx(met_ex_JetEnUp);
		METV.SetPy(met_ey_JetEnUp);
		met = sqrt(met_ex_JetEnUp*met_ex_JetEnUp + met_ey_JetEnUp*met_ey_JetEnUp);      
		}

	if (systematic =="JetEnDown")
		{
		METV.SetPy(met_ey_JetEnDown);
		met = sqrt(met_ex_JetEnDown*met_ex_JetEnDown + met_ey_JetEnDown*met_ey_JetEnDown);     
		}

	if (systematic =="UnclEnUp")
		{
		METV.SetPx(met_ex_UnclusteredEnUp);
		METV.SetPy(met_ey_UnclusteredEnUp);
		met = sqrt(met_ex_UnclusteredEnUp*met_ex_UnclusteredEnUp + met_ey_UnclusteredEnUp*met_ey_UnclusteredEnUp);     
		}

	if (systematic =="UnclEnDown")
		{
		METV.SetPx(met_ex_UnclusteredEnDown);
		METV.SetPy(met_ey_UnclusteredEnDown);
		met = sqrt(met_ex_UnclusteredEnDown*met_ex_UnclusteredEnDown + met_ey_UnclusteredEnDown*met_ey_UnclusteredEnDown);
		}


		if (abs(mu_count) > 8 || abs(el_count) > 8 || abs(ta_count) >8) continue;

if(muon_index>-1 && muon_index<8)			MuV.SetPtEtaPhiM(mu_pt[muon_index], mu_eta[muon_index], mu_phi[muon_index], muonMass);
if(electron_index>-1 && electron_index<8)		ElV.SetPtEtaPhiM(el_pt[electron_index], el_eta[electron_index], el_phi[electron_index],electronMass);
if (taus_index>-1 && taus_index<8)			TauV.SetPtEtaPhiM(ta_pt[taus_index], ta_eta[taus_index], ta_phi[taus_index], ta_mass[taus_index]);


		bool isSignal = false;
				bool isRealTau  = (genTauMatched>0.5 && !isData) ;


		int iCut = 1;

		all_weights = pu_weight * gen_weight ;

		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index, isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;
		
		if (mu_relIso[0] > 0.15 && !InvertLeptonIso) continue;
		if (mu_relIso[0] < 0.15 && InvertLeptonIso) continue;
		
		//if (mu_relIso[0] > 0.5 && InvertLeptonIso && HighInvertLeptonIso) continue;
		if (mu_relIso[0] > 0.3) continue;

		if ( fabs(mu_dz[muon_index]) > 0.2) continue;
		if ( fabs(mu_dxy[muon_index]) > 0.045) continue;

		if ( fabs(mu_charge[muon_index]) !=1 ) continue;
		if ( fabs(ta_charge[taus_index]) !=1 ) continue;

                double Dr=deltaR(MuV.Eta(), MuV.Phi(),
					TauV.Eta(),TauV.Phi());
		if (Dr>3.5 && !InvertLeptonIso) continue;
		if (Dr<3.5 && InvertLeptonIso) continue;

		double dPhiJM = 0;
		if (JetsMV.size()>0)
     		dPhiJM=dPhiFrom2P( JetsMV.at(0).Px(), JetsMV.at(0).Py(), METV.Px(),  METV.Py() );

//		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;


		if (event_secondLeptonVeto >0.5) continue;
//		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;

		if (event_thirdLeptonVeto >0.5) continue;
//		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;
		
		all_weights *= trig_weight;
//		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;

		all_weights *= LSF_weight;
//		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;
		//cout <<"isfakeTauFromJet  "<<isfakeTauFromJet << " isfakeTauFromPromptLepton  "<< isfakeTauFromPromptLepton<< "  isElPromptLepton  " <<isElPromptLepton<< endl;

//////////////////////////////////////////////////////////////////////////////

		string tID="MVA_Tight";
		double tauId = TauiD(channel,tID);
		if (isRealTau && !isData) all_weights *= 0.86;
		
        //        FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;


		//string WP = systematic;
//		double tfr  = TauFakeRate(ptTau1,etaTau1,channel,systematic);
//		if (!isRealTau && isfakeTauFromJet  && !isData) all_weights *= tfr;
		//FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;

//		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;



		if (isWJ || isDY || isNJ) {
		
		METV.SetPx(0.);
		METV.SetPy(0.);
		METV.SetPx(met_ex_recoil);
		METV.SetPy(met_ey_recoil);
		met=sqrt(met_ex_recoil*met_ex_recoil+met_ey_recoil*met_ey_recoil);
		}

		//FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;

		if (isDY || isDYNJ) all_weights *=zptmassweight;
		

	//	FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//if (njets>2.5 || njets<1.5) continue;
		if (njets>1.5) continue;
        if (TauV.Pt()<30) continue;
    	TLorentzVector DiL2 = MuV  + TauV;
		if (DiL2.M()<50) continue;

//		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;

		if (nbtag>0.5) continue;
		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;



		 TLorentzVector DiL = MuV  ;
     		double dPhi=dPhiFrom2P( DiL.Px(), DiL.Py(), METV.Px(),  METV.Py() );
         	double MT = 0;MT=sqrt(2*DiL.Pt()*met*(1-TMath::Cos(dPhi)));
     		double dPhiT=dPhiFrom2P( TauV.Px(), TauV.Py(), METV.Px(),  METV.Py() );
         	double MTt = 0;MTt=sqrt(2*TauV.Pt()*met*(1-TMath::Cos(dPhiT)));
		double MTsum = MT+MTt;
	     	
		if (MT > 60. && MT < 120. ) continue;
        if (MT < 20.) continue;
		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index, isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;


       if (njets>0.5) continue;
	    if (Dr < 2.) continue;
		double dPhiDiL=dPhiFrom2P( MuV.Px(), MuV.Py(), TauV.Px(),  TauV.Py() );
		if (dPhiDiL < 1.5) continue;

		//FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;

		double dEta = MuV.Eta() - TauV.Eta();
		double dEtaJMu = -1.; if (JetsMV.size()>0) dEtaJMu = JetsMV.at(0).Eta() - MuV.Eta();
		double dEtaJTau = -1.; if (JetsMV.size()>0) dEtaJTau = JetsMV.at(0).Eta() - TauV.Eta();
                double Dr2=-1; if  (JetsMV.size()>0) {Dr2=deltaR(JetsMV.at(0).Eta(),JetsMV.at(0).Phi(), TauV.Eta(),TauV.Phi());}




////////////// BDT calculation


	double dPhiMuMET=dPhiFrom2P( MuV.Px(), MuV.Py(), METV.Px(),  METV.Py() );
	double dPhiTauMET=dPhiFrom2P( TauV.Px(), TauV.Py(), METV.Px(),  METV.Py() );
	float mttotal = sqrt(MT*MT + MTt*MTt);
	double Mt2as = asymm_mt2_lester_bisect::get_mT2(muonMass, MuV.Px(), MuV.Py(),tauMass,TauV.Px(),TauV.Py(),METV.Px(),METV.Py(),0,0,0);
	float tauUnitX = TauV.Px()/TauV.Pt();
	float tauUnitY = TauV.Py()/TauV.Pt();

	float muonUnitX = MuV.Px()/MuV.Pt();
	float muonUnitY = MuV.Py()/MuV.Pt();

	float zetaX = tauUnitX + muonUnitX;
	float zetaY = tauUnitY + muonUnitY;

	float normZeta = TMath::Sqrt(zetaX*zetaX+zetaY*zetaY);

	zetaX = zetaX/normZeta;
	zetaY = zetaY/normZeta;

	float vectorVisX = MuV.Px() + TauV.Px();
	float vectorVisY = MuV.Py() + TauV.Py();

	// computation of DZeta variable
	// pfmet
	float pzetamiss = METV.Px()*zetaX + METV.Py()*zetaY;
	float pzetavis = vectorVisX*zetaX + vectorVisY*zetaY;
	float dzeta = pzetamiss - 0.85*pzetavis;

	DiL = MuV  + TauV;
	double mcta = sqrt( 2*MuV.Pt()*TauV.Pt()*(1+cos(MuV.Phi()-TauV.Phi())) );


            vector<float> new_inputVec;
            new_inputVec.push_back(METV.Pt());
            new_inputVec.push_back(MuV.Pt());
            new_inputVec.push_back(TauV.Pt());
            new_inputVec.push_back(dEta);
            new_inputVec.push_back(mttotal);
            new_inputVec.push_back(dzeta);
            new_inputVec.push_back(Dr);
            new_inputVec.push_back(DiL.M());
            new_inputVec.push_back(MT);
            new_inputVec.push_back(mcta);
            new_inputVec.push_back(Mt2as);
            new_inputVec.push_back(dPhiMuMET);
            new_inputVec.push_back(dPhiTauMET);

			(*inputVec)[0] = METV.Pt();
			(*inputVec)[1] = MuV.Pt();
			(*inputVec)[2] = TauV.Pt();
			(*inputVec)[3] = dEta;
			(*inputVec)[4] = mttotal;
			(*inputVec)[5] = dzeta;
			(*inputVec)[6] = Dr;
			(*inputVec)[7] = DiL.M();
			(*inputVec)[8] = MT;
			(*inputVec)[9] = mcta;
			(*inputVec)[10] = Mt2as;
			(*inputVec)[11] = dPhiMuMET;
            (*inputVec)[12] = dPhiTauMET;


             //MetV_Pt =  METV.Pt();
             //V_Pt = MuV.Pt();
             //Tau_Pt = TauV.Pt();
             //DiL_M = DiL.M();
             //t3f->Fill();

            DataChunk *sample = new DataChunkFlat(13, 0);
            sample->set_data(new_inputVec);


            vector<float> responce = ResponseMutau.compute_output(sample);
            cout<<"Responce "<< responce[0] <<endl;


/*			double retval = BDTResponseMutau_stau100_LSP1My->GetMvaValue( *inputVec );



if (retval>0.1)		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
if (retval>0.1)			CFCounter[iCut]+= all_weights;
if (retval>0.1)			CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;

x


if (retval<0.1)		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
if (retval<0.1)			CFCounter[iCut]+= all_weights;
if (retval<0.1)			CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;

	//	if ( fabs(mu_dz[muon_index]) > 0.04) continue;
	//	if ( fabs(mu_dxy[muon_index]) > 0.02) continue;
	//	if (ta_relIso[0] < 0.85) continue;

		 TLorentzVector DiLL = MuV +TauV ;
		if (fabs(dEta)>2) continue;
		if (DiLL.M()<20.) continue;
		if (MTsum<30.) continue;

//		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;

		if (fabs(dEta)>1.5) continue;
		if ( ta_relIso[0] < 0.85) continue;
		if (Dr<2.) continue;
		if (Dr>3.) continue;
		if (DiLL.M()<50.) continue;
		if (MTsum<50.) continue;
//		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;

		if (met<20.) continue;
//		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;

		if (met<30.) continue;
//		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;

		if (met<40.) continue;
//		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;

		continue;

		if (met<50.) continue;
//		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;


		if (ta_pt[taus_index] < 50. ) continue;
//		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;

		
		Tp->Fill();*/
	
  }

	cout<<" and about HT now.... <70 "<<genHTlow<<" 70<x<100 "<<genHTa<<" 100x<200 "<<genHTb<<" 200<x<400 "<<genHTc<<" 400<x<600  "<<genHTd<<" 0partons "<<is0partons<<" 1p  "<<is1partons<<" 2p "<<is2partons<<" 3p "<<is3partons<<" 4p "<<is4partons<<endl;

	float Lumi = 35864.;

	if (isData) Lumi = 1.;

		 for(int ci = 0; ci < CutNumb; ci++)
                        {
                                // tfile << CutList[ci]<<"\t & \t"
                                //          << CFCounter[ci]  <<"\t & \t"<< statUnc[ci] <<"\t & \t"<< iCFCounter[ci] << endl;
                                		CutFlowUnW->SetBinContent(1+ci,0);
                        if (CutNumb < 2)       CutFlowUnW->SetBinContent(1+ci,CFCounter_[ci]);
                           else  
 					CutFlowUnW->SetBinContent(1+ci,float(CFCounter[ci]) );
				

					//cout<<" Count "<<ci<<" binContent "<<CutFlowUnW->GetBinContent(1+ci)<<"  CFCounter_ "<<CFCounter_[ci]<<" CFCounter "<<CFCounter[ci]<<" iCFCounter "<<iCFCounter[ci]<<" Name "<<CutFlowUnW->GetXaxis()->GetBinLabel(1+ci)<<" entry  "<<CutFlowUnW->GetBinContent(1+ci)<<" prediction for Lumi " <<Lumi<<"  "<<float (CutFlowUnW->GetBinContent(1+ci))*hxsecW->GetMean()*Lumi/histW->GetSumOfWeights()<<endl;			
					cout<<" Count "<<ci<<" binContent "<<CutFlowUnW->GetBinContent(1+ci)<<"  CFCounter_ "<<CFCounter_[ci]<<" CFCounter "<<CFCounter[ci]<<" iCFCounter "<<iCFCounter[ci]<<" Name "<<CutFlowUnW->GetXaxis()->GetBinLabel(1+ci)<<" entry  "<<CutFlowUnW->GetBinContent(1+ci)<<endl;			

}


			cout<<"  end from TH1D "<<endl;

		//hcount->Draw();
		CutFlowUnW->Write();
		histW->Write();
	if (isTT) histTop->Write();
	if (isTT && systematic == "TopPtUp"  ) histTopSq->Write();
	//	Tp->Write();
	file->Write();
	file->Close();

    F->cd();
    t3f->Write();
	    //VectorTuple->Close();
    F->Close();

		cout<<" Failed events  "<<counter<<endl;
	}
/////////////////////////////////////////////

