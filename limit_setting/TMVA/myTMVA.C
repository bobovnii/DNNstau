// @(#)root/tmva $Id$
/**********************************************************************************
 * Project   : TMVA - a ROOT-integrated toolkit for multivariate data analysis    *
 * Package   : TMVA                                                               *
 * Root Macro: TMVAClassification                                                 *
 *                                                                                *
 * This macro provides examples for the training and testing of the               *
 * TMVA classifiers.                                                              *
 *                                                                                *
 * As input data is used a toy-MC sample consisting of four Gaussian-distributed  *
 * and linearly correlated input variables.                                       *
 *                                                                                *
 * The methods to be used can be switched on and off by means of booleans, or     *
 * via the prompt command, for example:                                           *
 *                                                                                *
 *    root -l ./TMVAClassification.C\(\"Fisher,Likelihood\"\)                     *
 *                                                                                *
 * (note that the backslashes are mandatory)                                      *
 * If no method given, a default set of classifiers is used.                      *
 *                                                                                *
 * The output file "TMVA.root" can be analysed with the use of dedicated          *
 * macros (simply say: root -l <macro.C>), which can be conveniently              *
 * invoked through a GUI that will appear at the end of the run of this macro.    *
 * Launch the GUI via the command:                                                *
 *                                                                                *
 *    root -l ./TMVAGui.C                                                         *
 *                                                                                *
 * You can also compile and run the example with the following commands           *
 *                                                                                *
 *    make                                                                        *
 *    ./TMVAClassification <Methods>                                              *
 *                                                                                *
 * where: <Methods> = "method1 method2"                                           *
 *        are the TMVA classifier names                                           *
 *                                                                                *
 * example:                                                                       *
 *    ./TMVAClassification Fisher LikelihoodPCA BDT                               *
 *                                                                                *
 * If no method given, a default set is of classifiers is used                    *
 **********************************************************************************/

/*
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
*/


#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <fstream>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"
#include "TMVA/DataLoader.h"

//#include "analyzer.h"
#include "/nfs/dust/cms/user/bobovnii/new/CMSSW_8_0_20/src/DesyTauAnalyses/NTupleMaker/interface/functionsSUSY.h"
#include "/nfs/dust/cms/user/bobovnii/new/CMSSW_8_0_20/src/DesyTauAnalyses/NTupleMaker/interface/lester_mt2_bisect.h"
//#include "/nfs/dust/cms/user/bobovnii/new/CMSSW_8_0_20/src/DesyTauAnalyses/NTupleMaker/test/plots.h"

using namespace std;



double MuonFakeRate(float pt, float eta){
float muonfake = 1.;

if (  fabs(eta) < 1.2 ) muonfake = 1.04;
if (  fabs(eta) > 1.2 && fabs(eta) < 1.7 ) muonfake = 1.54;
if (  fabs(eta) > 1.7 && fabs(eta) < 2.3 ) muonfake = 1.42;
	
return muonfake;
}


double ElectronFakeRate(float pt, float eta){
float elefake = 1.;

if (  fabs(eta) < 1.460 ) elefake = 1.80;
if (  fabs(eta) > 1.558 ) elefake = 1.30;
	
return elefake;
}






