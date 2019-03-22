#define analyzer_cxx
#include "analyzer.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TString.h>
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
cout <<"AAAAAAAAAAAAAAAAAAAAAAAAAAAAa" <<endl;

	if (fChain == 0) return;
cout <<"AAAAAAAAAAAAAAAAAAAAAAAAAAAAa" <<endl;

	CutList.clear();
	CutList.push_back("Nocut");
	CutList.push_back("PU");
	CutList.push_back("#mu#tau");
	CutList.push_back("2ndlepV");
	CutList.push_back("3rdlepV");
	CutList.push_back("TriggerSF");
	CutList.push_back("LeptonSF");
	CutList.push_back("Tau-id");
	CutList.push_back("TauFakeRate");
	CutList.push_back("MuTauFakeRate");
	CutList.push_back("METrecoil");
	CutList.push_back("ZpTcorrection");
	CutList.push_back("topPtRwgt");
	CutList.push_back("Jets<2 ");
	CutList.push_back("btag=0");
	CutList.push_back("MTwindowANDnominalTauID"); //15 
	CutList.push_back("MTwindowANDinvTauID");
	CutList.push_back("invMTwindowANDnominalTauID");
	CutList.push_back("invMTwindowANDinvTauID");
	CutList.push_back("MET>20");
	CutList.push_back("MET>30");
	CutList.push_back("MET>40");
	CutList.push_back("MTsum>120");

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
		
	bool ifCutBDT = true;
	double CutBDT = 0.236;

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
	bool isDYNJ = false;
	bool isZTT = false;
	bool isWNJ = false;
	bool isNJ = false;
	bool isSignal = false;
	bool cut_btag = true;
	
	bool isWith1jet=false;
	bool isWith2jet=false;
	bool isWith3jet=false;
	bool isWith4jet=false;


	bool t = true;
	bool f = false;
	bool InvertLeptonIso = LEPTONHERE;
	//bool HighInvertLeptonIso = HIGHISO;
	string Sign = "SIGNHERE";
	
	
	string systematic="Nominal";


	string cut = to_string(t);
	cout <<"  "<<cut.c_str()<<endl;
	char plots_[100];
	string data = "Single";
	string tt = "TT_TuneCUETP8M2T4_13TeV-powheg-pythia8";
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
	string signal = "stau";
	string signalC1 = "C1";

	string with1jet = "with1jet";
	string with2jet = "with2jet";
	string with3jet = "with3jet";
	string with4jet = "with4jet";
	if (string::npos != old.find(data)) isData= true;
	if (string::npos != old.find(signal)) isSignal= true;
	if (string::npos != old.find(signalC1)) isSignal= true;
	if (string::npos != old.find(tt)) isTT= true;
	if (string::npos != old.find(wj)) isWJ= true;
	if (string::npos != old.find(dyj)) isDY= true;
	if (string::npos != old.find(dyjhigh)) isDYhigh= true;
	if (string::npos != old.find(dyjlow) || string::npos != old.find(dyjlow2)) isDYlow= true;
	if (string::npos != old.find(dyj1) || string::npos != old.find(dyj2) || string::npos != old.find(dyj3) || string::npos != old.find(dyj4)) isDYNJ = true;
	if (string::npos != old.find(wj1) || string::npos != old.find(wj2) || string::npos != old.find(wj3) || string::npos != old.find(wj4)) isWNJ= true;

	isNJ = isDYNJ && isWNJ;
	
	if (isSignal) cout<<" YOU HAVE FOUND ITTTTTTTTT "<<endl;
	if (isData) cout<<" YOU HAVE FOUND a DATA "<<endl;

	string channel = "mutau";

	TString inputRootFile = "/nfs/dust/cms/user/bobovnii/CMSSW_8_0_25/src/DesyTauAnalyses/NTupleMaker//data/HIP_mediumID.root";
	TFile * fileInHIP = new TFile(inputRootFile, "read");

TGraphAsymmErrors * TGr_hip;
TGr_hip = new TGraphAsymmErrors();
TGr_hip = (TGraphAsymmErrors*)fileInHIP->Get("ratio_eta");


	TString inputRootFile2 = "/nfs/dust/cms/user/bobovnii/CMSSW_8_0_25/src/DesyTauAnalyses/NTupleMaker/data/sf_mu_medium.root";

	TFile * fileInFast = new TFile(inputRootFile2, "read");