int myTMVA( TString myMethodList = "", 
		TString outfileName = "TMVA_150withoutNormGiniIndexPray.root", 
		string BDTname = "mutau",
		float Sxsec = 1,
		TString Signal = "Stau100",  
		TString meth = "!H:!V:NTrees=850:MinNodeSize=0.2%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=35",
		TString WorkDir = "/nfs/dust/cms/user/bobovnii/MVAstau/CMSSW_8_0_25/src/DesyTauAnalyses/NTupleMaker/test/",
		TString channel = "mutau", 
		float EntriesProc = 1,
		float EntriesProcTest = 0.3, /// from EntriesProc!!!!!
		bool OnlyPositiveWeights = false,
		string Sdatasets = "datasets2"

		

)
{

////////////// definition!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	 const double MuMass = 0.105658367;
  double tauMass = 1.776;
  const double electronMass = 0.51100e-3;
  const double muonMass = 0.10565837;
  const double pionMass = 0.1396;
  const double bTagCut  = 0.8000;


  // Fixed size dimensions of array or collections stored in the TTree if any.

  // Declaration of leaf types
  Int_t	   nbtag;
  Int_t	   njets;
  Float_t         met_ex;
  Float_t         met_ey;
  Float_t         met_ez;
  Float_t         met_pt;
  Float_t         met_phi;
  Float_t         met_ex_JetEnUp;
  Float_t         met_ey_JetEnUp;
  Float_t         met_ex_JetEnDown;
  Float_t         met_ey_JetEnDown;
  Float_t		met_ex_UnclusteredEnUp;
  Float_t		met_ey_UnclusteredEnUp;
  Float_t		met_ex_UnclusteredEnDown;
  Float_t		met_ey_UnclusteredEnDown;

  Float_t         met_ex_JetEnUp_recoil;
  Float_t         met_ey_JetEnUp_recoil;
  Float_t         met_ex_JetEnDown_recoil;
  Float_t         met_ey_JetEnDown_recoil;
  Float_t         met_ex_UnclusteredEnUp_recoil;
  Float_t         met_ey_UnclusteredEnUp_recoil;
  Float_t         met_ex_UnclusteredEnDown_recoil;
  Float_t         met_ey_UnclusteredEnDown_recoil;
  Float_t         met_ex_recoil;
  Float_t         met_ey_recoil;
  Float_t         genmet;
  Float_t         genmetphi;
  Float_t         met_scaleUp;
  Float_t         metphi_scaleUp;
  Float_t         met_scaleDown;
  Float_t         metphi_scaleDown;
  Float_t         met_resoUp;
  Float_t         met_resoDown;
  Float_t         metphi_resoUp;
  Float_t         metphi_resoDown;

  Float_t         gen_weight;
  Float_t         pu_weight;
  Float_t         LSF_weight;
  Float_t         LSF_weight_el;
  Float_t         LSF_weight_mu;
  Float_t         LSF_weight_2;
  Float_t         LSF_weight_1;
  Float_t         TFR_weight;
  Float_t         top_weight;
  Float_t         all_weight;
  Float_t         trig_weight;
  Float_t         trig_weight_1;
  Float_t         trig_weight_2;
  Float_t         zptmassweight;
  Float_t         xsecs;
  Int_t           muon_index;
  Int_t           muon_index_1;
  Int_t           muon_index_2;
  Int_t           electron_index;
  Int_t           taus_index;
  Int_t           primvert_count;
  Float_t         primvert_x;
  Float_t         primvert_y;
  Float_t         primvert_z;
  Int_t           mu_count;
  Float_t         mu_px[20];
  Float_t         mu_py[20];
  Float_t         mu_pz[20];
  Float_t         mu_pt[20];
  Float_t         mu_eta[20];
  Float_t         mu_phi[20];
  Float_t         mu_charge[20];
  Float_t         mu_miniISO[20];
  Float_t         mu_dxy[20];
  Float_t         mu_dz[20];
  Float_t         mu_dzerr[20];
  Float_t         mu_dxyerr[20];
  Float_t         mu_relIso[20];
		
  Float_t     mu_neutralHadIso[20]; 
  Float_t     mu_photonIso[20]; 
  Float_t     mu_chargedHadIso[20]; 
  Float_t     mu_puIso[20]; 
  Float_t     mu_neutralIso[20];
  Float_t     mu_absIsoMu[20]; 
  Float_t     mu_relIsoMu[20]; 

  Float_t     el_neutralHadIso[20]; 
  Float_t     el_photonIso[20]; 
  Float_t     el_chargedHadIso[20]; 
  Float_t     el_puIso[20]; 
  Float_t     el_neutralIso[20];
  Float_t     el_absIsoEl[20]; 
  Float_t     el_relIsoEl[20]; 
  Float_t     el_isMVA[20]; 
               
  Int_t           jet_count;
  Int_t           npv;
  Int_t           npu;
  Int_t           jets_cleaned[30];
  Float_t         jet_e[30];
  Float_t         jet_px[30];
  Float_t         jet_py[30];
  Float_t         jet_pz[30];
  Float_t         jet_pt[30];
  Float_t         jet_eta[30];
  Float_t         jet_phi[30];
  Float_t         jet_flavour[30];
  Float_t         jet_btag[30];
  Float_t         CFCounter_[30];
  Int_t           jet_isLoose[30];
  Int_t           el_count;
  Float_t         el_px[20];
  Float_t         el_py[20];
  Float_t         el_pz[20];
  Float_t         el_pt[20];
  Float_t         el_eta[20];
  Float_t         el_phi[20];
  Float_t         el_miniISO[20];
  Float_t         el_dxy[20];
  Float_t         el_dxyerr[20];
  Float_t         el_dz[20];
  Float_t         el_dzerr[20];
  Float_t         el_charge[20];
  Float_t         el_relIso[20];
  Int_t           ta_count;
  Float_t         ta_px[30];
  Float_t         ta_py[30];
  Float_t         ta_pz[30];
  Float_t         ta_mass[30];
  Float_t         ta_eta[30];
  Float_t         ta_phi[30];
  Float_t         ta_pt[30];
  Float_t         ta_dxy[30];
  Float_t         ta_dz[30];
  Float_t         ta_charge[30];
  Float_t         ta_relIso[30];
  Float_t         ta_IsoFlag;
  
  Float_t 	  ta_IsoFlagVTight[5];
  Float_t 	  ta_IsoFlagTight[5];
  Float_t 	  ta_IsoFlagLoose[5];
  Float_t 	  ta_IsoFlagVLoose[5];
  Float_t 	  ta_IsoFlagMedium[5];
  Float_t         event_sign;
  Float_t         met_flag;
  Float_t         event_secondLeptonVeto;
  Float_t         eleMVA;
  Float_t         event_thirdLeptonVeto;
  Float_t         event_leptonDrTrigger;
  //   string 	   datasetName;
  string * datasetName = new std::string(); 
  string          *regionName;

  Float_t 	genTauMatched;
  Float_t 	genLeptonMatched;
  Float_t 	genLeptonMatchedEl;
  Float_t 	genLeptonMatchedMu;
  Float_t 	genTauDecayedMuMatched;
  Float_t 	genTauDecayedElMatched;
  Float_t 	genLeptonPromptElMatched;
  Float_t 	genLeptonPromptMuMatched;
  Float_t         genLeptonMatchedPrompEl;
  Float_t         genLeptonMatchedPrompMu;
  Float_t         genElMatchedToTauDecay;
  Float_t         genMuMatchedToTauDecay;
  Float_t         genTauMatchedToTauDecay;
  Float_t	        genLeptonMatchedPromptEl;
  Float_t	        genLeptonMatchedPromptMu;
  Float_t	        genLeptonMatchedPromptTau;
  Float_t	   genElMatchedHadrDecay;
  Float_t	   genMuMatchedHadrDecay;
  Float_t	   genTauMatchedHadrDecay;
  Float_t	   genLeptonMatchedGluon;
  Float_t	   genLeptonMatchedHFQ;
  Float_t	   genLeptonMatchedLFQ;

  Float_t         matchedTauToPromptEl;
  Float_t         matchedTauToPromptMu;
  Float_t         matchedTauToPromptTau;
  Float_t         matchedTauToTauDecEl;
  Float_t         matchedTauToTauDecMu;
  Float_t         matchedTauToTauDecTau;

  Float_t	   matchedTauToElHadronDec;
  Float_t	   matchedTauToMuHadronDec;
  Float_t	   matchedTauToTauHadronDec;


  Float_t	   qcdweight;
  Float_t	   qcdweightup;
  Float_t	   qcdweightdown;
  Int_t 	   npartons;

  // List of branches
  TBranch        *b_met_ex;   //!
  TBranch        *b_met_ey;   //!
  TBranch        *b_met_ez;   //!
  TBranch        *b_met_pt;   //!
  TBranch        *b_met_phi;   //!
  TBranch        *b_met_ex_JetEnUp;
  TBranch        *b_met_ey_JetEnUp;
  TBranch        *b_met_ex_JetEnDown;
  TBranch        *b_met_ey_JetEnDown;
  TBranch	       *b_met_ex_UnclusteredEnUp;
  TBranch	       *b_met_ey_UnclusteredEnUp;
  TBranch	       *b_met_ex_UnclusteredEnDown;
  TBranch	       *b_met_ey_UnclusteredEnDown;
  TBranch        *b_met_ex_JetEnUp_recoil;
  TBranch        *b_met_ey_JetEnUp_recoil;
  TBranch        *b_met_ex_JetEnDown_recoil;
  TBranch        *b_met_ey_JetEnDown_recoil;
  TBranch	       *b_met_ex_UnclusteredEnUp_recoil;
  TBranch	       *b_met_ey_UnclusteredEnUp_recoil;
  TBranch	       *b_met_ex_UnclusteredEnDown_recoil;
  TBranch	       *b_met_ey_UnclusteredEnDown_recoil;
  TBranch        *b_met_ex_recoil;
  TBranch        *b_met_ey_recoil;
  TBranch        *b_genmet;
  TBranch        *b_genmetphi;
  TBranch        *b_met_scaleUp;
  TBranch        *b_metphi_scaleUp;
  TBranch        *b_met_scaleDown;
  TBranch        *b_metphi_scaleDown;
  TBranch        *b_met_resoUp;
  TBranch        *b_met_resoDown;
  TBranch        *b_metphi_resoUp;
  TBranch        *b_metphi_resoDown;

  TBranch        *b_gen_weight;   //!
  TBranch        *b_pu_weight;   //!
  TBranch        *b_LSF_weight;   //!
  TBranch        *b_LSF_weight_mu;   //!
  TBranch        *b_LSF_weight_el;   //!
  TBranch        *b_LSF_weight_1;   //!
  TBranch        *b_LSF_weight_2;   //!
  TBranch        *b_TFR_weight;   //!
  TBranch        *b_top_weight;   //!
  TBranch        *b_all_weight;   //!
  TBranch        *b_trig_weight;   //!
  TBranch        *b_trig_weight_1;   //!
  TBranch        *b_trig_weight_2;   //!
  TBranch        *b_zptmassweight;   //!
  TBranch        *b_xsecs;   //!
  TBranch        *b_muon_index;   //!
  TBranch        *b_muon_index_1;   //!
  TBranch        *b_muon_index_2;   //!
  TBranch        *b_electron_index;   //!
  TBranch        *b_taus_index;   //!
  TBranch        *b_primvert_count;   //!
  TBranch        *b_primvert_x;   //!
  TBranch        *b_primvert_y;   //!
  TBranch        *b_primvert_z;   //!
  TBranch        *b_mu_count;   //!
  TBranch        *b_mu_px;   //!
  TBranch        *b_mu_py;   //!
  TBranch        *b_mu_pz;   //!
  TBranch        *b_mu_pt;   //!
  TBranch        *b_mu_eta;   //!
  TBranch        *b_mu_phi;   //!
  TBranch        *b_mu_charge;   //!
  TBranch        *b_mu_miniISO;   //!
  TBranch        *b_mu_dxy;   //!
  TBranch        *b_mu_dz;   //!
  TBranch        *b_mu_dxyerr;   //!
  TBranch        *b_mu_dzerr;   //!
  TBranch     *b_mu_neutralHadIso; 
  TBranch     *b_mu_photonIso; 
  TBranch     *b_mu_chargedHadIso; 
  TBranch     *b_mu_puIso; 
  TBranch     *b_mu_neutralIso;
  TBranch     *b_mu_absIsoMu; 
  TBranch     *b_mu_relIsoMu; 

  TBranch     *b_el_neutralHadIso; 
  TBranch     *b_el_photonIso; 
  TBranch     *b_el_chargedHadIso; 
  TBranch     *b_el_puIso; 
  TBranch     *b_el_neutralIso;
  TBranch     *b_el_absIsoEl; 
  TBranch     *b_el_relIsoEl; 
  TBranch     *b_el_isMVA; 
  TBranch        *b_mu_relIso;   //!
  TBranch        *b_jet_count;   //!
  TBranch        *b_npv;   //!
  TBranch        *b_npu;   //!
  TBranch        *b_jets_cleaned;   //!
  TBranch        *b_jet_e;   //!
  TBranch        *b_jet_px;   //!
  TBranch        *b_jet_py;   //!
  TBranch        *b_jet_pz;   //!
  TBranch        *b_jet_pt;   //!
  TBranch        *b_jet_eta;   //!
  TBranch        *b_jet_phi;   //!
  TBranch        *b_jet_flavour;   //!
  TBranch        *b_jet_btag;   //!
  TBranch        *b_jet_isLoose;   //!
  TBranch        *b_el_count;   //!
  TBranch        *b_el_px;   //!
  TBranch        *b_el_py;   //!
  TBranch        *b_el_pz;   //!
  TBranch        *b_el_pt;   //!
  TBranch        *b_el_eta;   //!
  TBranch        *b_el_phi;   //!
  TBranch        *b_el_miniISO;   //!
  TBranch        *b_el_dxy;   //!
  TBranch        *b_el_dz;   //!
  TBranch        *b_el_dxyerr;   //!
  TBranch        *b_el_dzerr;   //!
  TBranch        *b_el_charge;   //!
  TBranch        *b_el_relIso;   //!
  TBranch        *b_ta_count;   //!
  TBranch        *b_ta_px;   //!
  TBranch        *b_ta_py;   //!
  TBranch        *b_ta_pz;   //!
  TBranch        *b_ta_mass;   //!
  TBranch        *b_ta_eta;   //!
  TBranch        *b_ta_phi;   //!
  TBranch        *b_ta_pt;   //!
  TBranch        *b_ta_dxy;   //!
  TBranch        *b_ta_dz;   //!
  TBranch        *b_ta_charge;   //!
  TBranch        *b_ta_IsoFlag;   //!
  TBranch        *b_ta_Iso;   //!
  TBranch        *b_ta_relIso;   //!
  TBranch        *b_datasetName;   //!
  TBranch        *b_CFCounter_;   //!
  TBranch        *b_regionName;   //!
  TBranch        *b_event_sign;   //!
  
    TBranch	 *b_ta_IsoFlagVTight;
  TBranch	 *b_ta_IsoFlagTight;
  TBranch	 *b_ta_IsoFlagVLoose;
  TBranch	 *b_ta_IsoFlagLoose;
  TBranch	 *b_ta_IsoFlagMedium;
  TBranch        *b_met_flag;   //!
  TBranch        *b_event_secondLeptonVeto;   //!
  TBranch        *b_eleMVA;   //!
  TBranch        *b_event_thirdLeptonVeto;   //!
  TBranch        *b_event_leptonDrTrigger;   //!
  TBranch        *b_genTauMatched;   //!
  TBranch        *b_genLeptonMatched;   //!
  TBranch        *b_genLeptonMatchedEl;   //!
  TBranch        *b_genLeptonMatchedMu;   //!
  TBranch        *b_genTauDecayedMuMatched;   //!
  TBranch        *b_genTauDecayedElMatched;   //!
  TBranch        *b_genLeptonPromptElMatched;   //!
  TBranch        *b_genLeptonPromptMuMatched;   //!
  TBranch        *b_genLeptonMatchedPrompEl;   //!
  TBranch        *b_genLeptonMatchedPrompMu;   //!
  TBranch        *b_genTauMatchedToTauDecay;   //!
  TBranch        *b_matchedTauToPromptEl;   //!
  TBranch        *b_matchedTauToPromptMu;   //!
  TBranch        *b_matchedTauToPromptTau;   //!
  TBranch        *b_matchedTauToTauDecEl;   //!
  TBranch        *b_matchedTauToTauDecMu;   //!
  TBranch        *b_matchedTauToTauDecTau;   //!
  TBranch        *b_matchedTauToElHadronDec;   //!
  TBranch        *b_matchedTauToMuHadronDec;   //!
  TBranch        *b_matchedTauToTauHadronDec;   //!
  TBranch        *b_genLeptonMatchedPromptEl;   //!
  TBranch        *b_genLeptonMatchedPromptMu;   //!
  TBranch        *b_genLeptonMatchedPromptTau;   //!
  TBranch        *b_genElMatchedHadrDecay;   //!
  TBranch        *b_genMuMatchedHadrDecay;   //!
  TBranch        *b_genTauMatchedHadrDecay;   //!
  TBranch        *b_genLeptonMatchedGluon;   //!
  TBranch        *b_genLeptonMatchedLFQ;   //!
  TBranch        *b_genLeptonMatchedHFQ;   //!
  TBranch        *b_genElMatchedToTauDecay;   //!
  TBranch        *b_genMuMatchedToTauDecay;   //!





  TBranch        *b_qcdweight;   //!
  TBranch        *b_qcdweightup;   //!
  TBranch        *b_qcdweightdown;   //!
  TBranch        *b_npartons;   //!
  TBranch        *b_nbtag;   //!
  TBranch        *b_njets;   //!
		
///////////////////////////////////////// END of definition!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1111111






   //---------------------------------------------------------------
   // This loads the library
   TMVA::Tools::Instance();
   // Default MVA methods to be trained + tested
   std::map<std::string,int> Use;
   // --- Cut optimisation
   Use["Cuts"]            = 0;
   Use["CutsD"]           = 0;
   Use["CutsPCA"]         = 0;
   Use["CutsGA"]          = 0;
   Use["CutsSA"]          = 0;
   // 
   // --- 1-dimensional likelihood ("naive Bayes estimator")
   Use["Likelihood"]      = 0;
   Use["LikelihoodD"]     = 0; // the "D" extension indicates decorrelated input variables (see option strings)
   Use["LikelihoodPCA"]   = 0; // the "PCA" extension indicates PCA-transformed input variables (see option strings)
   Use["LikelihoodKDE"]   = 0;
   Use["LikelihoodMIX"]   = 0;
   //
   // --- Mutidimensional likelihood and Nearest-Neighbour methods
   Use["PDERS"]           = 0;
   Use["PDERSD"]          = 0;
   Use["PDERSPCA"]        = 0;
   Use["PDEFoam"]         = 0;
   Use["PDEFoamBoost"]    = 0; // uses generalised MVA method boosting
   Use["KNN"]             = 0; // k-nearest neighbour method
   Use["KNNup"]             = 0; // k-nearest neighbour method
   Use["KNNdown"]             = 0; // k-nearest neighbour method
   //
   // --- Linear Discriminant Analysis
   Use["LD"]              = 0; // Linear Discriminant identical to Fisher
   Use["Fisher"]          = 0;
   Use["FisherG"]         = 0;
   Use["BoostedFisher"]   = 0; // uses generalised MVA method boosting
   Use["HMatrix"]         = 0;
   //
   // --- Function Discriminant analysis
   Use["FDA_GA"]          = 0; // minimisation of user-defined function using Genetics Algorithm
   Use["FDA_SA"]          = 0;
   Use["FDA_MC"]          = 0;
   Use["FDA_MT"]          = 0;
   Use["FDA_GAMT"]        = 0;
   Use["FDA_MCMT"]        = 0;
   //
   // --- Neural Networks (all are feed-forward Multilayer Perceptrons)
   Use["MLP"]             = 0; // Recommended ANN
   Use["MLP1"]             = 0; // Recommended ANN
   Use["MLP2"]             = 0; // Recommended ANN
   Use["MLP3"]             = 0; // Recommended ANN
   Use["MLP4"]             = 0; // Recommended ANN
   Use["MLP5"]             = 0; // Recommended ANN
   Use["MLP6"]             = 0; // Recommended ANN
   Use["MLPGA"]             = 0; // Recommended ANN
   Use["MLPGA1"]             = 0; // Recommended ANN
   Use["MLPGA2"]             = 0; // Recommended ANN

   Use["MLPBFGS"]         = 0; // Recommended ANN with optional training method
   Use["MLPBFGS1"]         = 0; // Recommended ANN with optional training method
   Use["MLPBFGS2"]         = 0; // Recommended ANN with optional training method
   Use["MLPBFGS3"]         = 0; // Recommended ANN with optional training method

   Use["MLPBNN"]          = 0; // Recommended ANN with BFGS training method and bayesian regulator
   Use["MLPBNN1"]          = 0; // Recommended ANN with BFGS training method and bayesian regulator
   Use["MLPBNN2"]          = 0; // Recommended ANN with BFGS training method and bayesian regulator
   Use["MLPBNN3"]          = 0; // Recommended ANN with BFGS training method and bayesian regulator
   Use["MLPBNN4"]          = 0; // Recommended ANN with BFGS training method and bayesian regulator
   Use["MLPBNN5"]          = 0; // Recommended ANN with BFGS training method and bayesian regulator
   Use["MLPBNN6"]          = 0; // Recommended ANN with BFGS training method and bayesian regulator
   Use["MLPBNN7"]          = 0; // Recommended ANN with BFGS training method and bayesian regulator
   Use["MLPBNNmoreHiddenLayers"]          = 0; // Recommended ANN with BFGS training method and bayesian regulator


   Use["CFMlpANN"]        = 0; // Depreciated ANN from ALEPH
   Use["TMlpANN"]         = 0; // ROOT's own ANN
   //
   // --- Support Vector Machine 
   Use["SVM"]             = 0;

   Use["SVM1"]             = 0;
   Use["SVM2"]             = 0;
   Use["SVM3"]             = 0;
   Use["SVM4"]             = 0;

   // 
   // --- Boosted Decision Trees
   Use["BDT"]             = 0; // uses Adaptive Boost
   Use["BDT"+BDTname]             = 1; // uses Adaptive Boost
//   Use["BDTmutau"]             = 0; // uses Adaptive Boost
//   Use["BDTeltau"]             = 0; // uses Adaptive Boost
//   Use["BDTmuel"]             = 0; // uses Adaptive Boost
   Use["BDT1"]             = 0; // uses Adaptive Boost
   Use["BDT2"]             = 0; // uses Adaptive Boost
   Use["BDT3"]             = 0; // uses Adaptive Boost
   Use["BDT4"]             = 0; // uses Adaptive Boost
   Use["BDT5"]             = 0; // uses Adaptive Boost
   Use["BDT6"]             = 0; // uses Adaptive Boost
   Use["BDT7"]             = 0; // uses Adaptive Boost
   Use["BDT8"]             = 0; // uses Adaptive Boost

   Use["BDTG"]            = 0; // uses Gradient Boost
   Use["BDTB"]            = 0; // uses Bagging
   Use["BDTD"]            = 0; // decorrelation + Adaptive Boost
   Use["BDTF"]            = 0; // allow usage of fisher discriminant for node splitting 
   // 
   // --- Friedman's RuleFit method, ie, an optimised series of cuts ("rules")
   Use["RuleFit"]         = 0;
   // ---------------------------------------------------------------

   std::cout << std::endl;
   std::cout << "==> Start TMVAClassification" << std::endl;

   // Select methods (don't look at this code - not of interest)
   if (myMethodList != "") {
      for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;

      std::vector<TString> mlist = TMVA::gTools().SplitString( myMethodList, ',' );
      for (UInt_t i=0; i<mlist.size(); i++) {
         std::string regMethod(mlist[i]);

         if (Use.find(regMethod) == Use.end()) {
            std::cout << "Method \"" << regMethod << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
            for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) std::cout << it->first << " ";
            std::cout << std::endl;
            return 1;
         }
         Use[regMethod] = 1;
      }
   }

   // --------------------------------------------------------------------------------------------------

   // --- Here the preparation phase begins

   // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
   //TString outfileName( "TMVA_NEwStauSampleNewVariablesTestMoreCuts.root" );
   TFile* outputFile = TFile::Open( outfileName, "RECREATE" );

   TMVA::Factory *factory = new TMVA::Factory( "myTMVA", outputFile,
                                               "!V:!Silent:Color:DrawProgressBar:Transformations=I");

   TMVA::DataLoader *dataloader=new TMVA::DataLoader("dataset");

   dataloader->AddVariable( "met_pt","#it{p}_{T}^{miss} [GeV]", "GeV",'F' );
   dataloader->AddVariable( "Lept1Pt", "P_{T}(1st lepton)", "GeV", 'F' );
   dataloader->AddVariable( "Lept2Pt", "P_{T}(2st lepton)", "GeV", 'F' );
   dataloader->AddVariable( "EtaDil", "#Delta#eta(lep1,lep2)", "units", 'F' );
   dataloader->AddVariable( "MTtot", "M_{Ttot}", "GeV", 'F' );
   dataloader->AddVariable( "Dzeta", "D#zeta", "units", 'F' );
   dataloader->AddVariable( "Minv", "M_{inv}", "GeV", 'F' );
   dataloader->AddVariable( "dR", "#Delta R(dilepton)", "units", 'F' );
   dataloader->AddVariable( "MT", "M_T", "GeV", 'F' );
   dataloader->AddVariable( "MCTb", "M_{CT}", "GeV", 'F' );
   dataloader->AddVariable( "MT2lester", "M_{T2}", "GeV", 'F' );
   dataloader->AddVariable( "dMETPhiL1", "#Delta#Phi(#it{p}_{T}^{miss},lep1)", "rad", 'F' );
   dataloader->AddVariable( "dMETPhiL2", "#Delta#Phi(#it{p}_{T}^{miss},lep2)", "rad", 'F' );


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// FILLING



	ifstream datasets;
	datasets.open(Sdatasets);



	cout.setf(ios::fixed, ios::floatfield);
	cout.setf(ios::showpoint);
	string name;
	TString name2;
	TString name3;
	TString treeName;
	TString nameinputEventsH;
	int CutNumb;	
	TH1D *hist[50];
	int i = 0;


	string SE ="SingleElectron";
	string SM ="SingleMuon";
	string MuonEG ="MuonEG";
	string QCDs ="QCD";
	string Tau ="Tau";
	string stau ="LSP";
	string TT ="TT_Tune";
	string WJ ="JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM";
	string DY ="DY";
	string W0J ="WJetsToLNu_TuneCUETP8M1";
	string DY0J ="DYJetsToLL_M-50";	
	float xsec;
	float SumOfWeights;	
	float Lumi = 35800;
	float QCDscale = 2;
	float MCweight = 1;


	TLorentzVector  ElV;
	TLorentzVector  MuV;
	TLorentzVector  TauV;
	TLorentzVector  JetsV;
	TLorentzVector  LeptV1;
	TLorentzVector  LeptV2;
	TLorentzVector  METV;
	vector<TLorentzVector> JetsMV;


	int NselectedEv;
	int NnegEvents;

	string line;
   	 while(getline(datasets, line))
		{
		istringstream iss(line); // access line as a stream
		float xs,fact,fact2;
		xs=1;fact=1;fact2=1;
		iss >> name >> xs >> fact >> fact2;



		name2 = WorkDir+channel+"/"+name+"_B_OS.root";
		string HHname = name;	
		//if (std::string::npos != HHname.find(stau)) name2 = WorkDir+channel+"/"+name+".root";
		if (std::string::npos != HHname.find(stau) ) {name2 = WorkDir+channel+"/"+Signal+name+".root";xs=Sxsec*fact;}
       		cout << i << endl;
       		cout << name << endl; 
		name3 = name;
		//treeName = "tree"+name;
    		cout << name2 << endl;
		TFile * name3 = new TFile(name2);
		name3->cd(channel);

		TH1D * histWeightsH = (TH1D*)name3->Get(channel+"/histWeightsH");
		TTree *treeName = (TTree*)name3->Get(channel+"/T");


		xsec = xs*fact*fact2;
		SumOfWeights = histWeightsH->GetSumOfWeights();
		cout << "SumOfWeights = " << SumOfWeights << endl;
		cout << "xsec " << xsec<< endl;
		cout << "xsec*Lumi = " << xsec*Lumi << endl;
		cout << "norm  "<< xsec*Lumi/SumOfWeights << endl;
		MCweight = xsec*Lumi/SumOfWeights;
		if (std::string::npos != HHname.find(SM) || std::string::npos != HHname.find(SE)) MCweight =1;


  treeName->SetBranchAddress("met_ey", &met_ey, &b_met_ey);
  treeName->SetBranchAddress("met_ez", &met_ez, &b_met_ez);
  treeName->SetBranchAddress("met_pt", &met_pt, &b_met_pt);
  treeName->SetBranchAddress("met_phi", &met_phi, &b_met_phi);
  treeName->SetBranchAddress("met_ex_recoil", &met_ex_recoil, &b_met_ex_recoil);
  treeName->SetBranchAddress("met_ey_recoil", &met_ey_recoil, &b_met_ey_recoil);
  treeName->SetBranchAddress("genmet", &genmet, &b_genmet);
  treeName->SetBranchAddress("genmetphi", &genmetphi, &b_genmetphi);
  treeName->SetBranchAddress("met_scaleUp", &met_scaleUp, &b_met_scaleUp);
  treeName->SetBranchAddress("met_scaleDown", &met_scaleDown, &b_met_scaleDown);
  treeName->SetBranchAddress("metphi_scaleUp", &metphi_scaleUp, &b_metphi_scaleUp);
  treeName->SetBranchAddress("metphi_scaleDown", &metphi_scaleDown, &b_metphi_scaleDown);
  treeName->SetBranchAddress("met_resoUp", &met_resoUp, &b_met_resoUp);
  treeName->SetBranchAddress("met_resoDown", &met_resoDown, &b_met_resoDown);
  treeName->SetBranchAddress("metphi_resoUp", &metphi_resoUp, &b_metphi_resoUp);
  treeName->SetBranchAddress("metphi_resoDown", &metphi_resoDown, &b_metphi_resoDown);


  treeName->SetBranchAddress("met_ex_JetEnUp_recoil", &met_ex_JetEnUp_recoil, &b_met_ex_JetEnUp_recoil);
  treeName->SetBranchAddress("met_ey_JetEnUp_recoil", &met_ey_JetEnUp_recoil, &b_met_ey_JetEnUp_recoil);

  treeName->SetBranchAddress("met_ex_JetEnDown_recoil", &met_ex_JetEnDown_recoil, &b_met_ex_JetEnDown_recoil);
  treeName->SetBranchAddress("met_ey_JetEnDown_recoil", &met_ey_JetEnDown_recoil, &b_met_ey_JetEnDown_recoil);

  treeName->SetBranchAddress("met_ex_UnclusteredEnDown_recoil", &met_ex_UnclusteredEnDown_recoil, &b_met_ex_UnclusteredEnDown_recoil);
  treeName->SetBranchAddress("met_ey_UnclusteredEnDown_recoil", &met_ey_UnclusteredEnDown_recoil, &b_met_ey_UnclusteredEnDown_recoil);

  treeName->SetBranchAddress("met_ex_UnclusteredEnUp_recoil", &met_ex_UnclusteredEnUp_recoil, &b_met_ex_UnclusteredEnUp_recoil);
  treeName->SetBranchAddress("met_ey_UnclusteredEnUp_recoil", &met_ey_UnclusteredEnUp_recoil, &b_met_ey_UnclusteredEnUp_recoil);

  treeName->SetBranchAddress("met_ex_JetEnUp", &met_ex_JetEnUp, &b_met_ex_JetEnUp);
  treeName->SetBranchAddress("met_ey_JetEnUp", &met_ey_JetEnUp, &b_met_ey_JetEnUp);

  treeName->SetBranchAddress("met_ex_JetEnDown", &met_ex_JetEnDown, &b_met_ex_JetEnDown);
  treeName->SetBranchAddress("met_ey_JetEnDown", &met_ey_JetEnDown, &b_met_ey_JetEnDown);

  treeName->SetBranchAddress("met_ex_UnclusteredEnDown", &met_ex_UnclusteredEnDown, &b_met_ex_UnclusteredEnDown);
  treeName->SetBranchAddress("met_ey_UnclusteredEnDown", &met_ey_UnclusteredEnDown, &b_met_ey_UnclusteredEnDown);

  treeName->SetBranchAddress("met_ex_UnclusteredEnUp", &met_ex_UnclusteredEnUp, &b_met_ex_UnclusteredEnUp);
  treeName->SetBranchAddress("met_ey_UnclusteredEnUp", &met_ey_UnclusteredEnUp, &b_met_ey_UnclusteredEnUp);



  treeName->SetBranchAddress("gen_weight", &gen_weight, &b_gen_weight);
  treeName->SetBranchAddress("pu_weight", &pu_weight, &b_pu_weight);
  treeName->SetBranchAddress("LSF_weight", &LSF_weight, &b_LSF_weight);
  treeName->SetBranchAddress("LSF_weight_mu", &LSF_weight_mu, &b_LSF_weight_mu);
  treeName->SetBranchAddress("LSF_weight_el", &LSF_weight_el, &b_LSF_weight_el);
  treeName->SetBranchAddress("LSF_weight_1", &LSF_weight_1, &b_LSF_weight_1);
  treeName->SetBranchAddress("LSF_weight_2", &LSF_weight_2, &b_LSF_weight_2);
  treeName->SetBranchAddress("TFR_weight", &TFR_weight, &b_TFR_weight);
  treeName->SetBranchAddress("top_weight", &top_weight, &b_top_weight);
  treeName->SetBranchAddress("all_weight", &all_weight, &b_all_weight);
  treeName->SetBranchAddress("trig_weight", &trig_weight, &b_trig_weight);
  treeName->SetBranchAddress("trig_weight_1", &trig_weight_1, &b_trig_weight_1);
  treeName->SetBranchAddress("trig_weight_2", &trig_weight_2, &b_trig_weight_2);
  treeName->SetBranchAddress("zptmassweight", &zptmassweight, &b_zptmassweight);
  treeName->SetBranchAddress("xsecs", &xsecs, &b_xsecs);
  treeName->SetBranchAddress("muon_index", &muon_index, &b_muon_index);
  treeName->SetBranchAddress("muon_index_1", &muon_index_1, &b_muon_index_1);
  treeName->SetBranchAddress("muon_index_2", &muon_index_2, &b_muon_index_2);
  treeName->SetBranchAddress("electron_index", &electron_index, &b_electron_index);
  treeName->SetBranchAddress("taus_index", &taus_index, &b_taus_index);
  treeName->SetBranchAddress("primvert_count", &primvert_count, &b_primvert_count);
  treeName->SetBranchAddress("primvert_x", &primvert_x, &b_primvert_x);
  treeName->SetBranchAddress("primvert_y", &primvert_y, &b_primvert_y);
  treeName->SetBranchAddress("primvert_z", &primvert_z, &b_primvert_z);
  treeName->SetBranchAddress("mu_count", &mu_count, &b_mu_count);
  treeName->SetBranchAddress("mu_px", mu_px, &b_mu_px);
  treeName->SetBranchAddress("mu_py", mu_py, &b_mu_py);
  treeName->SetBranchAddress("mu_pz", mu_pz, &b_mu_pz);
  treeName->SetBranchAddress("mu_pt", mu_pt, &b_mu_pt);
  treeName->SetBranchAddress("mu_eta", mu_eta, &b_mu_eta);
  treeName->SetBranchAddress("mu_phi", mu_phi, &b_mu_phi);
  treeName->SetBranchAddress("mu_charge", mu_charge, &b_mu_charge);
  treeName->SetBranchAddress("mu_miniISO", mu_miniISO, &b_mu_miniISO);
  treeName->SetBranchAddress("mu_dxy", mu_dxy, &b_mu_dxy);
  treeName->SetBranchAddress("mu_dz", mu_dz, &b_mu_dz);
  treeName->SetBranchAddress("mu_dxyerr", mu_dxyerr, &b_mu_dxyerr);
  treeName->SetBranchAddress("mu_dzerr", mu_dzerr, &b_mu_dzerr);


  treeName->SetBranchAddress("mu_neutralHadIso", mu_neutralHadIso, &b_mu_neutralHadIso);
  treeName->SetBranchAddress("mu_photonIso", mu_photonIso, &b_mu_photonIso);
  treeName->SetBranchAddress("mu_chargedHadIso", mu_chargedHadIso, &b_mu_chargedHadIso);
  treeName->SetBranchAddress("mu_puIso", mu_puIso, &b_mu_puIso);
  treeName->SetBranchAddress("mu_neutralIso", mu_neutralIso, &b_mu_neutralIso);
  treeName->SetBranchAddress("mu_absIsoMu", mu_absIsoMu, &b_mu_absIsoMu);
  treeName->SetBranchAddress("mu_relIsoMu", mu_relIsoMu, &b_mu_relIsoMu);
  treeName->SetBranchAddress("el_neutralHadIso", el_neutralHadIso, &b_el_neutralHadIso);
  treeName->SetBranchAddress("el_photonIso", el_photonIso, &b_el_photonIso);
  treeName->SetBranchAddress("el_chargedHadIso", el_chargedHadIso, &b_el_chargedHadIso);
  treeName->SetBranchAddress("el_puIso", el_puIso, &b_el_puIso);
  treeName->SetBranchAddress("el_neutralIso", el_neutralIso, &b_el_neutralIso);
  treeName->SetBranchAddress("el_absIsoEl", el_absIsoEl, &b_el_absIsoEl);
  treeName->SetBranchAddress("el_relIsoEl", el_relIsoEl, &b_el_relIsoEl);
  treeName->SetBranchAddress("el_isMVA", el_isMVA, &b_el_isMVA);


  treeName->SetBranchAddress("mu_relIso", mu_relIso, &b_mu_relIso);
  treeName->SetBranchAddress("jet_count", &jet_count, &b_jet_count);
  treeName->SetBranchAddress("npv", &npv, &b_npv);
  treeName->SetBranchAddress("npu", &npu, &b_npu);
  treeName->SetBranchAddress("jets_cleaned", &jets_cleaned, &b_jets_cleaned);
  treeName->SetBranchAddress("njets", &njets, &b_njets);
  treeName->SetBranchAddress("jet_e", jet_e, &b_jet_e);
  treeName->SetBranchAddress("jet_px", jet_px, &b_jet_px);
  treeName->SetBranchAddress("jet_py", jet_py, &b_jet_py);
  treeName->SetBranchAddress("jet_pz", jet_pz, &b_jet_pz);
  treeName->SetBranchAddress("jet_pt", jet_pt, &b_jet_pt);
  treeName->SetBranchAddress("jet_eta", jet_eta, &b_jet_eta);
  treeName->SetBranchAddress("jet_phi", jet_phi, &b_jet_phi);
  treeName->SetBranchAddress("jet_flavour", jet_flavour, &b_jet_flavour);
  treeName->SetBranchAddress("jet_btag", jet_btag, &b_jet_btag);
  treeName->SetBranchAddress("jet_isLoose", jet_isLoose, &b_jet_isLoose);


  treeName->SetBranchAddress("el_count", &el_count, &b_el_count);
  treeName->SetBranchAddress("el_px", el_px, &b_el_px);
  treeName->SetBranchAddress("el_py", el_py, &b_el_py);
  treeName->SetBranchAddress("el_pz", el_pz, &b_el_pz);
  treeName->SetBranchAddress("el_pt", el_pt, &b_el_pt);
  treeName->SetBranchAddress("el_eta", el_eta, &b_el_eta);
  treeName->SetBranchAddress("el_phi", el_phi, &b_el_phi);
  treeName->SetBranchAddress("el_miniISO", el_miniISO, &b_el_miniISO);
  treeName->SetBranchAddress("el_dxy", el_dxy, &b_el_dxy);
  treeName->SetBranchAddress("el_dz", el_dz, &b_el_dz);
  treeName->SetBranchAddress("el_dxyerr", el_dxyerr, &b_el_dxyerr);
  treeName->SetBranchAddress("el_dzerr", el_dzerr, &b_el_dzerr);
  treeName->SetBranchAddress("el_charge", el_charge, &b_el_charge);
  treeName->SetBranchAddress("el_relIso", el_relIso, &b_el_relIso);


  treeName->SetBranchAddress("ta_count", &ta_count, &b_ta_count);
  treeName->SetBranchAddress("ta_px", ta_px, &b_ta_px);
  treeName->SetBranchAddress("ta_py", ta_py, &b_ta_py);
  treeName->SetBranchAddress("ta_pz", ta_pz, &b_ta_pz);
  treeName->SetBranchAddress("ta_mass", ta_mass, &b_ta_mass);
  treeName->SetBranchAddress("ta_eta", ta_eta, &b_ta_eta);
  treeName->SetBranchAddress("ta_phi", ta_phi, &b_ta_phi);
  treeName->SetBranchAddress("ta_pt", ta_pt, &b_ta_pt);
  treeName->SetBranchAddress("ta_dxy", ta_dxy, &b_ta_dxy);
  treeName->SetBranchAddress("ta_dz", ta_dz, &b_ta_dz);
  treeName->SetBranchAddress("ta_charge", ta_charge, &b_ta_charge);
  treeName->SetBranchAddress("ta_IsoFlag", &ta_IsoFlag, &b_ta_IsoFlag);
  treeName->SetBranchAddress("ta_relIso", ta_relIso, &b_ta_relIso);

  treeName->SetBranchAddress("ta_IsoFlagVTight", ta_IsoFlagVTight, &b_ta_IsoFlagVTight);
  treeName->SetBranchAddress("ta_IsoFlagTight", ta_IsoFlagTight, &b_ta_IsoFlagTight);
  treeName->SetBranchAddress("ta_IsoFlagVLoose", ta_IsoFlagVLoose, &b_ta_IsoFlagVLoose);
  treeName->SetBranchAddress("ta_IsoFlagLoose", ta_IsoFlagLoose, &b_ta_IsoFlagLoose);
  treeName->SetBranchAddress("ta_IsoFlagMedium", ta_IsoFlagMedium, &b_ta_IsoFlagMedium);


  treeName->SetBranchAddress("datasetName", &datasetName);

  treeName->SetBranchAddress("CFCounter_", CFCounter_,&b_CFCounter_);

  //treeName->SetBranchAddress("regionName", &regionName, &b_regionName);

  treeName->SetBranchAddress("event_sign", &event_sign, &b_event_sign);

  treeName->SetBranchAddress("met_flag", &met_flag, &b_met_flag);


  treeName->SetBranchAddress("eleMVA", &eleMVA, &b_eleMVA);


  treeName->SetBranchAddress("event_secondLeptonVeto", &event_secondLeptonVeto, &b_event_secondLeptonVeto);
  treeName->SetBranchAddress("event_thirdLeptonVeto", &event_thirdLeptonVeto, &b_event_thirdLeptonVeto);
  treeName->SetBranchAddress("event_leptonDrTrigger", &event_leptonDrTrigger, &b_event_leptonDrTrigger);


  treeName->SetBranchAddress("genTauMatched", &genTauMatched, &b_genTauMatched);
  treeName->SetBranchAddress("genLeptonMatched", &genLeptonMatched, &b_genLeptonMatched);
  treeName->SetBranchAddress("genLeptonMatchedEl", &genLeptonMatchedEl, &b_genLeptonMatchedEl);
  treeName->SetBranchAddress("genLeptonMatchedMu", &genLeptonMatchedMu, &b_genLeptonMatchedMu);
  treeName->SetBranchAddress("genTauDecayedMuMatched", &genTauDecayedMuMatched, &b_genTauDecayedMuMatched);
  treeName->SetBranchAddress("genTauDecayedElMatched", &genTauDecayedElMatched, &b_genTauDecayedElMatched);
  treeName->SetBranchAddress("genLeptonPromptElMatched", &genLeptonPromptElMatched, &b_genLeptonPromptElMatched);
  treeName->SetBranchAddress("genLeptonPromptMuMatched", &genLeptonPromptMuMatched, &b_genLeptonPromptMuMatched);


  treeName->SetBranchAddress("genLeptonMatchedPromptEl", &genLeptonMatchedPromptMu, &b_genLeptonMatchedPromptEl);
  treeName->SetBranchAddress("genLeptonMatchedPromptMu", &genLeptonMatchedPromptMu, &b_genLeptonMatchedPromptMu);
  //			treeName->SetBranchAddress("genLeptonMatchedPromptTau", &genLeptonMatchedPromptMu, &b_genLeptonMatchedPromptTau);



  treeName->SetBranchAddress("genLeptonMatchedPrompEl", &genLeptonMatchedPrompEl, &b_genLeptonMatchedPrompEl);
  treeName->SetBranchAddress("genLeptonMatchedPrompMu", &genLeptonMatchedPrompMu, &b_genLeptonMatchedPrompMu);
  treeName->SetBranchAddress("genElMatchedToTauDecay", &genElMatchedToTauDecay, &b_genElMatchedToTauDecay);
  treeName->SetBranchAddress("genMuMatchedToTauDecay", &genMuMatchedToTauDecay, &b_genMuMatchedToTauDecay);
  treeName->SetBranchAddress("genTauMatchedToTauDecay", &genTauMatchedToTauDecay, &b_genTauMatchedToTauDecay);
  treeName->SetBranchAddress("matchedTauToPromptEl", &matchedTauToPromptEl, &b_matchedTauToPromptEl);
  treeName->SetBranchAddress("matchedTauToPromptMu", &matchedTauToPromptMu, &b_matchedTauToPromptMu);
  treeName->SetBranchAddress("matchedTauToPromptTau", &matchedTauToPromptTau, &b_matchedTauToPromptTau);
  treeName->SetBranchAddress("matchedTauToTauDecEl", &matchedTauToTauDecEl, &b_matchedTauToTauDecEl);
  treeName->SetBranchAddress("matchedTauToTauDecMu", &matchedTauToTauDecMu, &b_matchedTauToTauDecMu);
  treeName->SetBranchAddress("matchedTauToTauDecTau", &matchedTauToTauDecTau, &b_matchedTauToTauDecTau);
  treeName->SetBranchAddress("matchedTauToElHadronDec", &matchedTauToElHadronDec, &b_matchedTauToElHadronDec);
  treeName->SetBranchAddress("matchedTauToMuHadronDec", &matchedTauToMuHadronDec, &b_matchedTauToMuHadronDec);
  treeName->SetBranchAddress("matchedTauToTauHadronDec", &matchedTauToTauHadronDec, &b_matchedTauToTauHadronDec);
  treeName->SetBranchAddress("genLeptonMatchedGluon", &genLeptonMatchedGluon, &b_genLeptonMatchedGluon);
  treeName->SetBranchAddress("genLeptonMatchedLFQ", &genLeptonMatchedLFQ, &b_genLeptonMatchedLFQ);
  treeName->SetBranchAddress("genLeptonMatchedHFQ", &genLeptonMatchedHFQ, &b_genLeptonMatchedHFQ);


  treeName->SetBranchAddress("qcdweight", &qcdweight, &b_qcdweight);
  treeName->SetBranchAddress("qcdweightup", &qcdweightup, &b_qcdweightup);
  treeName->SetBranchAddress("qcdweightdown", &qcdweightdown, &b_qcdweightdown);
  treeName->SetBranchAddress("nbtag", &nbtag, &b_nbtag);

  treeName->SetBranchAddress("npartons",&npartons,&b_npartons);




		//////////////////////////////////////////////////////////////////////////////////////End of BAD waY!!!!!!!!!!!!!!!!!!!!!!!1
		NselectedEv =0;
		NnegEvents =0;

		if (std::string::npos != HHname.find(stau)) EntriesProc=1.0;
		for (UInt_t i=0; i<(EntriesProc*treeName->GetEntries()); i++) {


           	//Define 4-LorenzVetor:
           	treeName->GetEntry(i);
			float all_weights = 1.;
			JetsV.SetPxPyPzE(0.,0.,0.,0.);
			MuV.SetPxPyPzE(0.,0.,0.,0.);
			ElV.SetPxPyPzE(0.,0.,0.,0.);
			TauV.SetPxPyPzE(0.,0.,0.,0.);
			double dEta;
			JetsMV.clear();

			if(muon_index>-1 && muon_index<8)			MuV.SetPtEtaPhiM(mu_pt[muon_index], mu_eta[muon_index], mu_phi[muon_index], muonMass);
			if(electron_index>-1 && electron_index<8)		ElV.SetPtEtaPhiM(el_pt[electron_index], el_eta[electron_index], el_phi[electron_index],electronMass);
			if (taus_index>-1 && taus_index<8)			TauV.SetPtEtaPhiM(ta_pt[taus_index], ta_eta[taus_index], ta_phi[taus_index], tauMass);
			for (int nj=0;nj<njets;++nj) {JetsV.SetPxPyPzE(jet_px[jets_cleaned[nj]], jet_py[jets_cleaned[nj]],jet_pz[jets_cleaned[nj]],jet_e[jets_cleaned[nj]]);
			JetsMV.push_back(JetsV);}

			METV.SetPx(met_ex);
			METV.SetPy(met_ey);
			float met = sqrt(met_ex*met_ex + met_ey*met_ey); 
			if (std::string::npos != HHname.find(WJ) || std::string::npos != HHname.find(DY)) 
			{
				METV.SetPx(0.);
				METV.SetPy(0.);
				METV.SetPx(met_ex_recoil);
				METV.SetPy(met_ey_recoil);
				met=sqrt(met_ex_recoil*met_ex_recoil+met_ey_recoil*met_ey_recoil);
			}

			if (channel == "mutau") {LeptV1 = MuV; LeptV2 = TauV;}
			if (channel == "eltau") {LeptV1 = ElV; LeptV2 = TauV;}
			if (channel == "muel") {LeptV1 = MuV; LeptV2 = ElV;}
//cout <<"V1 and V2  !!!   "<< LeptV1.Pt() <<"  "<< 	LeptV2.Pt() <<"  "<< TauV.Pt()<<endl;		

			if (std::string::npos != HHname.find(W0J) && (npartons>0 && npartons<5)) continue;
	
			if (std::string::npos != HHname.find(DY0J) && (npartons>0 && npartons<5)) continue;







			if (abs(mu_count) > 8 || abs(el_count) > 8 || abs(ta_count) >8) continue;

			all_weights = pu_weight * gen_weight;
			
			
			
			
			// Tau origin finding
		bool isData = false;
		isData = (std::string::npos != HHname.find(SM) || std::string::npos != HHname.find(SE));
			if (isData) all_weights = 1;

		bool isRealTau  = (genTauMatched>0.5 && !isData) ;
		bool isMuPromptLepton =  (genLeptonMatchedPromptMu >0.5 && genLeptonMatchedPromptEl<0.5 && !isData);
		bool isElPromptLepton =  (genLeptonMatchedPromptEl >0.5 && genLeptonMatchedPromptMu<0.5 && !isData);

		bool isMuFromTau =  (genMuMatchedToTauDecay > 0.5 && !isData);


		bool isfakeTauFromPromptLepton = matchedTauToPromptEl>0.5 || matchedTauToPromptMu >0.5 ;
	
		bool isfakeTauFromJet = genTauMatched<0.5 && !isfakeTauFromPromptLepton ;
		bool isfakeTauFromHadron = matchedTauToMuHadronDec > 0.5 || matchedTauToElHadronDec > 0.5;
		
		if (isfakeTauFromHadron) isfakeTauFromJet = true;

		bool isfakeTauFromMu =  ( genTauMatched < 0.5 && matchedTauToPromptMu > 0.5 && !isfakeTauFromJet && !isData) ;
		bool isfakeTauFromEl =  ( genTauMatched < 0.5 && matchedTauToPromptEl > 0.5 && !isfakeTauFromJet && !isData) ;
	

		bool isfakeMuFromGluon =  ( genLeptonMatchedGluon >0.5 && !isData) ;
		bool isfakeMuFromHFQ =  (  genLeptonMatchedHFQ >0.5 && !isData) ;
		bool isfakeMuFromLFQ =  (  genLeptonMatchedLFQ >0.5 && !isData) ;			
			
			
			if (std::string::npos != HHname.find(stau)  && ta_IsoFlag < 0.5 ) continue;

			
			if (std::string::npos == HHname.find(stau) )
			{
			// select data
			if ((std::string::npos != HHname.find(SM) || std::string::npos != HHname.find(SE)) && !(ta_IsoFlagTight[0] < 0.5 && ta_IsoFlagLoose[0] > 0.5) ) continue;
			if ((std::string::npos != HHname.find(SM) || std::string::npos != HHname.find(SE)) && (ta_IsoFlagTight[0] < 0.5 && ta_IsoFlagLoose[0] > 0.5) ) 
				{
					/*
					/// transfer factor 
				if (fabs(LeptV2.Eta())<0.8)
					{
					if (LeptV2.Pt()<30) all_weights *= 0.74;
					if (LeptV2.Pt()>30 && LeptV2.Pt()<40) all_weights *= 0.66;
					if (LeptV2.Pt()>40) all_weights *= 0.56;
					}
				if (fabs(LeptV2.Eta())>0.8 && fabs(LeptV2.Eta())<1.44)
					{
					if (LeptV2.Pt()<30) all_weights *= 0.68;
					if (LeptV2.Pt()>30 && LeptV2.Pt()<40) all_weights *= 0.61;
					if (LeptV2.Pt()>40) all_weights *= 0.39;
					}

				if (fabs(LeptV2.Eta())>1.44 && fabs(LeptV2.Eta())<1.566)
					{
					if (LeptV2.Pt()<40) all_weights *= 0.68;
					if (LeptV2.Pt()>40) all_weights *= 0.64;
					}

				if (fabs(LeptV2.Eta())>1.566 && fabs(LeptV2.Eta())<2.3)
					{
					if (LeptV2.Pt()<40) all_weights *= 0.59;
					if (LeptV2.Pt()>40) all_weights *= 0.61;
					}

				}
			
			*/
			
			 if (  fabs(LeptV2.Eta()) < 0.8 )
 {
        if (LeptV2.Pt()>20 && LeptV2.Pt()<30) all_weights *= 0.673922;
         if (LeptV2.Pt()>30 && LeptV2.Pt()<40) all_weights *= 0.643638;
         if (LeptV2.Pt()>40 ) all_weights *= 0.580981;
 }
 if (  fabs(LeptV2.Eta()) > 0.8 && fabs(LeptV2.Eta()) < 1.44 )
 {
        if (LeptV2.Pt()>20 && LeptV2.Pt()<30) all_weights *= 0.675703;
         if (LeptV2.Pt()>30 && LeptV2.Pt()<40) all_weights *= 0.635124;
         if (LeptV2.Pt()>40 ) all_weights *= 0.58225;
 }
 if (  fabs(LeptV2.Eta()) > 1.44 && fabs(LeptV2.Eta()) < 1.566 )
 {
        if (LeptV2.Pt()>20) all_weights *= 0.790876;
 }
 if (  fabs(LeptV2.Eta()) > 1.566 && fabs(LeptV2.Eta()) < 2.3 )
 {
        if (LeptV2.Pt()>20 && LeptV2.Pt()<30) all_weights *= 0.770244;
         if (LeptV2.Pt()>30 && LeptV2.Pt()<40) all_weights *= 0.707088;
         if (LeptV2.Pt()>40 ) all_weights *= 0.705259;
 }
}
			
			if (!(std::string::npos != HHname.find(SM) || std::string::npos != HHname.find(SE))  && ta_IsoFlagLoose[0] < 0.5 ) continue;


			// select real Tau in CR and substract from data
			if (!(std::string::npos != HHname.find(SM) || std::string::npos != HHname.find(SE)) && (ta_IsoFlagTight[0] < 0.5 && ta_IsoFlagLoose[0] > 0.5) && isfakeTauFromJet<0.5) 
				{
					all_weights = all_weights*(-1);
										/// transfer factor 
										/*
				if (fabs(LeptV2.Eta())<0.8)
					{
					if (LeptV2.Pt()<30) all_weights *= 0.74;
					if (LeptV2.Pt()>30 && LeptV2.Pt()<40) all_weights *= 0.66;
					if (LeptV2.Pt()>40) all_weights *= 0.56;
					}
				if (fabs(LeptV2.Eta())>0.8 && fabs(LeptV2.Eta())<1.44)
					{
					if (LeptV2.Pt()<30) all_weights *= 0.68;
					if (LeptV2.Pt()>30 && LeptV2.Pt()<40) all_weights *= 0.61;
					if (LeptV2.Pt()>40) all_weights *= 0.39;
					}

				if (fabs(LeptV2.Eta())>1.44 && fabs(LeptV2.Eta())<1.566)
					{
					if (LeptV2.Pt()<40) all_weights *= 0.68;
					if (LeptV2.Pt()>40) all_weights *= 0.64;
					}

				if (fabs(LeptV2.Eta())>1.566 && fabs(LeptV2.Eta())<2.3)
					{
					if (LeptV2.Pt()<40) all_weights *= 0.59;
					if (LeptV2.Pt()>40) all_weights *= 0.61;
					}*/
					
					
					 if (  fabs(LeptV2.Eta()) < 0.8 )
 {
        if (LeptV2.Pt()>20 && LeptV2.Pt()<30) all_weights *= 0.673922;
         if (LeptV2.Pt()>30 && LeptV2.Pt()<40) all_weights *= 0.643638;
         if (LeptV2.Pt()>40 ) all_weights *= 0.580981;
 }
 if (  fabs(LeptV2.Eta()) > 0.8 && fabs(LeptV2.Eta()) < 1.44 )
 {
        if (LeptV2.Pt()>20 && LeptV2.Pt()<30) all_weights *= 0.675703;
         if (LeptV2.Pt()>30 && LeptV2.Pt()<40) all_weights *= 0.635124;
         if (LeptV2.Pt()>40 ) all_weights *= 0.58225;
 }
 if (  fabs(LeptV2.Eta()) > 1.44 && fabs(LeptV2.Eta()) < 1.566 )
 {
        if (LeptV2.Pt()>20) all_weights *= 0.790876;
 }
 if (  fabs(LeptV2.Eta()) > 1.566 && fabs(LeptV2.Eta()) < 2.3 )
 {
        if (LeptV2.Pt()>20 && LeptV2.Pt()<30) all_weights *= 0.770244;
         if (LeptV2.Pt()>30 && LeptV2.Pt()<40) all_weights *= 0.707088;
         if (LeptV2.Pt()>40 ) all_weights *= 0.705259;
 }

					
					
				}

				
			if ( !(std::string::npos != HHname.find(SM) || std::string::npos != HHname.find(SE)) && ta_IsoFlagTight[0] && isfakeTauFromJet>0.5) continue;
			if ( !(std::string::npos != HHname.find(SM) || std::string::npos != HHname.find(SE)) && (ta_IsoFlagTight[0] < 0.5 && ta_IsoFlagLoose[0] > 0.5) && isfakeTauFromJet>0.5) continue;

		}
			
		// lepton fake tau
			
		double tfrmu  = MuonFakeRate(LeptV2.Pt(),LeptV2.Eta());
		double tfrel  = ElectronFakeRate(LeptV2.Pt(),LeptV2.Eta());


		if ( !isRealTau && isfakeTauFromMu  && !isfakeTauFromEl && !isData) all_weights *= tfrmu;
		if ( !isRealTau && isfakeTauFromEl  && !isfakeTauFromMu && !isData) all_weights *= tfrel;
			
			
			
			
			//if (OnlyPositiveWeights && gen_weight>0) all_weights = pu_weight;
			//if (OnlyPositiveWeights && gen_weight<0) continue;
                 	double Dr=deltaR(LeptV1.Eta(), LeptV1.Phi(),
					LeptV2.Eta(),LeptV2.Phi());
		///// iso cut.....
			if (mu_relIso[0] > 0.15 &&  channel == "mutau") continue;
			if (el_relIso[0] > 0.1 && channel == "eltau") continue;
			if ((el_relIso[0] > 0.1 || mu_relIso[0] > 0.15) && channel == "muel") continue;
			if (Dr>3.5) continue;
		//////////
//cout<<i <<endl;
		float charge_;
		if (channel == "mutau")
			{
			if ( fabs(mu_dz[muon_index]) > 0.2) continue;
			if ( fabs(mu_dxy[muon_index]) > 0.045) continue;
			if ( fabs(mu_charge[muon_index]) !=1 ) continue;
			if ( fabs(ta_charge[taus_index]) !=1 ) continue;
			 charge_ =mu_charge[muon_index]  * ta_charge[taus_index];
			if (event_secondLeptonVeto >0.5) continue;
			}

		if (channel == "eltau")
			{
			if ( fabs(el_dz[electron_index]) > 0.2) continue;
			if ( fabs(el_dxy[electron_index]) > 0.045) continue;
			if ( fabs(el_charge[electron_index]) !=1 ) continue;
			if ( fabs(ta_charge[taus_index]) !=1 ) continue;
			 charge_ =el_charge[electron_index]  * ta_charge[taus_index];
			if (event_secondLeptonVeto >0.5) continue;
			}

		if (channel == "muel")
			{
			if ( fabs(mu_charge[muon_index]) !=1 ) continue;
			if ( fabs(el_charge[electron_index]) !=1 ) continue;
			 charge_ =mu_charge[muon_index]  * el_charge[electron_index];
			if (event_secondLeptonVeto >0.5) continue;
			}



			if ( charge_ > 0.) continue;
			if (event_thirdLeptonVeto >0.5) continue;   ////////////???????? zdes' zakocnchil
			if (event_secondLeptonVeto >0.5) continue;


		if (channel == "muel") {float trigw =   ( 17677*trig_weight_1 +   18188 * trig_weight_2)/float(35865.) ;
					all_weights *= trigw;}
		else{all_weights *= trig_weight;}
			all_weights *= LSF_weight;
			// end of baseline selection
			if (njets>0.5) continue;
			if (nbtag!=0) continue;


         		double MT, dPhi;
		 	TLorentzVector DiL;




		if (channel == "eltau" || channel == "mutau")
			{
		 	DiL = LeptV1  ;
			dPhi=dPhiFrom2P( DiL.Px(), DiL.Py(), METV.Px(),  METV.Py() );
			MT = 0;MT=sqrt(2*DiL.Pt()*METV.Pt()*(1-TMath::Cos(dPhi)));

			if (MT > 60. && MT < 120. ) continue;
			if (MT < 20.) continue;
			}



		if (channel == "muel") 
			{
		 	DiL = LeptV1 + LeptV2 ;
			if (DiL.M() > 30. && DiL.M() < 90. ) continue;
			if (DiL.M() > 250.) continue;
			}


			dPhi=dPhiFrom2P( LeptV1.Px(), LeptV1.Py(), METV.Px(),  METV.Py() );		
			MT = TMath::Sqrt(2*LeptV1.Pt()*METV.Pt()*(1-TMath::Cos(dPhi)));


			dPhi=dPhiFrom2P( LeptV2.Px(), LeptV2.Py(), METV.Px(),  METV.Py() );
			float MTt = TMath::Sqrt(2*LeptV2.Pt()*METV.Pt()*(1-TMath::Cos(dPhi)));
			float MTsum = MT+MTt;
			float mttotal = sqrt(MT*MT + MTt*MTt);
			double deta=deltaEta(LeptV1.Px(),  LeptV1.Py(),LeptV1.Pz(), LeptV2.Px(),  LeptV2.Py(),LeptV2.Pz() );
			//double Dr = MuV.DeltaR(TauV);
			//cout <<"Dr      "<< Dr<< endl;

			//// additional cuts for mutau channel!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

			dEta = LeptV1.Eta() - LeptV2.Eta();
			double dEtaJMu = -1.; if (JetsMV.size()>0) dEtaJMu = JetsMV.at(0).Eta() - LeptV1.Eta();
			double dEtaJTau = -1.; if (JetsMV.size()>0) dEtaJTau = JetsMV.at(0).Eta() - LeptV2.Eta();
               		double Dr2=-1; if  (JetsMV.size()>0) {Dr2=deltaR(JetsMV.at(0).Eta(),JetsMV.at(0).Phi(), LeptV2.Eta(),LeptV2.Phi());}
			double dPhiDiL=dPhiFrom2P( LeptV1.Px(), LeptV1.Py(), LeptV2.Px(),  LeptV2.Py() );
			
			
			 double dMETPhiL1 = dPhiFrom2P(LeptV1.Px(),LeptV1.Py(),METV.Px(),METV.Py());
             double dMETPhiL2 = dPhiFrom2P(LeptV2.Px(),LeptV2.Py(),METV.Px(),METV.Py());

	/*	 	TLorentzVector DiLL = MuV +TauV ;
			if (fabs(dEta)>2) continue;
			if (DiLL.M()<20.) continue;
			if (MTsum<30.) continue;

			if (JetsMV.size()>0 && fabs(dEtaJMu)>3.) continue;
			if (JetsMV.size()>0 && fabs(dEtaJTau)>3.) continue;
			if (JetsMV.size()>0 && Dr2>4.) continue;


//// Stau cuts
			if ( fabs(mu_dz[muon_index]) > 0.04) continue;
			if ( fabs(mu_dxy[muon_index]) > 0.02) continue;
			if (ta_relIso[0] < 0.85) continue;

			if (fabs(dEta)>1.5) continue;
			if (dPhiDiL<1.5) continue;
			if (dPhiDiL>3.2) continue;
			if (Dr<2.) continue;
			if (Dr>3.2) continue;

			if (DiLL.M()<50.) continue;
			if (MTsum<50.) continue;
*/

			//if (fabs(deta) > 1.5) continue;
			//if (Dr > 3.) continue;
			//if (Dr < 2.) continue;
			//DiL = MuV  + TauV;
			//if (DiL.M() < 50.) continue;
			//if (MTsum < 50.) continue;


			///////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/*
			double ptTau1 = (double)ta_pt[taus_index];
			//ChargedIso CutBased MVA
			double ptN=0;double etaN=0;double FastSF=0;
			ptN = histo->GetXaxis()->FindBin(mu_pt[muon_index]);
			etaN = histo->GetYaxis()->FindBin(fabs(mu_eta[muon_index]));
			FastSF = histo->GetBinContent(ptN,etaN);
			//if (isSignal) all_weights *= FastSF; 


			all_weights *= top_weight;
			if (std::string::npos != HHname.find(DY)) all_weights *=zptmassweight;
			//some varible calculation

*/
			//TauFakeRate
/*			string WP = "MVA";
			double tfr  = TauFakeRate(ptTau1,etaTau1,channel,WP);
			if (genTauMatched<0.5 && genLeptonMatched < 0.5  && !isData) all_weights *= tfr;
*/
/*
			double ptMu1 = (double)mu_pt[muon_index];
			double tfrmu  = MuonFakeRate(ptMu1,etaMu1);
			double tfrel  = ElectronFakeRate(ptMu1,etaMu1);


			if ( (matchedTauToPromptMu > 0.5 && matchedTauToPromptEl < 0.5)) all_weights *= tfrmu;
			if ( (matchedTauToPromptEl > 0.5 && matchedTauToPromptMu < 0.5)) all_weights *= tfrel;
*/

/////////////////////////   search variables 







			/// DZETA calculation
				float tauUnitX = LeptV2.Px()/LeptV2.Pt();
				float tauUnitY = LeptV2.Py()/LeptV2.Pt();

				float muonUnitX = LeptV1.Px()/LeptV1.Pt();
				float muonUnitY = LeptV1.Py()/LeptV1.Pt();

				float zetaX = tauUnitX + muonUnitX;
				float zetaY = tauUnitY + muonUnitY;

				float normZeta = TMath::Sqrt(zetaX*zetaX+zetaY*zetaY);

				zetaX = zetaX/normZeta;
				zetaY = zetaY/normZeta;

				float vectorVisX = LeptV1.Px() + LeptV2.Px();
				float vectorVisY = LeptV1.Py() + LeptV2.Py();

				// computation of DZeta variable
				// pfmet
				float pzetamiss = METV.Px()*zetaX + METV.Py()*zetaY;
				float pzetavis = vectorVisX*zetaX + vectorVisY*zetaY;
				float dzeta = pzetamiss - 0.85*pzetavis;

				DiL = LeptV1  + LeptV2;
				double mcta = sqrt( 2*LeptV1.Pt()*LeptV2.Pt()*(1+cos(LeptV1.Phi()-LeptV2.Phi())) );
				double Mt2as = 0; 
				
				if (channel == "mutau") Mt2as = asymm_mt2_lester_bisect::get_mT2(muonMass, MuV.Px(), MuV.Py(),tauMass,TauV.Px(),TauV.Py(),METV.Px(),METV.Py(),0,0,0);
      				if (channel == "eltau") Mt2as = asymm_mt2_lester_bisect::get_mT2(electronMass, ElV.Px(), ElV.Py(),tauMass,TauV.Px(),TauV.Py(),METV.Px(),METV.Py(),0,0,0);
				if (channel == "muel") Mt2as = asymm_mt2_lester_bisect::get_mT2(muonMass, MuV.Px(), MuV.Py(),electronMass,ElV.Px(),ElV.Py(),METV.Px(),METV.Py(),0,0,0);

    double sumpT=0;

    if (JetsMV.size()>0) {for (unsigned int ij=0;ij<JetsMV.size();ij++){
      //sumpT+=jet_pt[ij];
      sumpT+=JetsMV.at(ij).Pt();
      //double dPhiJ=dPhiFrom2P( jet_px[ij], jet_py[ij], MetV.Px(),  MetV.Py() );
    }}

      double HTOsqrMET= (sumpT/sqrt(METV.Pt()));
      //hMeffMuonOsqrMET[CutIndex]->Fill( (sumMuonpT+sumpT+ MetV.Pt())/sqrt(MetV.Pt()),EvWeight);
      //hMeffElOsqrMET[CutIndex]->Fill( (sumElpT+sumpT+ MetV.Pt())/sqrt(MetV.Pt()),EvWeight);
      //hMeffOsqrMET[CutIndex]->Fill( (sumTaupT+sumpT+ MetV.Pt())/sqrt(MetV.Pt()),EvWeight);

	dEta = LeptV1.Eta() - LeptV2.Eta();

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
			<<"   11  "<< Mt2as *//*<<endl;*/
/*
			<<"   12  "<< MuV.Eta()
			<<"   13  "<< TauV.Eta()
			<<"   14  "<< dEta
			<<"   15  "<< dEtaJMu
			<<"   16  "<< dEtaJTau
			<<"   17  "<< mu_dz[muon_index]
			<<"   18  "<< mu_dxy[muon_index]
			<<"   19  "<< dPhiDiL
			<<"   20  "<< TauV.DeltaR(JetsMV.at(0))<<endl;*/
/*			<<"   21  "<< HTOsqrMET
			<<"   22  "<< JetsMV.at(0).Pt()
			<<"   23  "<< el_count
			<<"   24  "<< ta_count
			<<"   25  "<< mu_count <<endl;
*/
/*
			vector<Double_t> vars(8);
			vars[0] = METV.Pt();
			vars[1] = TauV.Pt();
			vars[2] = njets;
			vars[3] = mttotal;
			vars[4] = dzeta;
			vars[5] = DiL.M();
			vars[6] = Dr;
			vars[7] = Mt2as;
*/
/*
                        vector<Double_t> vars(4);
                        vars[0] = METV.Pt();
                        vars[1] = njets;
                        vars[2] = DiL.M();
                        vars[3] = Mt2as;
*/
//if (std::string::npos != HHname.find(stau)) hMt2lestermutau->Fill(Mt2as,MCweight*all_weights);

			vector<Double_t> vars(13);
			vars[0] = METV.Pt();
			vars[1] = LeptV1.Pt();
			vars[2] = LeptV2.Pt();
			vars[3] = dEta;
			vars[4] = mttotal;
			vars[5] = dzeta;
			vars[6] = DiL.M();
			vars[7] = Dr;
			vars[8] = MT;
			vars[9] = mcta;
			vars[10] = Mt2as;
			vars[11] = dMETPhiL1;
           		 vars[12] = dMETPhiL2;

/*
			vars[12] = MuV.Eta();
			vars[13] = TauV.Eta();

			vars[14] = dEtaJMu;
			vars[15] = dEtaJTau;
			vars[16] = mu_dz[muon_index];
			vars[17] = mu_dxy[muon_index];
			vars[18] = dPhiDiL;
			vars[19] = Dr2;*/

/*
			vars[21] = HTOsqrMET;
			vars[22] = JetPt;

			vars[23] = el_count;
			vars[24] = ta_count;
			vars[25] = mu_count;
*/


// additinal cuts

//if (mttotal<80) continue;
//if (dMETPhiL2 < 2) continue;
/*
   std::vector<double>* inputVec = new std::vector<double>( 13 );

			(*inputVec)[0] = METV.Pt();
			(*inputVec)[1] = LeptV1.Pt();
			(*inputVec)[2] = LeptV2.Pt();
			(*inputVec)[3] = dEta;
			(*inputVec)[4] = mttotal;
			(*inputVec)[5] = dzeta;
			(*inputVec)[6] = DiL.M();
			(*inputVec)[7] = Dr;
			(*inputVec)[8] = MT;
			(*inputVec)[9] = mcta;
			(*inputVec)[10] = Mt2as;
    	  		(*inputVec)[11] = dMETPhiL1;
    		        (*inputVec)[12] = dMETPhiL2;
*/
			
   //double retval = ANNResponse->GetMvaValue( *inputVec );
   //if (retval < 0.75) continue;

			//cout <<vars[0]<<vars[1]<<vars[2]<<vars[3] <<endl; 
			NselectedEv++;
			
			if (all_weights<0) NnegEvents++;
			
			if (std::string::npos != HHname.find(stau))
				{
					
					 //if (std::string::npos != HHname.find("Stau100"))          		 vars[13] = 100;
					 //if (std::string::npos != HHname.find("Stau150"))          		 vars[13] = 150;
					 //if (std::string::npos != HHname.find("Stau200"))          		 vars[13] = 200;


					EntriesProcTest = 0.0;
					if (std::string::npos != HHname.find("left")) EntriesProcTest = 0.2;
           			if (i < (EntriesProc*treeName->GetEntries())*(1-EntriesProcTest)) dataloader->AddSignalTrainingEvent( vars, MCweight*all_weights );
				//dataloader->AddSignalTrainingEvent( vars, 0.25*MCweight*all_weights );
           			else if (std::string::npos != HHname.find("left")) {dataloader->AddSignalTestEvent( vars, MCweight*all_weights );}
           			//else dataloader->AddSignalTestEvent( vars, MCweight*all_weights );
				//dataloader->AddSignalTestEvent( vars, 0.25*MCweight*all_weights );
				if (MCweight*all_weights >1) {
				cout << NselectedEv <<endl; 
				cout << MCweight*all_weights <<endl; 
				cout << MCweight <<endl; 
				cout << all_weights <<endl; }
				}
			else
				{
           			if (i < (EntriesProc*treeName->GetEntries())*(1-EntriesProcTest))	dataloader->AddBackgroundTrainingEvent( vars, MCweight*all_weights);
					//else dataloader->AddBackgroundTestEvent( vars, MCweight*all_weights );
           			 if (i > (EntriesProc*treeName->GetEntries())*(0.3))	dataloader->AddBackgroundTestEvent( vars, MCweight*all_weights);

           			
						/*{
						vars[13] = 100;
						dataloader->AddBackgroundTrainingEvent( vars, MCweight*all_weights);
						vars[13] = 150;
						dataloader->AddBackgroundTrainingEvent( vars, MCweight*all_weights);	
						vars[13] = 200;
						dataloader->AddBackgroundTrainingEvent( vars, MCweight*all_weights);												
							
						}
           			
           			else 
						{
						vars[13] = 100;	
						dataloader->AddBackgroundTestEvent( vars, MCweight*all_weights );
						}*/
        			}
		}

		cout << "N selected events = " << NselectedEv <<endl;
		cout << "N selected negative events = " << NnegEvents <<endl;

		i++;
    		}


	
    	datasets.close();
/*
    TCanvas* canv3 = new TCanvas("c3", "c3");
    hMt2lestermutau->Draw("sameh");
    canv3->Print("hMt2lestermutau2.pdf");
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 	END FILLING



   // Apply additional cuts on the signal and background samples (can be different)
   TCut mycuts = ""; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
   TCut mycutb = ""; // for example: TCut mycutb = "abs(var1)<0.5";


   // Tell the factory how to use the training and testing events
   //
   // If no numbers of events are given, half of the events in the tree are used 
   // for training, and the other half for testing:
       dataloader->PrepareTrainingAndTestTree( mycuts,"NormMode=None" );
   // To also specify the number of testing events, use:
   //    factory->PrepareTrainingAndTestTree( mycut,
   //                                         "NSigTrain=3000:NBkgTrain=3000:NSigTest=3000:NBkgTest=3000:SplitMode=Random:!V" );
   //factory->PrepareTrainingAndTestTree( mycuts, mycutb);

   // ---- Book MVA methods
   //
   // Please lookup the various method configuration options in the corresponding cxx files, eg:
   // src/MethoCuts.cxx, etc, or here: http://tmva.sourceforge.net/optionRef.html
   // it is possible to preset ranges in the option string in which the cut optimisation should be done:
   // "...:CutRangeMin[2]=-1:CutRangeMax[2]=1"...", where [2] is the third input variable

   // Cut optimisation
   if (Use["Cuts"])
      factory->BookMethod(  dataloader, TMVA::Types::kCuts, "Cuts",
                           "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart" );

   if (Use["CutsD"])
      factory->BookMethod(  dataloader, TMVA::Types::kCuts, "CutsD",
                           "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=Decorrelate" );

   if (Use["CutsPCA"])
      factory->BookMethod(  dataloader, TMVA::Types::kCuts, "CutsPCA",
                           "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=PCA" );

   if (Use["CutsGA"])
      factory->BookMethod(  dataloader, TMVA::Types::kCuts, "CutsGA",
                           "H:!V:FitMethod=GA:CutRangeMin[0]=-10:CutRangeMax[0]=10:VarProp[1]=FMax:EffSel:Steps=30:Cycles=3:PopSize=400:SC_steps=10:SC_rate=5:SC_factor=0.95" );

   if (Use["CutsSA"])
      factory->BookMethod(  dataloader, TMVA::Types::kCuts, "CutsSA",
                           "!H:!V:FitMethod=SA:EffSel:MaxCalls=150000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale" );

   // Likelihood ("naive Bayes estimator")
   if (Use["Likelihood"])
      factory->BookMethod(  dataloader, TMVA::Types::kLikelihood, "Likelihood",""
                           /*"H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50"*/ );

   // Decorrelated likelihood
   if (Use["LikelihoodD"])
      factory->BookMethod(  dataloader, TMVA::Types::kLikelihood, "LikelihoodD",
                           "!H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmooth=5:NAvEvtPerBin=50:VarTransform=Decorrelate" );

   // PCA-transformed likelihood
   if (Use["LikelihoodPCA"])
      factory->BookMethod(  dataloader, TMVA::Types::kLikelihood, "LikelihoodPCA",
                           "!H:!V:!TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmooth=5:NAvEvtPerBin=50:VarTransform=PCA" ); 

   // Use a kernel density estimator to approximate the PDFs
   if (Use["LikelihoodKDE"])
      factory->BookMethod(  dataloader, TMVA::Types::kLikelihood, "LikelihoodKDE",
                           "!H:!V:!TransformOutput:PDFInterpol=KDE:KDEtype=Gauss:KDEiter=Adaptive:KDEFineFactor=0.3:KDEborder=None:NAvEvtPerBin=50" ); 

   // Use a variable-dependent mix of splines and kernel density estimator
   if (Use["LikelihoodMIX"])
      factory->BookMethod(  dataloader, TMVA::Types::kLikelihood, "LikelihoodMIX",
                           "!H:!V:!TransformOutput:PDFInterpolSig[0]=KDE:PDFInterpolBkg[0]=KDE:PDFInterpolSig[1]=KDE:PDFInterpolBkg[1]=KDE:PDFInterpolSig[2]=Spline2:PDFInterpolBkg[2]=Spline2:PDFInterpolSig[3]=Spline2:PDFInterpolBkg[3]=Spline2:KDEtype=Gauss:KDEiter=Nonadaptive:KDEborder=None:NAvEvtPerBin=50" ); 

   // Test the multi-dimensional probability density estimator
   // here are the options strings for the MinMax and RMS methods, respectively:
   //      "!H:!V:VolumeRangeMode=MinMax:DeltaFrac=0.2:KernelEstimator=Gauss:GaussSigma=0.3" );
   //      "!H:!V:VolumeRangeMode=RMS:DeltaFrac=3:KernelEstimator=Gauss:GaussSigma=0.3" );
   if (Use["PDERS"])
      factory->BookMethod(  dataloader, TMVA::Types::kPDERS, "PDERS",
                           "!H:!V:NormTree=T:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600" );

   if (Use["PDERSD"])
      factory->BookMethod(  dataloader, TMVA::Types::kPDERS, "PDERSD",
                           "!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600:VarTransform=Decorrelate" );

   if (Use["PDERSPCA"])
      factory->BookMethod(  dataloader, TMVA::Types::kPDERS, "PDERSPCA",
                           "!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600:VarTransform=PCA" );

   // Multi-dimensional likelihood estimator using self-adapting phase-space binning
   if (Use["PDEFoam"])
      factory->BookMethod(  dataloader, TMVA::Types::kPDEFoam, "PDEFoam",
                           "!H:!V:SigBgSeparate=F:TailCut=0.001:VolFrac=0.0666:nActiveCells=500:nSampl=2000:nBin=5:Nmin=100:Kernel=None:Compress=T" );

   if (Use["PDEFoamBoost"])
      factory->BookMethod(  dataloader, TMVA::Types::kPDEFoam, "PDEFoamBoost",
                           "!H:!V:Boost_Num=30:Boost_Transform=linear:SigBgSeparate=F:MaxDepth=4:UseYesNoCell=T:DTLogic=MisClassificationError:FillFoamWithOrigWeights=F:TailCut=0:nActiveCells=500:nBin=20:Nmin=400:Kernel=None:Compress=T" );

   // K-Nearest Neighbour classifier (KNN)///////////////////////////////////1111111111111111111111111111111111111111111111111111111111111111111111111111111111
   if (Use["KNN"])
      factory->BookMethod(  dataloader, TMVA::Types::kKNN, "KNN",
                           "H:nkNN=20:ScaleFrac=0.8:SigmaFact=1.0:Kernel=Gaus:UseKernel=F:UseWeight=T:!Trim" );

   if (Use["KNNdown"])
      factory->BookMethod(  dataloader, TMVA::Types::kKNN, "KNNdown",
                           "H:nkNN=10:ScaleFrac=0.8:SigmaFact=1.0:Kernel=Gaus:UseKernel=F:UseWeight=T:!Trim" );


   if (Use["KNNup"])
      factory->BookMethod(  dataloader, TMVA::Types::kKNN, "KNNup",
                           "H:nkNN=40:ScaleFrac=0.8:SigmaFact=1.0:Kernel=Gaus:UseKernel=F:UseWeight=T:!Trim" );

   // H-Matrix (chi2-squared) method
   if (Use["HMatrix"])
      factory->BookMethod(  dataloader, TMVA::Types::kHMatrix, "HMatrix", "!H:!V:VarTransform=None" );

   // Linear discriminant (same as Fisher discriminant)
   if (Use["LD"])
      factory->BookMethod(  dataloader, TMVA::Types::kLD, "LD", "H:!V:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );

   // Fisher discriminant (same as LD)
   if (Use["Fisher"])
      factory->BookMethod(  dataloader, TMVA::Types::kFisher, "Fisher", "H:!V:Fisher:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );

   // Fisher with Gauss-transformed input variables
   if (Use["FisherG"])
      factory->BookMethod(  dataloader, TMVA::Types::kFisher, "FisherG", "H:!V:VarTransform=Gauss" );

   // Composite classifier: ensemble (tree) of boosted Fisher classifiers
   if (Use["BoostedFisher"])
      factory->BookMethod(  dataloader, TMVA::Types::kFisher, "BoostedFisher", 
                           "H:!V:Boost_Num=20:Boost_Transform=log:Boost_Type=AdaBoost:Boost_AdaBoostBeta=0.2:!Boost_DetailedMonitoring" );

   // Function discrimination analysis (FDA) -- test of various fitters - the recommended one is Minuit (or GA or SA)
   if (Use["FDA_MC"])
      factory->BookMethod(  dataloader, TMVA::Types::kFDA, "FDA_MC",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MC:SampleSize=100000:Sigma=0.1" );

   if (Use["FDA_GA"]) // can also use Simulated Annealing (SA) algorithm (see Cuts_SA options])
      factory->BookMethod(  dataloader, TMVA::Types::kFDA, "FDA_GA",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:PopSize=300:Cycles=3:Steps=20:Trim=True:SaveBestGen=1" );

   if (Use["FDA_SA"]) // can also use Simulated Annealing (SA) algorithm (see Cuts_SA options])
      factory->BookMethod(  dataloader, TMVA::Types::kFDA, "FDA_SA",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=SA:MaxCalls=15000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale" );

   if (Use["FDA_MT"])
      factory->BookMethod(  dataloader, TMVA::Types::kFDA, "FDA_MT",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=2:UseImprove:UseMinos:SetBatch" );

   if (Use["FDA_GAMT"])
      factory->BookMethod(  dataloader, TMVA::Types::kFDA, "FDA_GAMT",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:Cycles=1:PopSize=5:Steps=5:Trim" );

   if (Use["FDA_MCMT"])
      factory->BookMethod(  dataloader, TMVA::Types::kFDA, "FDA_MCMT",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MC:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:SampleSize=20" );

   // TMVA ANN: MLP (recommended ANN) -- all ANNs in TMVA are Multilayer Perceptrons
   if (Use["MLP"])
      factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLP");      
	//factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLP", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=50:!UseRegulator" );

   if (Use["MLP1"])
      factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLP1", "H:!V:NeuronType=sigmoid:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5" );
      //factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLP1", "H:!V:NeuronType=sigmoid:VarTransform=N:NCycles=1700:HiddenLayers=N+5,N:TestRate=5:UseRegulator" );


   if (Use["MLP2"])
      //factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLP2", "H:!V:NeuronType=tanh:VarTransform=N,G,D:NCycles=600:HiddenLayers=N+5:TestRate=5:!UseRegulator" );
      //factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLP2", "H:!V:NeuronType=sigmoid:VarTransform=N:NCycles=2000:HiddenLayers=N+5,N:TestRate=5:UseRegulator" );
      factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLP1", "Boost_Num=10:Boost_Type=AdaBoost:Boost_Transform=step:Boost_AdaBoostBeta=0.1:H:!V:NeuronType=sigmoid:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5" );

   if (Use["MLP3"])
      //factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLP3", "H:!V:NeuronType=sigmoid:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:!UseRegulator" );
      factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLP1", "Boost_Num=100:Boost_Type=AdaBoost:Boost_Transform=step:Boost_AdaBoostBeta=0.1:H:!V:NeuronType=sigmoid:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5" );

   if (Use["MLP4"])
      //factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLP4", "H:!V:NeuronType=sigmoid:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:UseRegulator" );
      factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLP1", "Boost_Num=10:Boost_Type=AdaBoost:Boost_Transform=linear:Boost_AdaBoostBeta=0.1:H:!V:NeuronType=sigmoid:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5" );

   if (Use["MLP5"])
      //factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLP5", "H:!V:NeuronType=sigmoid:VarTransform=N:NCycles=600:HiddenLayers=N+5,N:TestRate=5:UseRegulator" );
      factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLP1", "Boost_Num=10:Boost_Type=AdaBoost:Boost_Transform=log:Boost_AdaBoostBeta=0.1:H:!V:NeuronType=sigmoid:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5" );

   if (Use["MLP6"])
      //factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLP6", "H:!V:NeuronType=sigmoid:VarTransform=N:NCycles=1000:HiddenLayers=N+5,N:TestRate=5:UseRegulator" );
      factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLP1", "Boost_Num=10:Boost_Type=Bagging:H:!V:NeuronType=sigmoid:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5" );

   if (Use["MLPGA"])
      factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLPGA", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:TrainingMethod=GA" );

   if (Use["MLPGA1"])
      factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLPGA", "H:!V:NeuronType=sigmoid:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:TrainingMethod=GA:UseRegulator" );

   if (Use["MLPGA2"])
      factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLPGA", "H:!V:NeuronType=sigmoid:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:TrainingMethod=GA:!UseRegulator" );





   if (Use["MLPBFGS"])
      factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLPBFGS", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS:!UseRegulator" );

   if (Use["MLPBFGS1"])
      factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLPBFGS1", "Boost_Num=10:Boost_Type=AdaBoost:Boost_Transform=step:Boost_AdaBoostBeta=0.1:H:!V:NeuronType=tanh:VarTransform=N:NCycles=1000:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS:!UseRegulator" );

   if (Use["MLPBFGS2"])
      factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLPBFGS2", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5,N:TestRate=5:TrainingMethod=BFGS:!UseRegulator" );

   if (Use["MLPBFGS3"])
      factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLPBFGS3", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=1000:HiddenLayers=N+5,N:TestRate=5:TrainingMethod=BFGS:!UseRegulator" );




   if (Use["MLPBNN"])
      factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLPBNN", "Boost_Num=10:Boost_Type=AdaBoost:Boost_Transform=step:H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS:UseRegulator" ); // BFGS training with bayesian regulators

   if (Use["MLPBNN1"])
      factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLPBNN1", "Boost_Num=10:Boost_Type=AdaBoost:Boost_Transform=step:Boost_AdaBoostBeta=0.1:H:!V:NeuronType=tanh:VarTransform=N:NCycles=1000:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS:UseRegulator" ); // BFGS training with bayesian regulators

   if (Use["MLPBNN2"])
      factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLPBNN2", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5,N:TestRate=5:TrainingMethod=BFGS:UseRegulator" ); // BFGS training with bayesian regulators

   if (Use["MLPBNN3"])
      factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLPBNN3", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=1000:HiddenLayers=N+5,N:TestRate=5:TrainingMethod=BFGS:UseRegulator" ); // BFGS training with bayesian regulators

   if (Use["MLPBNN4"])
      factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLPBNN4", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=1500:HiddenLayers=N+5,N:TestRate=5:TrainingMethod=BFGS:UseRegulator" ); // BFGS training with bayesian regulators

   if (Use["MLPBNN5"])
      factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLPBNN5", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=2000:HiddenLayers=N+5,N:TestRate=5:TrainingMethod=BFGS:UseRegulator" ); // BFGS training with bayesian regulators

   if (Use["MLPBNN6"])
      factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLPBNN6", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=2500:HiddenLayers=N+5,N:TestRate=5:TrainingMethod=BFGS:UseRegulator" ); // BFGS training with bayesian regulators

   if (Use["MLPBNN7"])
      factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLPBNN7", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=3000:HiddenLayers=N+5,N:TestRate=5:TrainingMethod=BFGS:UseRegulator" ); // BFGS training with bayesian regulators


   if (Use["MLPBNNmoreHiddenLayers"])
      factory->BookMethod(  dataloader, TMVA::Types::kMLP, "MLPBNNmoreHiddenLayers", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=1000:HiddenLayers=N+5,N,N:TestRate=5:TrainingMethod=BFGS:UseRegulator" ); // BFGS training with bayesian regulators







   // CF(Clermont-Ferrand)ANN
   if (Use["CFMlpANN"])
      factory->BookMethod(  dataloader, TMVA::Types::kCFMlpANN, "CFMlpANN", "!H:!V:NCycles=2000:HiddenLayers=N+1,N"  ); // n_cycles:#nodes:#nodes:...  

   // Tmlp(Root)ANN
   if (Use["TMlpANN"])
      factory->BookMethod(  dataloader, TMVA::Types::kTMlpANN, "TMlpANN", "!H:!V:NCycles=200:HiddenLayers=N+1,N:LearningMethod=BFGS:ValidationFraction=0.3"  ); // n_cycles:#nodes:#nodes:...

   // Support Vector Machine///////////////////////////////////1111111111111111111111111111111111111111111111111111111111111111111111111111111111
   if (Use["SVM"])
      factory->BookMethod(  dataloader, TMVA::Types::kSVM, "SVM", "Gamma=0.25:Tol=0.001:VarTransform=Norm:MaxIter=2000" );

   if (Use["SVM1"])
      factory->BookMethod(  dataloader, TMVA::Types::kSVM, "SVM1", "Gamma=1:Tol=0.001:VarTransform=Norm:MaxIter=2000" );

   if (Use["SVM2"])
      factory->BookMethod(  dataloader, TMVA::Types::kSVM, "SVM2", "Gamma=0.25:Tol=0.01:VarTransform=Norm:MaxIter=2000" );

   if (Use["SVM3"])
      factory->BookMethod(  dataloader, TMVA::Types::kSVM, "SVM3", "Gamma=0.25:Tol=0.001:VarTransform=Norm:MaxIter=20000" );

   if (Use["SVM4"])
      factory->BookMethod(  dataloader, TMVA::Types::kSVM, "SVM4", "Gamma=0.25:C=2:Tol=0.001:VarTransform=Norm:MaxIter=2000" );


/*   if (Use["SVMdownGamma"])
      factory->BookMethod(  dataloader, TMVA::Types::kSVM, "SVMdownGamma", "Gamma=0.1:Tol=0.001:VarTransform=Norm:MaxIter=2000" );

   if (Use["SVMupTol"])
      factory->BookMethod(  dataloader, TMVA::Types::kSVM, "SVMupTol", "Gamma=0.25:Tol=0.01:VarTransform=Norm:MaxIter=2000" );

   if (Use["SVMdownTol"])
      factory->BookMethod(  dataloader, TMVA::Types::kSVM, "SVMdownTol", "Gamma=0.25:Tol=0.0001:VarTransform=Norm:MaxIter=2000" );

   if (Use["SVMdownC"])
      factory->BookMethod(  dataloader, TMVA::Types::kSVM, "SVMdownC", "Gamma=0.25:Tol=0.001:VarTransform=Norm:C=0.5:MaxIter=2000" );

   if (Use["SVMupC"])
      factory->BookMethod(  dataloader, TMVA::Types::kSVM, "SVMupC", "Gamma=0.25:Tol=0.001:VarTransform=Norm:C=2:MaxIter=2000" );
*/
   // Boosted Decision Trees///////////////////////////////////1111111111111111111111111111111111111111111111111111111111111111111111111111111111
   if (Use["BDT"+BDTname])  // Adaptive Boost
      {
	//	factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT"+BDTname,
                 //          "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=SDivSqrtSPlusB:nCuts=20" );
	//		"!H:!V:NTrees=850:MinNodeSize=0.2%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=35"/*:NegWeightTreatment=Pray*/ );

		factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT"+BDTname,
			meth );


		  
		  //factory->BookMethod(  dataloader, TMVA::Types::kBDT, "BDT"+BDTname,
           //                "!H:!V:NTrees=850:MinNodeSize=0.2%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=SDivSqrtSPlusB:nCuts=35:DoBoostMonitor=true"/*:UseFisherCuts=True:SkipNormalization=true/*:NegWeightTreatment=Pray*NegWeightTreatment=PairNegWeightsGlobal:*/ );
		//BDTmutau->SkipNormalization("false");SeparationType=SDivSqrtSPlusB
		}
   if (Use["BDTG"]) // Gradient Boost
      factory->BookMethod(  dataloader, TMVA::Types::kBDT, "BDTG",
                           "!H:!V:NTrees=1000:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=35:MaxDepth=2" );

   if (Use["BDT"])  // Adaptive Boost
      factory->BookMethod(  dataloader, TMVA::Types::kBDT, "BDT",
                           "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );

/*   if (Use["BDTmutau"])  // Adaptive Boost
      factory->BookMethod(  dataloader, TMVA::Types::kBDT, "BDTmutau",
                           "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );

   if (Use["BDTeltau"])  // Adaptive Boost
      factory->BookMethod(  dataloader, TMVA::Types::kBDT, "BDTeltau",
                           "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );

   if (Use["BDTmuel"])  // Adaptive Boost
      factory->BookMethod(  dataloader, TMVA::Types::kBDT, "BDTmuel",
                           "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );
*/
   if (Use["BDT1"])  // Adaptive Boost
      factory->BookMethod(  dataloader, TMVA::Types::kBDT, "BDT1",
                           "!H:!V:NTrees=10000:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );

   if (Use["BDT2"])  // Adaptive Boost
      factory->BookMethod(  dataloader, TMVA::Types::kBDT, "BDT2",
                           "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );

   if (Use["BDT3"])  // Adaptive Boost
      factory->BookMethod(  dataloader, TMVA::Types::kBDT, "BDT3",
                           "!H:!V:NTrees=850:MinNodeSize=5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );

   if (Use["BDT4"])  // Adaptive Boost
      factory->BookMethod(  dataloader, TMVA::Types::kBDT, "BDT4",
                           "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );

   if (Use["BDT5"])  // Adaptive Boost
      factory->BookMethod(  dataloader, TMVA::Types::kBDT, "BDT5",
                           "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20:UseRandomisedTrees=True" );
   if (Use["BDT6"])  // Adaptive Boost
      factory->BookMethod(  dataloader, TMVA::Types::kBDT, "BDT6",
                           "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=50" );

   if (Use["BDT7"])  // Adaptive Boost
      factory->BookMethod(  dataloader, TMVA::Types::kBDT, "BDT7",
                           "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=-1" );

 if (Use["BDT8"])  // Adaptive Boost
      factory->BookMethod(  dataloader, TMVA::Types::kBDT, "BDT8",
                           "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.1:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );


   if (Use["BDTB"]) // Bagging
      factory->BookMethod(  dataloader, TMVA::Types::kBDT, "BDTB",
                           "!H:!V:NTrees=1000:BoostType=Bagging:SeparationType=GiniIndex:nCuts=20" );

   if (Use["BDTD"]) // Decorrelation + Adaptive Boost
      factory->BookMethod(  dataloader, TMVA::Types::kBDT, "BDTD",
                           "!H:!V:NTrees=1000:MinNodeSize=5%:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:VarTransform=Decorrelate" );

   if (Use["BDTF"])  // Allow Using Fisher discriminant in node splitting for (strong) linearly correlated variables
      factory->BookMethod(  dataloader, TMVA::Types::kBDT, "BDTMitFisher",
                           "!H:!V:NTrees=1000:MinNodeSize=2.5%:UseFisherCuts:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=20" );

   // RuleFit -- TMVA implementation of Friedman's method
   if (Use["RuleFit"])
      factory->BookMethod(  dataloader, TMVA::Types::kRuleFit, "RuleFit",
                           "H:!V:RuleFitModule=RFTMVA:Model=ModRuleLinear:MinImp=0.001:RuleMinDist=0.001:NTrees=20:fEventsMin=0.01:fEventsMax=0.5:GDTau=-1.0:GDTauPrec=0.01:GDStep=0.01:GDNSteps=10000:GDErrScale=1.02" );

   // For an example of the category classifier usage, see: TMVAClassificationCategory

   // --------------------------------------------------------------------------------------------------

   // ---- Now you can optimize the setting (configuration) of the MVAs using the set of training events

   // ---- STILL EXPERIMENTAL and only implemented for BDT's ! 
   // factory->OptimizeAllMethods("SigEffAt001","Scan");
   // factory->OptimizeAllMethods("ROCIntegral","FitGA");

   // --------------------------------------------------------------------------------------------------

   // ---- Now you can tell the factory to train, test, and evaluate the MVAs

   // Train MVAs using the set of training events
   factory->TrainAllMethods();

   // ---- Evaluate all MVAs using the set of test events
   factory->TestAllMethods();

   // ----- Evaluate and compare performance of all configured MVAs
   factory->EvaluateAllMethods();

   // --------------------------------------------------------------

   // Save the output
   outputFile->Close();

   std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
   std::cout << "==> TMVAClassification is done!" << std::endl;

   delete factory;
   delete dataloader;
   // Launch the GUI for the root macros
   //if (!gROOT->IsBatch()) TMVA::TMVAGui( outfileName );
   TMVA::TMVAGui( outfileName );
   return 0;
}

int main( int argc, char** argv )
{
   // Select methods (don't look at this code - not of interest)
   TString methodList; 
   for (int i=1; i<argc; i++) {
      TString regMethod(argv[i]);
      if(regMethod=="-b" || regMethod=="--batch") continue;
      if (!methodList.IsNull()) methodList += TString(","); 
      methodList += regMethod;
   }
   return myTMVA(methodList); 
}