TIter nextkey (fileInFast->GetListOfKeys ());
TKey *key = 0;
key = (TKey *) nextkey ();
TObject *obj = key->ReadObj ();
TH2D *histo = (TH2D*) obj;

	TString fileinName = "/nfs/dust/cms/user/bobovnii/CMSSW_8_0_25/src/DesyTauAnalyses/NTupleMaker/test/mutau/FILEIN";
	if (systematic == "JetEnUp" || systematic == "JetEnDown"  || systematic == "UnclEnDown" || systematic == "UnclEnDown" || systematic == "TauEnUp" || systematic == "TauEnDown" || systematic == "ElEnUp" || systematic == "ElEnDown" || systematic == "MuEnUp" || systematic == "MuEnDown") 
	{fileinName ="/nfs/dust/cms/user/bobovnii/CMSSW_8_0_25/src/DesyTauAnalyses/NTupleMaker/test/mutau_"+systematic+"/FILEIN";}


	string fileName = "FILEIN";
	if (string::npos != fileName.find(with1jet)) isWith1jet= true;
	if (string::npos != fileName.find(with2jet)) isWith2jet= true;
	if (string::npos != fileName.find(with3jet)) isWith3jet= true;
	if (string::npos != fileName.find(with4jet)) isWith4jet= true;


	TFile * filein = new TFile(fileinName,"read");

	string fln = filein->GetName();
	if (string::npos != fln.find(ztt)) isZTT= true;
	if (string::npos == fln.find(ztt)) isZTT= false;

	cout<<" ============================================ is it ZTT ???  "<<isZTT<<" old "<<fln<<endl;
	TH1D * histW = (TH1D*)filein->Get("mutau/histWeightsH");


///// for mixing inclusive and Njets samples
	if (isWith1jet && isDYhigh) histW->Scale((1.1637*1012.5)/5765.4);
	if (isWith2jet && isDYhigh) histW->Scale((1.1637*332.8)/5765.4);
	if (isWith3jet && isDYhigh) histW->Scale((1.1637*101.8)/5765.4);
	if (isWith4jet && isDYhigh) histW->Scale((1.1637*54.8)/5765.4);

	if (isWith1jet && isWJ) histW->Scale((1.221*9644.5)/61526.7);
	if (isWith2jet && isWJ) histW->Scale((1.221*3144.5)/61526.7);
	if (isWith3jet && isWJ) histW->Scale((1.221*954.8)/61526.7);
	if (isWith4jet && isWJ) histW->Scale((1.221*485.6)/61526.7);

	TH1D * histTop , *histTopSq;
	if (isTT) histTop = (TH1D*)filein->Get("mutau/histTopPt");
	if (isTT && systematic == "TopPtUp" ) histTopSq = (TH1D*)filein->Get("mutau/histTopPtSq");
	
	TH1D * CutFlow = (TH1D*)filein->Get("mutau/CutFlowUnW");
	TFile * file = new TFile("plots.root","recreate");

	//cout<<"  "<<old<<"  "<<datasetName->c_str()<<"  "<<hist_<<"  "<<CutFlow->GetName()<<"  "<<CutFlow->GetBinContent(1)<<endl;
		
	string Channel = "mutau";
	file->mkdir(Channel.c_str());
	file->cd(Channel.c_str());
	float top_w = 1.;
	Long64_t nentries = fChain->GetEntriesFast();
	SetupHists(CutNumb);

		cout<<" will process "<<nentries<<" dataset "<<datasetName->c_str()<< endl;

	Long64_t nbytes = 0, nb = 0;


	//for (Long64_t jentry=0; jentry<1000;jentry++) {
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		float all_weights = 1.;


		bool isDYTauTau = false;
		bool isDYll = false;

		if ((isDY  ||  isDYNJ) && isDYTT>0.5) isDYTauTau = true;
		if ((isDY  ||  isDYNJ) && isDYEE>0.5) isDYee = true;
		if ((isDY  ||  isDYNJ) && isDYMM>0.5) isDYmm = true;
		if ((isDY  ||  isDYNJ) && isDYNuNu>0.5) isDYnunu = true;

		if ((isDY  ||  isDYNJ) && isDYTT<0.5) isDYll = true;
		float charge_ =mu_charge[muon_index]  * ta_charge[taus_index];

		//cout<<" Again ..................... "<<isZTT<<" DYTT "<<isDYTauTau<<endl;
		if ((isDY || isDYNJ) && isZTT && !isDYTauTau) continue;
		if ((isDY || isDYNJ) && !isZTT && !isDYll) continue;

		if ( charge_ < 0. && Sign == "SS") continue;
		if ( charge_ > 0. && Sign == "OS") continue;
		//if ( charge_ > 0.) continue;		


		if ( jentry%1000 ==0 ) cout<<" working for you...processed " <<jentry<<" events from "<<nentries<<" dataset "<<datasetName->c_str()<<" weight "<<all_weights<< endl;

		if (isWJ && (npartons>0 && npartons<5) && !(isWith1jet || isWith2jet || isWith3jet || isWith4jet)) continue;
		if ( isDYhigh  && (npartons>0 && npartons<5) && !(isWith1jet || isWith2jet || isWith3jet || isWith4jet)) continue;

		if (isWith1jet && npartons != 1) continue;
		if (isWith2jet && npartons != 2) continue;
		if (isWith3jet && npartons != 3) continue;
		if (isWith4jet && npartons != 4) continue;

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


//JetEnUp JetEnDown UnclEnUp UnclEnDown
	if (systematic =="JetEnUp")
		{
		METV.SetPx(met_ex_JetEnUp);
		METV.SetPy(met_ey_JetEnUp);
		met = sqrt(met_ex_JetEnUp*met_ex_JetEnUp + met_ey_JetEnUp*met_ey_JetEnUp);      
		}

	if (systematic =="JetEnDown")
		{
		METV.SetPx(met_ex_JetEnDown);
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

if(muon_index>-1 )			MuV.SetPtEtaPhiM(mu_pt[muon_index], mu_eta[muon_index], mu_phi[muon_index], muonMass);
if(electron_index>-1 )		ElV.SetPtEtaPhiM(el_pt[electron_index], el_eta[electron_index], el_phi[electron_index],electronMass);
if (taus_index>-1 )			TauV.SetPtEtaPhiM(ta_pt[taus_index], ta_eta[taus_index], ta_phi[taus_index], ta_mass[taus_index]);

		int iCut = 1;

		all_weights = pu_weight * gen_weight ;

		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;
		if (mu_relIso[0] > 0.15 && !InvertLeptonIso) continue;
		//if (mu_relIso[0] > 0.15) continue;
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


		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;

		if (event_secondLeptonVeto >0.5) continue;
		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;

		if (event_thirdLeptonVeto >0.5) continue;
		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;
		
		if (!isData) all_weights *= trig_weight;
		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;

		if (!isData) all_weights *= LSF_weight;
		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;

		double ptTau1 = (double)ta_pt[taus_index];
		double etaTau1 = (double)ta_eta[taus_index];
		double ptMu1 = (double)mu_pt[muon_index];
		double etaMu1 = (double)mu_eta[muon_index];

		double ptN=0;double etaN=0;double FastSF=0;

		ptN = histo->GetXaxis()->FindBin(mu_pt[muon_index]);
		etaN = histo->GetYaxis()->FindBin(fabs(mu_eta[muon_index]));
		FastSF = histo->GetBinContent(ptN,etaN);
	
//	if (isSignal) all_weights *= FastSF;
		//int binNumber = TGr_hip->GetXaxis()->FindBin(mu_eta[muon_index]);

		//string tID="CutBased_Medium";
		string tID="MVA_Tight";
		double tauId = TauiD(channel,tID);
		//if (genTauMatched>0.5 && genLeptonMatched < 0.5 && !isData) all_weights *= tauId;
		if (genTauMatched>0.5 && genLeptonMatched < 0.5 && !isData) all_weights *= 0.9;
		
                FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;


		string WP = "MVA";
		double tfr  = TauFakeRate(ptTau1,etaTau1,channel,WP);
		if (genTauMatched<0.5 && genLeptonMatched < 0.5  && !isData) all_weights *= tfr;
		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;

		//reco tau to matched genmu but not matched tau which is decayed hadronically or electron

		double tfrmu  = MuonFakeRate(ptMu1,etaMu1);
		double tfrel  = ElectronFakeRate(ptMu1,etaMu1);
	//	if (genTauMatched<0.5 && genLeptonMatchedMu > 0.5 && matchedTauToPromptMu < 0.5 && !isData) all_weights *= tfrmu;
	//	if ( (genLeptonMatched > 0.5 || matchedTauToPromptMu > 0.5) && !isData) all_weights *= tfrmu;


		if ( (matchedTauToPromptMu > 0.5 && matchedTauToPromptEl < 0.5) && !isData) all_weights *= tfrmu;
		if ( (matchedTauToPromptEl > 0.5 && matchedTauToPromptMu < 0.5) && !isData) all_weights *= tfrel;
		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;



		if (isWJ || isDY || isWNJ || isDYNJ) {
		
		METV.SetPx(0.);
		METV.SetPy(0.);
		METV.SetPx(met_ex_recoil);
		METV.SetPy(met_ey_recoil);
		met=sqrt(met_ex_recoil*met_ex_recoil+met_ey_recoil*met_ey_recoil);

	if (systematic =="JetEnUp")
		{
		METV.SetPx(met_ex_JetEnUp_recoil);
		METV.SetPy(met_ey_JetEnUp_recoil);
		met = sqrt(met_ex_JetEnUp_recoil*met_ex_JetEnUp_recoil + met_ey_JetEnUp_recoil*met_ey_JetEnUp_recoil);      
		}

	if (systematic =="JetEnDown")
		{
		METV.SetPx(met_ex_JetEnDown_recoil);
		METV.SetPy(met_ey_JetEnDown_recoil);
		met = sqrt(met_ex_JetEnDown_recoil*met_ex_JetEnDown_recoil + met_ey_JetEnDown_recoil*met_ey_JetEnDown_recoil);     
		}

	if (systematic =="UnclEnUp")
		{
		METV.SetPx(met_ex_UnclusteredEnUp_recoil);
		METV.SetPy(met_ey_UnclusteredEnUp_recoil);
		met = sqrt(met_ex_UnclusteredEnUp_recoil*met_ex_UnclusteredEnUp_recoil + met_ey_UnclusteredEnUp_recoil*met_ey_UnclusteredEnUp_recoil);     
		}

	if (systematic =="UnclEnDown")
		{
		METV.SetPx(met_ex_UnclusteredEnDown_recoil);
		METV.SetPy(met_ey_UnclusteredEnDown_recoil);
		met = sqrt(met_ex_UnclusteredEnDown_recoil*met_ex_UnclusteredEnDown_recoil + met_ey_UnclusteredEnDown_recoil*met_ey_UnclusteredEnDown_recoil);
		}

		}




		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;

	if (isDY || isDYNJ){
		if (systematic !="ZPtUp" && systematic !="ZPtDown" ) all_weights *= zptmassweight;
		if (systematic =="ZPtUp") all_weights *= zptmassweight*zptmassweight;
		}


		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;

		if (systematic !="TopPtUp" && systematic !="TopPtDown" ) all_weights *= top_weight;
		if (systematic =="TopPtUp") all_weights *= top_weight*top_weight;
		if (systematic =="TopPtDown") all_weights *= 1;
		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if (njets>1) continue;
		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;

		if (nbtag!=0) continue;
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
	     	
		//if (MT > 60. && MT < 120. ) continue;
		if (MT < 20.) continue;


		if ( (MT < 60. ||  MT > 120.) && TightID>0.5)
			{		
		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
			}
		iCut++;

		if ( (MT < 60. || MT > 120.) && TightID<0.5 && LooseID>0.5)
			{		
		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
			}
		iCut++;
		
		if (( MT > 60. && MT < 120.) && TightID>0.5)
			{		
		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
			}
		iCut++;
		if (( MT > 60. && MT < 120.) && TightID<0.5 && LooseID>0.5)
			{		
		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
			}
		iCut++;



		 TLorentzVector DiLL = MuV +TauV ;
		double retvalBDT =0;

////// To cut on BDT value

	if (ifCutBDT)

	{
			float mttotal = sqrt(MT*MT + MTt*MTt);
			double deta=deltaEta(MuV.Px(),  MuV.Py(),MuV.Pz(), TauV.Px(),  TauV.Py(),TauV.Pz() );


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
				double Mt2as = 0; 
				
				Mt2as = asymm_mt2_lester_bisect::get_mT2(muonMass, MuV.Px(), MuV.Py(),tauMass,TauV.Px(),TauV.Py(),METV.Px(),METV.Py(),0,0,0);
      				double Dr = MuV.DeltaR(TauV);

//cout<< "!!!" << endl;
/*
cout << 			"0  "<< METV.Pt()
			<<"   1  "<< MuV.Pt()
			<<"   2  "<< TauV.Pt()
			<<"   3  "<< njets
			<<"   4  "<< deta
			<<"   5  "<< mttotal
			<<"   6  "<< dzeta
			<<"   7  "<< DiL.M()
			<<"   8  "<< Dr
			<<"   9  "<< MT
			<<"   10  "<< mcta
			<<"   11  "<< Mt2as <<endl;
cout << "AAAAAAA??"<< endl;
*/
			(*inputVec)[0] = METV.Pt();
			(*inputVec)[1] = MuV.Pt();
			(*inputVec)[2] = TauV.Pt();
			(*inputVec)[3] = njets;
			(*inputVec)[4] = deta;
			(*inputVec)[5] = mttotal;
			(*inputVec)[6] = dzeta;
			(*inputVec)[7] = DiL.M();
			(*inputVec)[8] = Dr;
			(*inputVec)[9] = MT;
			(*inputVec)[10] = mcta;
			(*inputVec)[11] = Mt2as;

//cout << "AAAAAAA??"<< endl;

//cout<<"!!!!!!!!!!!" <<endl;

			retvalBDT = BDTResponseMutau->GetMvaValue( *inputVec );
//cout<<retvalBDT <<endl;	
			
			if (retvalBDT > CutBDT) {	FillHists(20, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal); /*cout<<"fill 20" <<endl;*/}
//cout<<"????" <<endl;
			if (retvalBDT < CutBDT) {	FillHists(21, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);/*cout<<"fill 21" <<endl;*/}
//cout<<"????" <<endl;

	}




                 
if (ifCutBDT) continue;

		double dEta = MuV.Eta() - TauV.Eta();
		double dEtaJMu = -1.; if (JetsMV.size()>0) dEtaJMu = JetsMV.at(0).Eta() - MuV.Eta();
		double dEtaJTau = -1.; if (JetsMV.size()>0) dEtaJTau = JetsMV.at(0).Eta() - TauV.Eta();
                double Dr2=-1; if  (JetsMV.size()>0) {Dr2=deltaR(JetsMV.at(0).Eta(),JetsMV.at(0).Phi(), TauV.Eta(),TauV.Phi());}
		double dPhiDiL=dPhiFrom2P( MuV.Px(), MuV.Py(), TauV.Px(),  TauV.Py() );


		if (fabs(dEta)>2 &&  !InvertLeptonIso) continue;
		if (DiLL.M()<20.) continue;
		if (MTsum<30.) continue;

		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;


		if (JetsMV.size()>0 && fabs(dEtaJMu)>3.) continue;
		if (JetsMV.size()>0 && fabs(dEtaJTau)>3.) continue;
		if (JetsMV.size()>0 && Dr2>4.) continue;

		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;

		if ( fabs(mu_dz[muon_index]) > 0.04) continue;
		if ( fabs(mu_dxy[muon_index]) > 0.02) continue;
		if (ta_relIso[0] < 0.85) continue;

		if (fabs(dEta)>1.5 && !InvertLeptonIso) continue;
		if (fabs(dEta)<2 && InvertLeptonIso) continue;
		if (dPhiDiL<1.5) continue;
		if (dPhiDiL>3.2) continue;
		if (Dr<2.) continue;
		if (Dr>3.2 && !InvertLeptonIso) continue;
		if (Dr<3.5 && InvertLeptonIso) continue;

		if (DiLL.M()<50.) continue;
		if (MTsum<50.) continue;

		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;

		if (met<20.) continue;
		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;




		if (met<30.) continue;
		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;

		if (met<40.) continue;
		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;
	
		if (MTsum<120.) continue;
		FillHists(iCut, all_weights, ElV, MuV, TauV,JetsMV, METV, ChiMass,mIntermediate, Channel, muon_index,electron_index,taus_index,isSignal);
		CFCounter[iCut]+= all_weights;
		CFCounter_[iCut]+= all_weights;
                iCFCounter[iCut]++;
		iCut++;
		
	
	
  }


	float Lumi = 36590.;

	if (isData) Lumi = 1.;
		 for(int ci = 0; ci < CutNumb; ci++)
                        {
                                // tfile << CutList[ci]<<"\t & \t"
                                //          << CFCounter[ci]  <<"\t & \t"<< statUnc[ci] <<"\t & \t"<< iCFCounter[ci] << endl;
                                		CutFlowUnW->SetBinContent(1+ci,0);
                        if (CutNumb < 2)       CutFlowUnW->SetBinContent(1+ci,CFCounter_[ci]);
                           else  
 					CutFlowUnW->SetBinContent(1+ci,float(CFCounter[ci]) );
				

					cout<<" Count "<<ci<<" binContent "<<CutFlowUnW->GetBinContent(1+ci)<<"  CFCounter_ "<<CFCounter_[ci]<<" CFCounter "<<CFCounter[ci]<<" iCFCounter "<<iCFCounter[ci]<<" Name "<<CutFlowUnW->GetXaxis()->GetBinLabel(1+ci)<<" entry  "<<CutFlowUnW->GetBinContent(1+ci)<<endl;			

}
/*

	for (int jj = 3; jj < 25 ; jj++)
	
	{
	cout <<jj<<"  "<< vBDTlog[jj].empty()<<endl; 
	if (!vBDTlog[jj].empty())
	{
	double max = *max_element(vBDTlog[jj].begin(), vBDTlog[jj].end());
	double min = *min_element(vBDTlog[jj].begin(), vBDTlog[jj].end());
cout <<jj<<" max  "<< max <<"  min  "<<min << endl;
max = 0.29; min = -0.35;
		for (int ii = 0; ii < vBDTlog[jj].size() ; ii++) {vBDTlog[jj][ii]=(vBDTlog[jj][ii]-(max+min)/2)*(2/(max-min));}

	max = *max_element(vBDTlog[jj].begin(), vBDTlog[jj].end());
	min = *min_element(vBDTlog[jj].begin(), vBDTlog[jj].end());
cout <<jj<<" Novie max  "<< max <<"  min  "<<min << endl;

		for (int ii = 0; ii < vBDTlog[jj].size() ; ii++) {vBDTlog[jj][ii]=-log(1-vBDTlog[jj][ii]+0.0001);}

	max = *max_element(vBDTlog[jj].begin(), vBDTlog[jj].end());
	min = *min_element(vBDTlog[jj].begin(), vBDTlog[jj].end());
cout <<jj<<" 2 Novie max  "<< max <<"  min  "<<min << endl;
max = -log(1-1+0.0001); min = -log(1+1+0.0001);
		for (int ii = 0; ii < vBDTlog[jj].size() ; ii++) {vBDTlog[jj][ii]=(vBDTlog[jj][ii]-(max+min)/2)*(2/(max-min));}

	max = *max_element(vBDTlog[jj].begin(), vBDTlog[jj].end());
	min = *min_element(vBDTlog[jj].begin(), vBDTlog[jj].end());
cout <<jj<<" 3 Novie max  "<< max <<"  min  "<<min << endl;

		for (int ii = 0; ii < vBDTlog[jj].size() ; ii++) {hBDTchannelLog[jj]->Fill(vBDTlog[jj][ii],vWeight[jj][ii]);}

	}
	}
*/

			cout<<"  end from TH1D "<<endl;

		CutFlowUnW->Write();
		histW->Write();
	if (isTT) histTop->Write();
	if (isTT && systematic == "TopPtUp"  ) histTopSq->Write();
		file->Write();
		file->Close();
		cout<<" Failed events  "<<counter<<endl;
	}
/////////////////////////////////////////////

