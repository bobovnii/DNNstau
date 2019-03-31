#include <sstream>
#include <iomanip>
#include "TChain.h"
#include "TH1.h"
#include "TTree.h"
#include "TKey.h"
#include "Riostream.h"
#include "TCanvas.h"
#include "TFile.h"
#include <string>
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
#include "TPie.h"
#include "Riostream.h"
#include <iostream>
#include <fstream>
#include "tdrstyle.C"
#include "CMS_lumi.C"


using namespace std;

void Impose( TDirectory *ttarget, TList *ssourcelist, string &np_legend , vector<string> titles_ ,vector<float> xsecs);
void ModifyHist (TH1D* &h, int cl,vector <string> title);
void ModifyHist (TH1D* &h, int cl,vector <string> title, TLegend * & tleg);
void ModifyHist (TH1D* &h, int cl);
void OverFlow (TH1D* &h, int bin);
void FixBoxPadding(TPad *pad, TBox *box, TH1D* &h1, double frac);

void FixOverlay();
void FixTopRange(TPad *pad, TH1D* &h1, double fix_y, double fraction);
ReBin(TH1* &h);
TCanvas *modifyCanvas (TCanvas *c1);
TCanvas *example_plot(int iPeriod, int iPos , TString name);


int mycolor=TColor::GetColor("#ffcc66");
int mycolorvv=TColor::GetColor("#8646ba");
//int mycolorww=TColor::GetColor("#6F2D35");
//int mycolorvv=TColor::GetColor("#FF6633");
int mycolorqcd=TColor::GetColor("#33CCFF");
//int mycolorqcd=TColor::GetColor("#ffccff");
//int mycolorqcd=TColor::GetColor("#de5a6a");
int mycolortt=TColor::GetColor("#9999cc");
//int mycolorttx=TColor::GetColor("#bbccdd");
int mycolorttx=TColor::GetColor("#33CCFF");
int mycolorwjet=TColor::GetColor("#de5a6a");
//int mycolorwjet=TColor::GetColor("#66CC66");
int mycolordyj=TColor::GetColor("#ffcc66");
int mycolorztt=TColor::GetColor("#58d885");
int mycolorst=TColor::GetColor("#b6d0ea");

int mycolorww=TColor::GetColor("#C390D4");


TH1D* WInclw,*W1Jw,*W2Jw,*W3Jw,*W4Jw;
TH1D* DYInclw,*DY1Jw,*DY2Jw,*DY3Jw,*DY4Jw;


	bool norm_=false;
	bool SO_=true;
	bool WDD=true;
	bool QCDMC=false;
        int MaxEventsBin = 0.5;
	bool drawData=true;
	TString Channel="mutau";
void OverlapWDD()
{
	setTDRStyle();

	writeExtraText = true;       // if extra text
	extraText  = "Preliminary";  // default extra text is "Preliminary"
	lumi_8TeV  = "19.1 fb^{-1}"; // default is "19.7 fb^{-1}"
	lumi_7TeV  = "4.9 fb^{-1}";  // default is "5.1 fb^{-1}"
	lumi_sqrtS = "13 TeV";       // used with iPeriod = 0, e.g. for simulation-only plots (default is an empty string)

	int iPeriod = 3;    // 1=7TeV, 2=8TeV, 3=7+8TeV, 7=7+8+13TeV, 0=free form (uses lumi_sqrtS)

	// second parameter in example_plot is iPos, which drives the position of the CMS logo in the plot
	// iPos=11 : top-left, left-aligned
	// iPos=33 : top-right, right-aligned
	// iPos=22 : center, centered
	// mode generally :
	//   iPos = 10*(alignement 1/2/3) + position (1/2/3 = left/center/right)

	//example_plot( iPeriod, 0 );   // out of frame (in exceptional cases)
	//  example_plot( iPeriod, 11 );  // left-aligned
	//  example_plot( iPeriod, 33 );  // right-aligned

	//  writeExtraText = false;       // remove Preliminary

	//  example_plot( iPeriod, 0 );   // out of frame (in exceptional cases)

	//  example_plot( iPeriod, 11 );  // default: left-aligned
	//  example_plot( iPeriod, 22 );  // centered
	//  example_plot( iPeriod, 33 );  // right-aligned

	vector <string> titles;
	TList *FileList;
	TFile *Target;
	titles.clear();
	int np=1;

	Float_t value=0;
	vector<float> xsecs_;
	ifstream ifs("datasetsWDD");
	if( QCDMC)  ifstream ifs("datasetsQCD");
	//if(WDD)  ifstream ifs("datasetsWDD");
	string line;
	while(std::getline(ifs, line)) // read one line from ifs
	{
		istringstream iss(line); // access line as a stream
		string dataname;
		float XSec;	
		float xs,fact,fact2,fact3;
		xs=0;fact=1;fact2=1;fact3=1;
		iss >> dataname >> xs >> fact >> fact2 >> fact3;
		titles.push_back(dataname+"_Nominal_B.root");
		XSec= xs*fact*fact2*fact3;
		if ((std::string::npos == dataname.find("Stau"))) XSec = XSec/0.7; /////////////////////important!!!!!!!!!

		cout<<" Found the correct cross section "<<xs<<" for Dataset "<<dataname<<" XSec "<<XSec<<"  "<<fact<<"  "<<fact2<<"  "<<fact<<endl;
		xsecs_.push_back(XSec);
	}

	string fout = "mergedplotsTau.root";
			
	FileList = new TList ();


	for (unsigned int i=0; i <titles.size();++i){
		//string ext=".root";
		cout<<" loading dataset "<<titles[i]<<endl;
		//string file=titles[i]+".root";
		string file=titles[i];
		FileList->Add (TFile::Open (file.c_str()));

	}

	Target = TFile::Open (fout.c_str (), "RECREATE");

	string np_title = titles[0];


                                TString WInclname, W1Jname, W2Jname, W3Jname, W4Jname;
                                TString DYInclname, DY1Jname, DY2Jname, DY3Jname, DY4Jname;
                                TString app_="";

                                        WInclname = "WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"+app_+"_Nominal_B.root";
                                        W1Jname = "W1JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"+app_+"_Nominal_B.root";
                                        W2Jname = "W2JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"+app_+"_Nominal_B.root";
                                        W3Jname = "W3JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"+app_+"_Nominal_B.root";
                                        W4Jname = "W4JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"+app_+"_Nominal_B.root";

                                        DYInclname = "DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"+app_+"_Nominal_B.root";
                                        DY1Jname = "DY1JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"+app_+"_Nominal_B.root";
                                        DY2Jname = "DY2JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"+app_+"_Nominal_B.root";
                                        DY3Jname = "DY3JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"+app_+"_Nominal_B.root";
                                        DY4Jname = "DY4JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"+app_+"_Nominal_B.root";

                                TFile *WIncl = TFile::Open (WInclname, "read");
                 
  	               
			TFile *W1J = TFile::Open (W1Jname, "read");
                        TFile *W2J = TFile::Open (W2Jname, "read");
		        TFile *W3J = TFile::Open (W3Jname, "read");
	                TFile *W4J = TFile::Open (W4Jname, "read");
                        TFile *DYIncl = TFile::Open (DYInclname, "read");
			TFile *DY1J = TFile::Open (DY1Jname, "read");
                        TFile *DY2J = TFile::Open (DY2Jname, "read");
                        TFile *DY3J = TFile::Open (DY3Jname, "read");
                        TFile *DY4J = TFile::Open (DY4Jname, "read");


                        DYInclw= (TH1D*)DYIncl->Get(Channel+"/histWeightsH");
                        DY1Jw= (TH1D*)DY1J->Get(Channel+"/histWeightsH");
                        DY2Jw= (TH1D*)DY2J->Get(Channel+"/histWeightsH");
                        DY3Jw= (TH1D*)DY3J->Get(Channel+"/histWeightsH");
                        DY4Jw= (TH1D*)DY4J->Get(Channel+"/histWeightsH");
		        WInclw= (TH1D*)WIncl->Get(Channel+"/histWeightsH");
                        W1Jw= (TH1D*)W1J->Get(Channel+"/histWeightsH");
                        W2Jw= (TH1D*)W2J->Get(Channel+"/histWeightsH");
                        W3Jw= (TH1D*)W3J->Get(Channel+"/histWeightsH");
                        W4Jw= (TH1D*)W4J->Get(Channel+"/histWeightsH");



	Impose (Target, FileList, np_title,titles,xsecs_);
	delete FileList;
	delete Target;
}


	void
Impose (TDirectory * target, TList * sourcelist, string & np_title_, vector<string> titles,vector<float> xsecs)
{
	cout << "	" << "========================================================" << endl;
	cout << "	" << "This is a macro to superimpose plots of different root files." << endl;
	cout << "	" << "Only TH1Dobjects are superimposed." << endl;
	cout << "	" << "Target path: " << target->GetPath () << endl;
	TString path ((char *) strstr (target->GetPath (), ":"));
	path.Remove (0, 2);
	float Lumi= 35864.;
	cout<<titles[0]<<"   "<<titles.size()<<endl;

	//not really useful if plots already weighted to lumi - usefull is plots are in a.u.
	vector <float > lumiweights;
	vector <string > signal_names;
	signal_names.clear();
	lumiweights.clear();
	string sign_="stau";
	string n_;
	for (unsigned int k=0; k<titles.size();k++)
	{
		if (   std::string::npos != titles[k].find(sign_)){
			n_ = titles[k];
			n_.erase(n_.length()-5);
			string nn_ = n_+"_out.root";
			signal_names.push_back(nn_.c_str());
		}
	}
	for (unsigned int k=0; k<signal_names.size();k++){
		cout<<" HERE ==========================  "<<signal_names[k]<<endl;
	}

	TH1D* allbkg, *htt,*hstop,*hwj,*hdyj,*hztt,*hrare,*hdib,*hww,*hqcd,*httx;

	TFile *first_source = (TFile *) sourcelist->First ();
	string fsourcename = first_source->GetName();
	first_source->cd (Channel);

	TH1D* eventCount = (TH1D*)first_source->Get(Channel+"/histWeightsH");
	float nGen = eventCount->GetSumOfWeights();
	float xsec = 1;//hxsec->GetMean();
	float norm = xsec*Lumi/nGen;

	norm =1;
	lumiweights.push_back(float(norm));


	//cout<< " for first source file, there where "<<nGen<<" events with xsec "<<xsec<<" weight "<<lumiweights[0]<<endl;//" weight "<<float(xsec*Lumi/nGen)<<"  norm "<<norm<<endl;

	TDirectory *current_sourcedir = gDirectory;
	//gain time, do not add the objects in the list in memory
	Bool_t status = TH1::AddDirectoryStatus ();
	TH1::AddDirectory (kFALSE);
	// loop over all keys in this directory
	TChain *globChain = 0;
	TIter nextkey (current_sourcedir->GetListOfKeys ());
	//TIter nextkey (((TDirectory *) current_sourcedir->Get ("ana"))->GetListOfKeys ());
	TKey *key, *oldkey = 0;
	int count=0;
//	TCanvas *c1 = new TCanvas ("c1","c1",0,22,600,600);//
          auto c1 = new TCanvas("c1","c1",200,10,600,600);
	while ((key = (TKey *) nextkey ())) {
		count++;
		//if (count>20) break;
		//keep only the highest cycle number for each key
		//        if (oldkey && !strcmp (oldkey->GetName (), key->GetName ()))
		//            continue;

		// read object from first source file and create a canvas
		first_source->cd (Channel);
		TObject *obj = key->ReadObj ();

		//string nn = obj->GetName();
		// if (std::string::npos == nn.find("Cut")) continue;
		//cout<<obj->GetName()<<endl;
		//c1->SetName(obj->GetName());
		c1->SetTitle(obj->GetName());


		//TCanvas *c1 = example_plot(3, 0 , obj->GetName ());

		string nn = obj->GetName();
		bool flcont = true;
		//if ( string::npos == nn.find("CutFlowUnW")) flcont=false;
	//	if (string::npos == nn.find("1D_") ) {MaxEventsBin = -1;drawData = false;  flcont=false;};
		//if (string::npos == nn.find("Mode") ) {MaxEventsBin = -1;drawData = false;  flcont=false;};
		//if (string::npos == nn.find("BDTmutau") ||  (string::npos == nn.find("_15") && string::npos == nn.find("_14")  && string::npos == nn.find("_13") && string::npos == nn.find("_16") && string::npos == nn.find("_17"))) flcont=false;
		//if (string::npos != nn.find("Delta") ) continue;
		if (string::npos == nn.find("BDTmutau") ||  (string::npos == nn.find("_16"))) flcont=false;
		//if (string::npos == nn.find("_18") && string::npos == nn.find("_17")  && string::npos == nn.find("_19")  && string::npos == nn.find("_20")  && string::npos == nn.find("_16") && string::npos == nn.find("_21") && string::npos == nn.find("_22") && string::npos == nn.find("_15")) flcont=false;
	//	if (string::npos == nn.find("_15") && string::npos == nn.find("_14")  && string::npos == nn.find("_13") && string::npos == nn.find("_16") && string::npos == nn.find("_17") && string::npos == nn.find("_18")) flcont=false;
//if (string::npos == nn.find("_16"))  flcont=false;
		if (!flcont) continue;
	//	if (std::string::npos != nn.find("Mt2") ||  std::string::npos != nn.find("DZeta_") ||  std::string::npos != nn.find("MET_") ) {MaxEventsBin = -1;}

		if (obj->IsA ()->InheritsFrom ("TTree") ) continue;
		if (obj->IsA ()->InheritsFrom ("TH2") ) continue;
		if (obj->IsA ()->InheritsFrom ("TH3") ) continue;
		if (obj->IsA ()->InheritsFrom ("TH1D") ) {
			// descendant of TH1D-> prepare the histograms to be superimposed

			TH1D* hh[500];
			TH1D* hsignal[500];
			TH1D* h1 = (TH1D*) obj;
			if (h1->Integral() <0.0001) continue;
      		        //if ( string::npos == nn.find("Iso")) MaxEventsBin=0; 


			TLegend *legend_c1 ;

if (SO_) legend_c1    = new TLegend (0.35, 0.92, 0.95, 0.55);
if (!SO_) legend_c1    = new TLegend (0.65, 0.92, 0.9, 0.55);

			TLegend *legend_c2 = new TLegend (0.3, 0.92, 0.5, 0.8);
			legend_c1->SetFillColor(1);
			legend_c1->SetFillStyle(0);
			legend_c1->SetLineColor(0);
			legend_c1->SetTextFont (132);
			legend_c1->SetTextSize (0.035);
			legend_c2->SetFillColor(1);
			legend_c2->SetFillStyle(0);
			legend_c2->SetLineColor(0);
			legend_c2->SetTextFont (132);
			legend_c2->SetTextSize (0.035);
if (SO_)
			 legend_c1-> SetNColumns(2);
			ModifyHist (h1,1,Lumi,lumiweights[0],titles[0],false);
			//hh[1]->GetXaxis()->SetRange(hh[1]->FindFirstBinAbove(0),hh[1]->FindLastBinAbove(MaxEventsBin));	

			// loop over all source files and modify the correspondant
			// histogram to the one pointed to by "h1"
			TFile *nextsource = (TFile *) sourcelist->After (first_source);

			int cl, countsignal;
			h1->SetStats(000000);
			h1->SetLineWidth(2);
			cl=1;
			countsignal=1;

			hh[cl]=h1;

			THStack *hs = new THStack(h1->GetName(),h1->GetTitle());


			string sn="Stau";
			string sdata="Single";
			string sdata2="MuonEG";
			string cc1="C1";


			while (nextsource) {
	
			string fname= nextsource->GetName();


					bool flagg= false;

					if (std::string::npos != fname.find(sn) || std::string::npos != fname.find(cc1) || std::string::npos != fname.find(sdata)  || std::string::npos != fname.find(sdata2)  ) 	flagg=true;

					//if (flagg) cout<<"=============================================================== "<<fname<<endl;
					// make sure we are at the correct directory level by cd'ing to path

					nextsource->cd(Channel);
					TH1D* eventCountt ;

				  if  ( std::string::npos == fname.find("TT_TuneCUETP8M2T4_13TeV-powheg-pythia8") && std::string::npos == fname.find("DataDriven")) eventCountt = (TH1D*)nextsource->Get(Channel+"/histWeightsH");
				  if ( std::string::npos != fname.find("TT_TuneCUETP8M2T4_13TeV-powheg-pythia8") ) eventCountt = (TH1D*)nextsource->Get(Channel+"/histTopPt");


					float xsecc = xsecs[cl];

					float nGenn = eventCountt->GetSumOfWeights();
					if ( std::string::npos != fname.find("DataDriven")) nGenn=1.;

					float normm = float(xsecc*Lumi) / float(nGenn)  ;
					string qcd="Fakes";
					if (std::string::npos != fname.find(qcd)) { normm =xsecc;}


                                        if (std::string::npos != fname.find("WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_1Parton") || std::string::npos != fname.find("W1JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"))
                                                normm = 1.428*Lumi/ ( WInclw->GetSumOfWeights()/float(61526.7*0.8853) + W1Jw->GetSumOfWeights()/float(1.221*9644.5*0.8853));


                                        if (std::string::npos != fname.find("WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_2Parton") || std::string::npos != fname.find("W2JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"))
                                                normm = 1.428*Lumi/ ( WInclw->GetSumOfWeights()/float(61526.7*0.8853) + W2Jw->GetSumOfWeights()/float(1.221*3144.5*0.8853));

                                        if (std::string::npos != fname.find("WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_3Parton") || std::string::npos != fname.find("W3JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"))
                                                normm = 1.428*Lumi/ ( WInclw->GetSumOfWeights()/float(61526.7*0.8853) + W3Jw->GetSumOfWeights()/float(1.221*954.8*0.8853));

                                        if (std::string::npos != fname.find("WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_4Parton") || std::string::npos != fname.find("W4JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"))
                                                normm = 1.428*Lumi/ ( WInclw->GetSumOfWeights()/float(61526.7*0.8853) + W4Jw->GetSumOfWeights()/float(1.221*485.6*0.8853));


                                        if (std::string::npos != fname.find("DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_1Parton") || std::string::npos != fname.find("DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_isZTT_1Parton") || std::string::npos != fname.find("DY1JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"))
                                                normm = 1.428*Lumi/ ( float(DYInclw->GetSumOfWeights())/float(5765.4*0.9611) + float(DY1Jw->GetSumOfWeights())/float(1.1637*1012.5*0.9611));


                                        if (std::string::npos != fname.find("DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_2Parton") || std::string::npos != fname.find("DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_isZTT_2Parton") || std::string::npos != fname.find("DY2JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"))
                                                normm = 1.428*Lumi/ ( DYInclw->GetSumOfWeights()/float(5765.4*0.9611) + float(DY2Jw->GetSumOfWeights())/float(1.1637*332.8*0.9611));

                                        if (std::string::npos != fname.find("DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_3Parton") || std::string::npos != fname.find("DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_isZTT_3Parton") || std::string::npos != fname.find("DY3JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"))
                                                normm = 1.428*Lumi/ ( DYInclw->GetSumOfWeights()/float(5765.4*0.9611) + float(DY3Jw->GetSumOfWeights())/float(1.1637*101.8*0.9611));

                                        if (std::string::npos != fname.find("DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_4Parton") || std::string::npos != fname.find("DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_isZTT_4Parton") || std::string::npos != fname.find("DY4JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"))
                                                normm = 1.428*Lumi/ ( DYInclw->GetSumOfWeights()/float(5765.4*0.9611) + float(DY4Jw->GetSumOfWeights())/float(1.1637*54.8*0.9611));





					lumiweights.push_back(normm);

					TKey *key2 = (TKey *) gDirectory->GetListOfKeys ()->FindObject (h1->GetName ());

					if (key2) {
						cl++;
						countsignal++;
						TH1D *h2;

						h2 = (TH1D*) key2->ReadObj ();
			

						h2->SetLineWidth(3);
      		        			if ( string::npos == nn.find("CutFlowUnW") &&  string::npos == nn.find("1D_")) 
						h2->GetXaxis()->SetRange(hh[1]->FindFirstBinAbove(0),hh[1]->FindLastBinAbove(MaxEventsBin));	
						//cout << lumiweights[cl-1]<<endl;

						ModifyHist (h2, cl,Lumi,lumiweights[cl-1],titles[cl-1],false,xsecc);
						h2->SetStats(0);
						//if (std::string::npos == fname.find(signn))
						hh[cl] = h2;
						//if (std::string::npos != fname.find(signn))
						//	hsignal[cl] = h2;

						if (cl==2){
							allbkg  = (TH1D*) h2->Clone();
							allbkg->Reset();
							htt  = (TH1D*) h2->Clone();
							httx  = (TH1D*) h2->Clone();
							hstop  = (TH1D*) h2->Clone();
							hwj  = (TH1D*) h2->Clone();
							hdyj  = (TH1D*) h2->Clone();
							hztt  = (TH1D*) h2->Clone();
							hrare  = (TH1D*) h2->Clone();
							hdib  = (TH1D*) h2->Clone();
							hww  = (TH1D*) h2->Clone();
							hqcd  = (TH1D*) h2->Clone();

							htt->Reset();
							httx->Reset();
							hstop->Reset();
							hdyj->Reset();
							hztt->Reset();
							hwj->Reset();
							hrare->Reset();
							hdib->Reset();
							hww->Reset();
							hqcd->Reset();


						}

						//				cout<< " for "<<fname<<"  "<<count<<" file, there xsec is "<<xsecc<<" SuOfWeights "<<nGenn<<" Lumi "<<Lumi<<" lumiweight "<<lumiweights[cl-1]<<" weight "<<float(xsecc*Lumi/nGenn)<<"  norm "<<normm<<" cl "<<cl<<"  "<<h2->GetSumOfWeights()<<endl;
						//cout<< " for "<<fname<<" file, there xsec is "<<xsecc<<" SuOfWeights "<<"  "<<h2->GetSumOfWeights()<<" Lumi "<<Lumi<<" lumiweight "<<lumiweights[cl-1]<<" weight "<<float(xsecc*Lumi/nGenn)<<endl;
						//if (std::string::npos == fname.find(sn) )

						if (std::string::npos != fname.find(sn) || std::string::npos != fname.find(cc1) || std::string::npos != fname.find(sdata) ||  std::string::npos != fname.find(sdata2)  )
							flagg=true;


						string  title_ = fname;
							//cout<<"  "<<fname<<endl;


			//////////TT
			if (std::string::npos != title_.find("TT_TuneCUETP8M2T4_13TeV-powheg-pythia8") || std::string::npos != title_.find("TTPow")) { col= mycolortt;htt->Add(h2); htt->SetLineColor(col) ;}
			///////////Fakes
			if (std::string::npos != title_.find("Fakes") || std::string::npos != title_.find("QCD"))  {col= mycolorqcd; hqcd->Add(h2); hqcd->SetLineColor(col); }
			///////////////Zll
			if ( (std::string::npos != title_.find("DYJets") || std::string::npos != title_.find("DY1Jets") || 
			std::string::npos != title_.find("DY2Jets") || std::string::npos != title_.find("DY3Jets") || std::string::npos != title_.find("DY4Jets") )&& std::string::npos == title_.find("isZTT") )  {col= mycolordyj;hdyj->Add(h2); hdyj->SetLineColor(col);}
			/////////////////ZTT
			if (std::string::npos != title_.find("isZTT") ){col= mycolorztt;hztt->Add(h2); hztt->SetLineColor(col);} 
	/////////////SinglTop
			if (std::string::npos != title_.find("ST_") || std::string::npos != title_.find("channel") )  {col= mycolorst; hstop->Add(h2);hstop->SetLineColor(col);}
			////WW
			if ( std::string::npos != title_.find("WWTo")  ) {col=mycolorww; hww->Add(h2); hww->SetLineColor(col);}

			////////////////////Rest now has the colorvv
			if (std::string::npos != title_.find("wJets")|| std::string::npos != title_.find("WJetsToLNu") || std::string::npos != title_.find("W1JetsToLNu") || std::string::npos != title_.find("W2JetsToLNu") 
			|| std::string::npos != title_.find("W3JetsToLNu") || std::string::npos != title_.find("W4JetsToLNu"))  { col=mycolorvv ; hwj->Add(h2); hwj->SetLineColor(col);}
			
			if ( (std::string::npos != title_.find("WW") || std::string::npos != title_.find("ZZ") ||  std::string::npos != title_.find("WZ") || std::string::npos != title_.find("WG") || std::string::npos != title_.find("ZG") ) && std::string::npos == title_.find("WWTo") ) {col=mycolorvv; hdib->Add(h2); hdib->SetLineColor(col);}


			if ( std::string::npos != title_.find("TTW") || std::string::npos != title_.find("TTZ") || std::string::npos != title_.find("TG") || std::string::npos != title_.find("tG") || std::string::npos != title_.find("TTG") || std::string::npos != title_.find("ttW") || std::string::npos != title_.find("tZ") || std::string::npos != title_.find("TTT_") ) {col=mycolorvv ;httx->Add(h2);   httx->SetLineColor(col);}


						if (!flagg)
						{
							//cout<<" will add histogram "<<h2->GetName()<< " for "<<titles[cl-1]<<"  "<<fname<<"  cl  "<<cl<<endl;
							//cout << h2->GetSumOfWeights()<< endl;
							hs->Add(h2);
							allbkg->Add(h2);
						}

				}

				nextsource = (TFile *) sourcelist->After (nextsource);
			}				// while ( nextsource )
		}

		if (obj->IsA ()->InheritsFrom ("TH1")) {
			///////// TPie

			char savename[50];
			string nan_ = obj->GetName();
			int nvals = 5;
			if (std::string::npos != nan_.find("nJet_") ){


			Float_t vals[]={hdib->GetSumOfWeights()+httx->GetSumOfWeights()+hwj->GetSumOfWeights(), ,hqcd->GetSumOfWeights(), hww->GetSumOfWeights(), htt->GetSumOfWeights()+hstop->GetSumOfWeights(),  hdyj->GetSumOfWeights()+hztt->GetSumOfWeights()};
			 // cout<<" Found it here  "<<nan_<<"   "<<key->GetName()<<endl;
			//Int_t nvals = sizeof(counter)/sizeof(counter[0]);
			//cout<<"  vals ============== "<<vals[0]<<"  "<<vals[1]<<"  "<<vals[2]<<"  "<<vals[3]<<"  "<<vals[4]<<"   "<<endl;
			//cout<<" VV + TTX  "<<hdib->GetSumOfWeights()+httx->GetSumOfWeights()+hwj->GetSumOfWeights()<<" TT+ST "<<htt->GetSumOfWeights()+hstop->GetSumOfWeights()<<" WJ "<<hwj->GetSumOfWeights()<<" DY "<<hdyj->GetSumOfWeights()<<" TTX "<<httx->GetSumOfWeights()<<" QCD "<<hqcd->GetSumOfWeights()<<endl;

			Int_t colors[] = { hdib->GetLineColor(), hqcd->GetLineColor(), hww->GetLineColor(),htt->GetLineColor(),hdyj->GetLineColor()};
			TPie *pie3 = new TPie("pie3","Pie with tangential labels",nvals,vals,colors);

			TCanvas *cpie = new TCanvas("cpie","TPie test",1200,1200);
			cpie->cd();
			pie3->SetRadius(.2);
			pie3->SetLabelsOffset(.015);
			pie3->SetLabelFormat("%txt - (%perc)");
			pie3->SetTextAngle(0.45);
			pie3->SetTextSize(0.04);
			pie3->SetFractionFormat(".0g");


		
			for (int j=0;j<nvals;j++) cout<<" VALUES         ======================================="<<  vals[j]<<endl;

			char lumitag[100];
			sprintf(lumitag,"Data #int L = %.3g fb^{-1}",Lumi/1000);
			//pie3->GetSlice(0)->SetTitle(lumitag);
                        pie3->SetLabelFormat("#splitline{%txt}{%perc}");
                        pie3->SetLabelFormat("%txt-%perc");
			pie3->GetSlice(0)->SetTitle("Rest");
			pie3->GetSlice(1)->SetTitle("mis-iD #tau");
			pie3->GetSlice(2)->SetTitle("WW");
			pie3->GetSlice(3)->SetTitle("t#bar{t}+SingleT");
			pie3->GetSlice(4)->SetTitle("DYJets");
			//pie3->SetEntryRadiusOffset(2,-.05);
			//pie3->SetLabelsOffset(-0);
			//pie3->SetLabelsSize(0.05);
			pie3->Draw("nol <");
			pie3->Draw("rsc");
			//pie3->Draw("3d");
			nan_.erase(0,4);
			sprintf(savename,"TauWDD/Pie_mutau.pdf",nan_);
			cpie->Print(savename,".pdf");


			}
			//////////////////////////// Pie

			
			target->cd ();
			//legend_c1->SetNColumns(3);

			TPad *pad1 ;//= new TPad("pad1","pad1",0,0.2,1,1);//ratio

			TPad *pad2 = new TPad("pad2","pad2",0, 0, 1,0.2);

			if (!drawData) 
			pad1= new TPad("pad1","pad1",0,0,1,1);
			if (drawData)  
			pad1= new TPad("pad1","pad1",0,0.2,1,1);

			pad1->SetBottomMargin(0.0);
			if (!drawData) pad1->SetBottomMargin(0.1);
			pad1->SetRightMargin(0.05);
			//pad1->SetGridy();

			pad2->SetTopMargin(0.01);
			pad2->SetBottomMargin(0.38);
			pad2->SetRightMargin(0.05);


			//hs->Draw("nostack");
			c1->cd();


			c1->Clear();

			pad1->SetLogy();
			 //if (std::string::npos != nn.find("Mt2lester")  || std::string::npos != nn.find("MET_") || std::string::npos != nn.find("HighDzeta_") ||  std::string::npos != nn.find("MiddleDzeta_") ||  std::string::npos != nn.find("LowDzeta_"))	{ pad1->SetLogx(); pad2->SetLogx();}
			//pad1->SetLogx();
			//pad1->SetGridx();
			pad1->Draw();
	if (drawData)	pad2->Draw();

			pad2->SetGridy(1);

			pad1->cd();
			pad1->Clear();


			TH1D *hsum = ((TH1D*)(hs->GetStack()->Last())); // the "SUM"
      		        if ( string::npos == nn.find("CutFlowUnW") && string::npos == nn.find("1D_")) 
			hsum->GetXaxis()->SetRange(hh[1]->FindFirstBinAbove(0),hh[1]->FindLastBinAbove(MaxEventsBin));	
			OverFlow(hsum,hh[1]->FindLastBinAbove(MaxEventsBin));
			//cout<<" events................. "<<hsum->GetSumOfWeights()<<"  "<<hsum->GetName()<<"  "<<hsum->GetTitle()<<" bkg "<<allbkg->GetSumOfWeights()<<endl;

			char namee[100];
			sprintf(namee,"%s",key->GetName ());
			char nnn[100];

			string titlee= hsum->GetTitle();
			hsum->GetYaxis()->SetTitleOffset(1);
			hsum->GetYaxis()->SetTitleSize(0.045);

			//hh[1]->GetXaxis()->SetRange(hh[1]->FindFirstBinAbove(0),hh[titles.size()-1]->FindLastBinAbove(MaxEventsBin));
      		        if ( string::npos == nn.find("CutFlowUnW") && string::npos == nn.find("1D_")) 
			hh[1]->GetXaxis()->SetRange(hh[1]->FindFirstBinAbove(0),hh[1]->FindLastBinAbove(MaxEventsBin));
			OverFlow(hh[1],hh[1]->FindLastBinAbove(MaxEventsBin));
			//hh[1]->GetXaxis()->SetRange(hh[1]->FindFirstBinAbove(0),hh[1]->GetNbinsX());
			if (cl=1){
				TH1F* v1 = new TH1F("", "", 1, 0, 1);v1->SetLineWidth(4);
				TH1F* v2 = new TH1F("", "", 1, 0, 1);v2->SetLineWidth(4);
				TH1F* v3 = new TH1F("", "", 1, 0, 1);v3->SetLineWidth(4);
				TH1F* v4 = new TH1F("", "", 1, 0, 1);v4->SetLineWidth(4);
				TH1F* v5 = new TH1F("", "", 1, 0, 1);v5->SetLineWidth(4);
				TH1F* v6 = new TH1F("", "", 1, 0, 1);v6->SetLineWidth(4);
				TH1F* v7 = new TH1F("", "", 1, 0, 1);v7->SetLineWidth(4);
				TH1F* v8 = new TH1F("", "", 1, 0, 1);v8->SetLineWidth(4);
				TH1F* v9 = new TH1F("", "", 1, 0, 1);v9->SetLineWidth(4);
				TH1F* v10 = new TH1F("", "", 1, 0, 1);v10->SetLineWidth(4);
				v9->SetMarkerSize(0.4);
				v9->SetMarkerColor(49);
				v1->SetMarkerSize(0.4);

				char lumitag[100];
				sprintf(lumitag,"Data #int L = %.3g fb^{-1}",Lumi/1000);
				//v1->SetLineColor(kBlack);legend_c1->AddEntry(hh[1], lumitag, "LEP");
				v1->SetLineColor(kBlack);
if (drawData)			legend_c1->AddEntry(hh[1], "Data", "LEP");
				char lab_[100];
if (SO_){
				legend_c1->AddEntry(hh[2], "#tilde{#tau}_{L}(100,1)", "LEP");
				legend_c1->AddEntry(hh[3], "#tilde{#tau}_{L}(90,1)", "LEP");
				legend_c1->AddEntry(hh[4], "#tilde{#tau}_{L}(150,1)", "LEP");
				legend_c1->AddEntry(hh[5], "#tilde{#tau}_{L}(200,1)", "LEP");

//				legend_c1->AddEntry(hh[5], "C1N2(300,100)", "LEP");
//				legend_c1->AddEntry(hh[6], "C1N2(400,125)", "LEP");
}

				v1->SetLineColor(mycolortt);v1->SetFillColor(mycolortt);legend_c1->AddEntry(v1, "TTJets", "f");
				v3->SetLineColor(mycolorst);v3->SetFillColor(mycolorst);legend_c1->AddEntry(v3, "singleTop", "f");
				//v2->SetLineColor(mycolorttx);v2->SetFillColor(mycolorvv);legend_c1->AddEntry(v2, "TTX/TG", "f");
				//v4->SetLineColor(mycolorwjet);v4->SetFillColor(mycolorwjet);legend_c1->AddEntry(v4, "WJets", "f");
				v7->SetLineColor(mycolorztt);v7->SetFillColor(mycolorztt);legend_c1->AddEntry(v7, "DY#rightarrow#tau#tau", "f");
				v5->SetLineColor(mycolorqcd);v5->SetFillColor(mycolorqcd);legend_c1->AddEntry(v5, "Jet#rightarrow#tau-Fakes", "f");
				v6->SetLineColor(mycolordyj);v6->SetFillColor(mycolordyj);legend_c1->AddEntry(v6, "DY#rightarrow#mu#mu/ee", "f");
				v8->SetLineColor(mycolorvv);v8->SetFillColor(mycolorvv);legend_c1->AddEntry(v8, "Rest", "f");
				v9->SetLineColor(mycolorww);v9->SetFillColor(mycolorww);legend_c1->AddEntry(v9, "WW", "f");


				float chi2 =0;
				float pValue =0;


				for (int iB=1; iB<=hsum->GetNbinsX(); ++iB) 
				{
					float error = 0; 
					float content = 0;
					content = allbkg->GetBinContent(iB);	
					error = allbkg->GetBinError(iB);		
					
					error = error + content*0.15;
					allbkg->SetBinError(iB,error);
					
				}
					
				chi2 = hh[1]->Chi2Test(allbkg,"WW CHI2/NDF");
				pValue = hh[1]->Chi2Test(allbkg,"WW");



				
				TString Schi2;
				Schi2->Form("%.2f",chi2);
				TString SpValue;
				SpValue->Form("%.2f",pValue);
				
				legend_c1->AddEntry((TObject*)0, "#chi^{2}/ndf "+Schi2, "");
				legend_c1->AddEntry((TObject*)0, "p-value "+SpValue, "");


				sprintf(lab_,"bkg,(%.2f)",hsum->GetSumOfWeights());
				sprintf(lab_,"data,(%.2f)",hh[1]->GetSumOfWeights());
				v10->SetLineColor(kBlack);legend_c2->AddEntry(hh[1], lab_, "l");
			}
			//	cout<<"  Total event from bkg  "<<counter[0]<<"   "<<hh[1]->GetSumOfWeights()<<endl;
			hsum->SetMinimum(0.05);
			sprintf(nnn,"Entries  / %d GeV",hh[1]->GetBinWidth(2));
			
			if (std::string::npos != nn.find("BDTmutau_")) sprintf(nnn,"Entries");

			hsum->GetYaxis()->SetTitle(nnn);
			if ( std::string::npos != titlee.find("phi") || std::string::npos != titlee.find("Phi") ||  std::string::npos != titlee.find("eta") ||  std::string::npos != titlee.find("nJ") || std::string::npos != titlee.find("nB") ||  std::string::npos != titlee.find("nEl") || std::string::npos != titlee.find("nM") || std::string::npos != titlee.find("nT") || std::string::npos != titlee.find("dxy") || std::string::npos != titlee.find("_dz")) hsum->GetYaxis()->SetTitle("Entries");

			if (norm_) {hsum->Scale(1/hsum->Integral()); hsum->SetFillColor(0);hsum->SetLineColor(kRed);}

			hsum->Draw("hist");
//cout<<			hsum->GetEntries()<<"  "<<hsum->GetSumOfWeights()<<endl;
			if (!norm_)
				hs->Draw("same hist");
				
				
				
if (drawData)	


{			//if (std::string::npos != nn.find("BDTmutau_")) hh[1]->GetXaxis()->SetRange(0,17);
			hh[1]->Draw("same ep hist");
		}
			hh[1]->SetMarkerStyle(20);
			hh[1]->SetFillColor(0);
			if (norm_) hh[1]->Scale(1/hh[1]->Integral());
if (SO_){
			for (int ij=2;ij<6;ij++){
      		        	if ( string::npos == nn.find("CutFlowUnW") && string::npos == nn.find("1D_")) 
				hh[ij]->GetXaxis()->SetRange(hh[1]->FindFirstBinAbove(0),hh[1]->FindLastBinAbove(MaxEventsBin));
				OverFlow(hh[ij],hh[1]->FindLastBinAbove(MaxEventsBin));
				if (norm_) { hh[ij]->Scale(1/hh[ij]->Integral());hh[ij]->SetLineColor(kBlue+ij);}
				hh[ij]->SetLineStyle(2);
				hh[ij]->SetLineWidth(3);
				hh[ij]->SetFillColor(0);
				hh[ij]->Draw("same e1 hist");
			}
}

			string Title = hh[1]->GetName();

			FixBoxPadding(pad1, legend_c1, hsum,0.1);
    			legend_c1->Draw("sames");
    			FixOverlay();
			c1->SetFillColor(0);
			c1->SetBorderMode(0);
			c1->SetBorderSize(0);
			c1->SetFrameBorderMode(0);
			c1->SetBorderSize(0);
			pad1->SetFrameLineColor(0);;


			pad2->cd();
			ratio = (TH1D*) hh[1]->Clone();
			string nT = hh[1]->GetName();
			if (std::string::npos != nT.find("CutFlowUnW"))
			ratio->Sumw2();
			ratio->SetStats(000000);
			ratio->Divide(hsum);
			ratio->SetMarkerStyle(20);
			ratio->SetMaximum( 2.2 );
			ratio->SetMinimum(0.5);
			ratio->GetXaxis()->SetTitle(hh[1]->GetXaxis()->GetTitle());
			ratio->GetYaxis()->SetTitle("");
			ratio->SetTitleSize(0);
			ratio->SetMarkerSize(0.08);
			ratio->SetLineStyle(1);
                        ratio->SetMarkerSize(1.2);
			ratio->SetMarkerColor(kBlack);
			ratio->SetLineColor(kBlack);
			ratio->SetLineWidth(1);
			ratio->GetYaxis()->SetNdivisions(5);

			ratio->GetXaxis()->SetNdivisions(545);
			ratio->GetXaxis()->SetLabelSize(0.15);
			ratio->GetXaxis()->SetTitleOffset(1.);
			ratio->GetXaxis()->SetTitleSize(0.17);

			ratio->GetYaxis()->SetLabelSize(0.1);
			ratio->GetYaxis()->SetLabelSize(0.15);
			ratio->GetYaxis()->SetTitleSize(0.15);
			ratio->GetYaxis()->SetTitleOffset(0.25);
			ratio->GetYaxis()->SetTitle("Obs / MC");


			ratio->SetTitleFont(62);
			//ratio->GetXaxis()->SetRange(ratio->FindFirstBinAbove(0),ratio->FindLastBinAbove(MaxEventsBin));
      		        if ( string::npos == nn.find("CutFlowUnW") && string::npos == nn.find("1D_")) 
			ratio->GetXaxis()->SetRange(hh[1]->FindFirstBinAbove(0),hh[1]->FindLastBinAbove(MaxEventsBin));

			//ratio->GetXaxis()->SetRange(hh[1]->FindFirstBinAbove(0),hh[1]->GetNbinsX());

			ratio->GetXaxis()->SetNdivisions(510);
			ratio->SetTitle("");
			TAxis *axis = hh[1]->GetXaxis();
			float halfbin = axis->GetBinWidth(hh[1]->GetBin(1))*0.5;
			//cout<<" halfbin  "<<halfbin
			//  TLine *l=new TLine(axis->GetBinLowEdge(hh[1]->FindFirstBinAbove(0.)),1,axis->GetBinWidth(hh[1]->GetBin(1))*hh[1]->GetNbinsX() - halfbin,1);
      		        //if ( string::npos == nn.find("CutFlowUnW")) 
			TLine *l=new TLine(axis->GetBinLowEdge(hh[1]->FindFirstBinAbove(0.)),1,axis->GetBinLowEdge(hh[1]->FindLastBinAbove(MaxEventsBin)+1),1);

			//   cout<<"  ================= > "<< hh[1]->GetBinContent(hh[1]->FindLastBinAbove(0.0))<<"    "<<hh[1]->FindLastBinAbove(float(0.0))<<endl;
			l->SetLineColor(kRed);
			l->SetLineWidth(2);
			bkgRatioErrH  = (TH1D*) ratio->Clone();
			DataHErrP  = (TH1D*) hh[1]->Clone();
			DataHErrM  = (TH1D*) hh[1]->Clone();
			//bkgRatioErrH =(TH1D*) bkgRatioErrH->Clone("hh[1]");
			//bkgRatioErrH->Reset();
			bkgRatioErrH->SetLineColor(kBlack);
			bkgRatioErrH->SetFillStyle(3002);
			bkgRatioErrH->SetFillColor(1);
			bkgRatioErrH->SetMarkerStyle(20);
			bkgRatioErrH->SetMarkerSize(0);
			bkgRatioErrH->SetLineColor(1);
			bkgRatioErrH->SetLineWidth(2);

			//hh[1]->Sumw2();
		//	allbkg->Sumw2();
			for (int iB=1; iB<=hsum->GetNbinsX(); ++iB) {
				float datX = hh[1]->GetBinContent(iB);
				float datE = hh[1]->GetBinError(iB);
				float bkgX = hsum->GetBinContent(iB);
				float bkgE = hsum->GetBinError(iB);
				//if (bkgX>0)
				//    cout<<" bin  "<<iB<<" error "<<bkgE<<" Content  "<<bkgX<<" rel "<<float(bkgE/bkgX)<<" data content  "<<datX<<" data error "<<datE<<endl;
				float datRatioX = datX/bkgX;
				float datRatioE = datE/bkgX;
				//float bkgErr = float(bkgE / bkgX);
				float bkgErr = float((bkgE+0.15*bkgX) / bkgX);
				// blind BDT signal region
				//if (std::string::npos != nn.find("BDTmutau_") && iB>17) {datE = 0;bkgErr=0;ratio->SetBinContent(iB,0);}
				
				bkgRatioErrH->SetBinContent(iB,1);
				bkgRatioErrH->SetBinError(iB,bkgErr);
				ratio->SetBinError(iB,datE/bkgX);
				DataHErrP->SetBinContent(iB,datX);
				DataHErrP->SetBinError(iB,datE);
				DataHErrM->SetBinContent(iB,bkgX);
				DataHErrM->SetBinError(iB,bkgE);

			}
if (std::string::npos != nn.find("BDTmutau_Stau100")) 
	{	
	cout << "  hdib  "<< hdib->GetSumOfWeights()<<"  httx  "<< httx->GetSumOfWeights()<<"  hwj  "<< hwj->GetSumOfWeights()<<
	"  hqcd  "<<hqcd->GetSumOfWeights()<<"  hww  "<<
	hww->GetSumOfWeights()<<"  htt  "<< htt->GetSumOfWeights()<<"  hstop  "<<hstop->GetSumOfWeights()<<"  hdyj  "<<
	  hdyj->GetSumOfWeights()<<"  hztt  "<<hztt->GetSumOfWeights()<<"  data  "<<hh[1]->GetSumOfWeights()<<endl;
	
	}
                       if (drawData) { ratio->Draw("ep ");}

			bkgRatioErrH->Draw("e2 same");
			l->Draw("same");
/*
			 pad1->cd();
			//pad1->Clear();
			DataHErrP->Sumw2();
			DataHErrP->SetFillColor(1);
			DataHErrP->SetLineColor(1);
			DataHErrP->SetFillStyle(2013);
			DataHErrP->SetMarkerStyle(21);
			DataHErrP->SetMarkerSize(0);

			DataHErrM->SetLineColor(2);
			DataHErrM->SetFillColor(2);
			DataHErrM->SetFillStyle(3013);
			DataHErrM->SetMarkerStyle(22);
			DataHErrM->SetMarkerSize(0);

			DataHErrP->Draw("e2 same");
			DataHErrM->Draw("e2 same");
*/
			//hh[1]->Draw("e1same");
			
			c1->SetTitle(namee);
			gPad->RedrawAxis();
			gPad->Modified();
			gPad->Update();
			//tex = new TLatex(6.471403,1.803921e+08,"Some title here");
			//tex->SetLineWidth(2);
			//tex->Draw();

			c1->cd();
                	CMS_lumi( c1, 4, 1 );
			c1->Modified();
			c1->cd();
			c1->Update();


			c1->Write (namee);


			char f[100];char ff[100];
	if(!WDD){
			if (SO_ && !QCDMC)sprintf(f,"Tau/%sSO.pdf",namee);
			if (SO_ && QCDMC)sprintf(f,"Tau/%s_QCDMC_SO.pdf",namee);
			if(!SO_ && !QCDMC) sprintf(f,"Tau/%s.pdf",namee);
			if(!SO_ && QCDMC) sprintf(f,"Tau/%s_QCDMC.pdf",namee);
		}
		if (WDD){
			if (SO_ && !QCDMC)sprintf(f,"TauWDD/%sSO.pdf",namee);
			if (SO_ && QCDMC)sprintf(f,"TauWDD/%s_QCDMC_SO.pdf",namee);
			if(!SO_ && !QCDMC) sprintf(f,"TauWDD/%s.pdf",namee);
			if(!SO_ && QCDMC) sprintf(f,"TauWDD/%s_QCDMC.pdf",namee);

		}
			c1->Print (f,"pdf");


		}

		} 			// while ( ( TKey *key = (TKey*)nextkey() ) )
		delete c1;
		// save modifications to target file
		target->SaveSelf (kTRUE);
		TH1::AddDirectory (status);
		cout << "	" << "========================================================" << endl;
		cout<< " Ended SuperImpose of files.... " <<endl;



	}

void ModifyHist (TH1D* &h, int cl_ ,float & lumi,float & weight,string & title_, bool norm_=false,float XSec=1.)
		{


			int nbins=h->GetNbinsX();
			int nn=1;
			string title1,title2;
			title1= h->GetName();
			//h->Rebin(4);

			h->SetMinimum(0.0005);
			h->GetXaxis()->SetNdivisions(512);
			string titlee=h->GetName();
			int col=1;//kOrange;
			

			if (std::string::npos != title_.find("Data") || std::string::npos != title_.find("Single") || std::string::npos != title_.find("MuonEG")  )  {

				col=kBlack ;
				h->SetLineStyle(1);
				h->SetMarkerStyle(20);
				h->SetMarkerSize(1.2);
				h->SetMarkerColor(col);
				h->SetLineColor(col);
			}


			if ( std::string::npos != title_.find("tau") || std::string::npos != title_.find("C1") || std::string::npos != title_.find("SMS")){

				//col=kBlue-10 + cl_;
				//if (cl_>9) col=30+cl_;
				col=cl_+1;
				if (cl_==2) col=kBlue+1;
				if (cl_==3) col=kRed+1;
				if (cl_==4) col=kGreen+2;
				h->SetLineStyle(2);
				h->SetMarkerStyle(20);
				h->SetMarkerSize(0.4);
				h->SetMarkerColor(col);
				h->SetFillColor(0);
				h->SetLineWidth(3);
			}


			int nb = h->GetNbinsX();
			float over_ = h->GetBinContent(nb+1);
			float contlast = h->GetBinContent(nb);

			h->Scale(weight);



			if (std::string::npos != title_.find("TT_TuneCUETP8M2T4_13TeV-powheg-pythia8") || std::string::npos != title_.find("TTPow")) { col= mycolortt; }

			if (std::string::npos != title_.find("Fakes") || std::string::npos != title_.find("QCD"))  {col= mycolorqcd; }

			if ((std::string::npos != title_.find("DYJets") || std::string::npos != title_.find("DY1Jets") || 
			std::string::npos != title_.find("DY2Jets") || std::string::npos != title_.find("DY3Jets") || std::string::npos != title_.find("DY4Jets")) &&std::string::npos == title_.find("isZTT"))  {col= mycolordyj;}

			if (std::string::npos != title_.find("isZTT") ){col= mycolorztt;}

			if (std::string::npos != title_.find("ST_") || std::string::npos != title_.find("channel") )  {col= mycolorst; }

			if ( std::string::npos != title_.find("WWTo") ) {col=mycolorww;}


			if (std::string::npos != title_.find("wJets")|| std::string::npos != title_.find("WJetsToLNu") || std::string::npos != title_.find("W1JetsToLNu") || std::string::npos != title_.find("W2JetsToLNu") 
			|| std::string::npos != title_.find("W3JetsToLNu") || std::string::npos != title_.find("W4JetsToLNu"))  { col=mycolorvv ; }

			if ( ( std::string::npos != title_.find("WW") || std::string::npos != title_.find("ZZ") ||  std::string::npos != title_.find("WZ") || std::string::npos != title_.find("WG") || std::string::npos != title_.find("ZG") ) &&  std::string::npos == title_.find("WWTo")) {col=mycolorvv;}

			if ( std::string::npos != title_.find("TTW") || std::string::npos != title_.find("TTZ") || std::string::npos != title_.find("tZ") || std::string::npos != title_.find("TG") || std::string::npos != title_.find("tG")  || std::string::npos != title_.find("ttW") || std::string::npos != title_.find("TTT_") ) {col=mycolorvv ;}


			char histotitle[100];
		//	string htitle;
		//	if (std::string::npos != title1.find("met"))  h->GetXaxis()->SetTitle("GeV");
		//	if (std::string::npos != title1.find("ST"))  h->GetXaxis()->SetTitle("GeV/c^{2}`");
		//	if (std::string::npos != title1.find("HT"))  h->GetXaxis()->SetTitle("GeV/c^{2}");
		//	if (std::string::npos != title1.find("Jetp"))  h->GetXaxis()->SetTitle("GeV/c^{2}");
		//	if (std::string::npos != title1.find("Phi"))  h->GetXaxis()->SetTitle("rad");
		//	if (std::string::npos != title1.find("phi"))  h->GetXaxis()->SetTitle("rad");
		//	if (std::string::npos != title1.find("pT"))  h->GetXaxis()->SetTitle("GeV/c^{2}");
			if (std::string::npos != title1.find("nB"))  h->GetXaxis()->SetTitle("nBJets");
			if (std::string::npos != title1.find("nJ"))  h->GetXaxis()->SetTitle("nJets");
			if (std::string::npos != title1.find("nL"))  h->GetXaxis()->SetTitle("nLept");
			if (std::string::npos != title1.find("nmu"))  h->GetXaxis()->SetTitle("nMu");
			if (std::string::npos != title1.find("nEl"))  h->GetXaxis()->SetTitle("nEl");
			if (std::string::npos != title1.find("nMu"))  h->GetXaxis()->SetTitle("nMu");
			if (std::string::npos != title1.find("nTau"))  h->GetXaxis()->SetTitle("nTau");
			if (std::string::npos != title1.find("dz"))  h->GetXaxis()->SetTitle("d_{z} [cm]");
			if (std::string::npos != title1.find("dxy"))  h->GetXaxis()->SetTitle("d_{xy} [cm]");
			if (std::string::npos != title1.find("el_dxy"))  h->GetXaxis()->SetTitle("d_{xy}(e) [cm]");
			if (std::string::npos != title1.find("mu_dxy"))  h->GetXaxis()->SetTitle("d_{xy}(#mu) [cm]");
			if (std::string::npos != title1.find("tau_dxy"))  h->GetXaxis()->SetTitle("d_{xy}(#tau) [cm]");
			if (std::string::npos != title1.find("mu_dz"))  h->GetXaxis()->SetTitle("d_{z}(#mu) [cm]");
			if (std::string::npos != title1.find("el_dz"))  h->GetXaxis()->SetTitle("d_{z}(e) [cm]");
			if (std::string::npos != title1.find("tau_dz"))  h->GetXaxis()->SetTitle("d_{z}(#tau) [cm]");
			if (std::string::npos != title1.find("IsoTau"))  h->GetXaxis()->SetTitle("RelIso(#tau)");
			if (std::string::npos != title1.find("IsoMu"))  h->GetXaxis()->SetTitle("RelIso(#mu)");
			if (std::string::npos != title1.find("IsoEl"))  h->GetXaxis()->SetTitle("RelIso(e)");
			if (std::string::npos != title1.find("Rht"))  h->GetXaxis()->SetTitle("R(H_{T})");

			if (std::string::npos != title1.find("dEtaJ0Tau"))  h->GetXaxis()->SetTitle("#Delta#eta(J_{0},#tau)");
			if (std::string::npos != title1.find("dEtaJ0Mu"))  h->GetXaxis()->SetTitle("#Delta#eta(J_{0},#mu)");
			if (std::string::npos != title1.find("dEtaJ0El"))  h->GetXaxis()->SetTitle("#Delta#eta(J_{0},e)");
			if (std::string::npos != title1.find("dEtaDil"))  h->GetXaxis()->SetTitle("#Delta#eta(Dil)");
			if (std::string::npos != title1.find("dPhiDil"))  h->GetXaxis()->SetTitle("#Delta#Phi(Dil) [rad]");


			if (std::string::npos != title1.find("HT"))  h->GetXaxis()->SetTitle("H_{T} [GeV]");
			if (std::string::npos != title1.find("HText"))  h->GetXaxis()->SetTitle("H_{Text} [GeV]");
			if (std::string::npos != title1.find("MCT"))  h->GetXaxis()->SetTitle("M_{CT} [GeV]");
//			if (std::string::npos != title1.find("MCTb"))  h->GetXaxis()->SetTitle("M_{CTb} [GeV]");
			if (std::string::npos != title1.find("MCTx"))  h->GetXaxis()->SetTitle("M_{CTx} [GeV]");
			if (std::string::npos != title1.find("MCTy"))  h->GetXaxis()->SetTitle("M_{CTy} [GeV]");
			if (std::string::npos != title1.find("MET"))  h->GetXaxis()->SetTitle("#it{p}_{T}^{miss} [GeV]");
			if (std::string::npos != title1.find("MT"))  h->GetXaxis()->SetTitle("M_{T} [GeV]");
			if (std::string::npos != title1.find("MTmu"))  h->GetXaxis()->SetTitle("M_{T}(#mu) [GeV]");
			if (std::string::npos != title1.find("MTel"))  h->GetXaxis()->SetTitle("M_{T}(el) [GeV]");
			if (std::string::npos != title1.find("MTtau"))  h->GetXaxis()->SetTitle("M_{T}(#tau) [GeV]");

			if (std::string::npos != title1.find("MTmutau"))  h->GetXaxis()->SetTitle("M_{T}(#mu,#tau) [GeV]");
			if (std::string::npos != title1.find("MTeltau"))  h->GetXaxis()->SetTitle("M_{T}(e,#tau) [GeV]");
			if (std::string::npos != title1.find("MTmuel"))  h->GetXaxis()->SetTitle("M_{T}(#mu,e) [GeV]");


			if (std::string::npos != title1.find("MTtot"))  h->GetXaxis()->SetTitle("M_{Ttot} [GeV]");
			if (std::string::npos != title1.find("MTsum"))  h->GetXaxis()->SetTitle("M_{Tsum} [GeV]");
			if (std::string::npos != title1.find("Mt2lester"))  h->GetXaxis()->SetTitle("M_{T2} [GeV]");
			if (std::string::npos != title1.find("Mt2mu"))  h->GetXaxis()->SetTitle("M_{T2} [GeV]");
			if (std::string::npos != title1.find("Mt2el"))  h->GetXaxis()->SetTitle("M_{T2} [GeV]");

			if (std::string::npos != title1.find("MET_"))  h->GetXaxis()->SetTitle("#it{p}_{T}^{miss} [GeV]");
			if (std::string::npos != title1.find("dPhiMETLep"))  h->GetXaxis()->SetTitle("#Delta#Phi(#it{p}_{T}^{miss},lep) [rad]");
			if (std::string::npos != title1.find("dPhiMETMu"))  h->GetXaxis()->SetTitle("#Delta#Phi(#it{p}_{T}^{miss},#mu) [rad]");
			if (std::string::npos != title1.find("dPhiMETEl"))  h->GetXaxis()->SetTitle("#Delta#Phi(#it{p}_{T}^{miss},e) [rad]");
			if (std::string::npos != title1.find("dPhiMuMET"))  h->GetXaxis()->SetTitle("#Delta#Phi(#it{p}_{T}^{miss},#mu) [rad]");
			if (std::string::npos != title1.find("dPhiElMET"))  h->GetXaxis()->SetTitle("#Delta#Phi(#it{p}_{T}^{miss},e) [rad]");
			if (std::string::npos != title1.find("dPhiTauMET"))  h->GetXaxis()->SetTitle("#Delta#Phi(#it{p}_{T}^{miss},#tau) [rad]");
			if (std::string::npos != title1.find("dPhiJ0MET"))  h->GetXaxis()->SetTitle("#Delta#Phi(J_{0},#it{p}_{T}^{miss}) [rad]");
			if (std::string::npos != title1.find("dPhiJMET"))  h->GetXaxis()->SetTitle("#Delta#Phi(J,#it{p}_{T}^{miss}) [rad]");
			if (std::string::npos != title1.find("dPhiJ0Mu"))  h->GetXaxis()->SetTitle("#Delta#Phi(J_{0},#mu) [rad]");
			if (std::string::npos != title1.find("dPhiJ0El"))  h->GetXaxis()->SetTitle("#Delta#Phi(J_{0},e) [rad]");
			if (std::string::npos != title1.find("dPhiJ0Tau"))  h->GetXaxis()->SetTitle("#Delta#Phi(J_{0},#tau) [rad]");
			if (std::string::npos != title1.find("dR_mutau"))  h->GetXaxis()->SetTitle("#Delta R(#mu,#tau)");
			if (std::string::npos != title1.find("dR_eltau"))  h->GetXaxis()->SetTitle("#Delta R(e,#tau)");
			if (std::string::npos != title1.find("dR_muel"))  h->GetXaxis()->SetTitle("#Delta R(#mu,e)");
			if (std::string::npos != title1.find("Centrality"))  h->GetXaxis()->SetTitle("Centrality");
			if (std::string::npos != title1.find("DZeta"))  h->GetXaxis()->SetTitle("D#zeta [GeV]");
			if (std::string::npos != title1.find("InvMassMuTau"))  h->GetXaxis()->SetTitle("M_{#mu,#tau} [GeV]");
			if (std::string::npos != title1.find("InvMassElTau"))  h->GetXaxis()->SetTitle("M_{e,#tau} [GeV]");
			if (std::string::npos != title1.find("InvMassMuEl"))  h->GetXaxis()->SetTitle("M_{#mu,e} [GeV]");
			 if (std::string::npos != title1.find("relIsoMu"))  h->GetXaxis()->SetTitle("RelIso{#mu}");
                        if (std::string::npos != title1.find("relIsoEl"))  h->GetXaxis()->SetTitle("RelIso{e}");

			if (std::string::npos != title1.find("PtJ"))  h->GetXaxis()->SetTitle("P_{T}(J) [GeV]");
			if (std::string::npos != title1.find("PtJ1"))  h->GetXaxis()->SetTitle("P_{T}(J_{0}) [GeV]");
			if (std::string::npos != title1.find("PtJ2"))  h->GetXaxis()->SetTitle("P_{T}(J_{1}) [GeV]");
			if (std::string::npos != title1.find("PtJ3"))  h->GetXaxis()->SetTitle("P_{T}(J_{2}) [GeV]");
			if (std::string::npos != title1.find("MupT"))  h->GetXaxis()->SetTitle("P_{T}(#mu) [GeV]");
			if (std::string::npos != title1.find("mupT"))  h->GetXaxis()->SetTitle("P_{T}(#mu) [GeV]");
			if (std::string::npos != title1.find("ElpT"))  h->GetXaxis()->SetTitle("P_{T}(e) [GeV]");
			if (std::string::npos != title1.find("TaupT"))  h->GetXaxis()->SetTitle("P_{T}(#tau) [GeV]");
			if (std::string::npos != title1.find("LeppT"))  h->GetXaxis()->SetTitle("P_{T}(lepton) [GeV]");
			if (std::string::npos != title1.find("Mueta"))  h->GetXaxis()->SetTitle("#eta(#mu)");
			if (std::string::npos != title1.find("Eleta"))  h->GetXaxis()->SetTitle("#eta(e)");
			if (std::string::npos != title1.find("Taueta"))  h->GetXaxis()->SetTitle("#eta(#tau)");
			if (std::string::npos != title1.find("Lepeta"))  h->GetXaxis()->SetTitle("#eta(lepton)");
			if (std::string::npos != title1.find("METphi"))  h->GetXaxis()->SetTitle("#Phi(#it{p}_{T}^{miss}) [rad]");
			if (std::string::npos != title1.find("HTOsqrMET"))  h->GetXaxis()->SetTitle("H_{T}/#sqrt{#it{p}_{T}^{miss}}");
			if (std::string::npos != title1.find("MeffEl"))  h->GetXaxis()->SetTitle("M_{eff}(e,H_{T})");
			if (std::string::npos != title1.find("MeffMu"))  h->GetXaxis()->SetTitle("M_{eff}(#mu,H_{T})");
			if (std::string::npos != title1.find("MeffTau"))  h->GetXaxis()->SetTitle("M_{eff}(#tau,H_{T})");
			if (std::string::npos != title1.find("MeffElOsqrMET"))  h->GetXaxis()->SetTitle("M_{eff}(e,H_{T})/#sqrt{#it{p}_{T}^{miss}}");
			if (std::string::npos != title1.find("MeffMuonOsqrMET"))  h->GetXaxis()->SetTitle("M_{eff}(#mu,H_{T})/#sqrt{#it{p}_{T}^{miss}}");
			if (std::string::npos != title1.find("MeffTauOsqrMET"))  h->GetXaxis()->SetTitle("M_{eff}(#tau,H_{T})/#sqrt{#it{p}_{T}^{miss}}");
			if (std::string::npos != title1.find("npv"))  h->GetXaxis()->SetTitle("#PV");
			if (std::string::npos != title1.find("1D"))  {h->GetXaxis()->SetTitle("SR");h->GetXaxis()->SetNdivisions(35);h->GetXaxis()->SetLabelSize(0.02);h->GetYaxis()->SetTitle("Events");}
			if (std::string::npos != title1.find("01J1D"))  {h->GetXaxis()->SetTitle("SR");h->GetXaxis()->SetNdivisions(70);h->GetXaxis()->SetLabelSize(0.01);}
			
			if (std::string::npos != title1.find("BDTmutau_Stau100"))  {h->GetXaxis()->SetTitle("BDT (100,1)");h->GetYaxis()->SetTitle("Events");}
			if (std::string::npos != title1.find("BDTmutau_Stau150"))  {h->GetXaxis()->SetTitle("BDT (150,1)");h->GetYaxis()->SetTitle("Events");}
			if (std::string::npos != title1.find("BDTmutau_Stau200"))  {h->GetXaxis()->SetTitle("BDT (200,1)");h->GetYaxis()->SetTitle("Events");}
			if (std::string::npos != title1.find("BDTmutau_Stau300"))  {h->GetXaxis()->SetTitle("BDT (300,1)");h->GetYaxis()->SetTitle("Events");}
			



			h->SetMinimum(0.009);

			h->SetFillColor(col);
			h->SetLineColor(col);

			ofstream tfile;
			string fname = h->GetName();
			string n_ = h->GetName();
			if (std::string::npos != n_.find("CutFlowUnW") ){

				//   cout<<" Counting from CutFlow 1 "<<h->GetSumOfWeights()<<endl;
				TString outname=title_;
				TString textfilename = "cutflow_"+outname +".txt";
				tfile.open(textfilename);

				tfile << "########################################" << endl;
				for (int b=0;b<25;b++){
					//cout<<h->GetXaxis()->GetBinLabel(b)<<" & "<<h->GetBinContent(b)<<endl;	

					if(h->GetBinContent(b)<0) h->SetBinContent(b,0);

					//ev_ += h->GetBinContent(b);
					//tfile <<h->GetXaxis()->GetBinLabel(b)<<" & "<<h->GetBinContent(b)<<" & "<<" & "<<endl;	
//					if (b>10 &&  std::string::npos != fname.find("TT_TuneCUETP8M1_13TeV-powheg-pythia8")) h->SetBinContent(b, h->GetBinContent(b) * h->GetBinContent(10) / h->GetBinContent(11));

					//if (h->GetBinContent(b) > 0. ) tfile <<h->GetXaxis()->GetBinLabel(b)<<" & "<<h->GetBinContent(b)<<endl;	
					//else tfile <<h->GetXaxis()->GetBinLabel(b)<<" & "<<0<<endl;
					if (h->GetBinContent(b)>=0) tfile <<h->GetXaxis()->GetBinLabel(b)<<" & "<<fabs(h->GetBinContent(b))<<endl;
					else tfile <<h->GetXaxis()->GetBinLabel(b)<<" & 0"<<endl;
					//tfile <<h->GetXaxis()->GetBinLabel(b)<<" & "<<fabs(h->GetBinContent(b))<<endl;

					//if (b==15) cout<<"  "<<h->GetXaxis()->GetBinLabel(b)<<" & "<<h->GetBinContent(b)<<endl;
					//cout <<title1<<" & "<<h->GetXaxis()->GetBinLabel(b)<<" & "<<h->GetBinContent(b)<<" & "<<" & "<<endl;	
				}
				// cout<<" Counting from CutFlow  "<<ev_<<endl;
			}
tfile.close();
string namet=h->GetName();



}





void OverFlow(TH1D *& h, int &last_bin){

        int nb = h->GetNbinsX();
        float over_ = h->GetBinContent(last_bin);
        float contlast = 0.;//h->GetBinContent(last_bin);
        for (int b=last_bin; b <= nb+1; ++b) {contlast +=h->GetBinContent(b);h->SetBinContent(b,0.);h->SetBinError(b,0.);}

        h->SetBinContent(last_bin,0);
        h->SetBinError(last_bin,0);
        h->SetBinContent(last_bin,contlast);
        //h->Sumw2();
        float err = h->GetBinError(last_bin);
        err = sqrt(contlast);
        h->SetBinError(last_bin,err);
}





void FixBoxPadding(TPad *pad, TBox *box,  TH1D *& hobj,double frac) {
  // Get the bounds of the box - these are in the normalised
  // Pad co-ordinates.
  double p_x1 = box->GetX1();
  double p_x2 = box->GetX2();
  double p_y1 = box->GetY1();
  // double p_y2 = box->GetY2();

  // Convert to normalised co-ordinates in the frame
  double f_x1 = (p_x1 - pad->GetLeftMargin()) /
                (1. - pad->GetLeftMargin() - pad->GetRightMargin());
  double f_x2 = (p_x2 - pad->GetLeftMargin()) /
                (1. - pad->GetLeftMargin() - pad->GetRightMargin());
  double f_y1 = (p_y1 - pad->GetBottomMargin()) /
                (1. - pad->GetTopMargin() - pad->GetBottomMargin());

  // Extract histogram providing the frame and axes
  //TH1 *hobj = GetAxisHist(pad);

  double xmin = hobj->GetBinLowEdge(hobj->GetXaxis()->GetFirst());
  double xmax = hobj->GetBinLowEdge(hobj->GetXaxis()->GetLast()+1);
  double ymin = hobj->GetMinimum();
  double ymax = hobj->GetMaximum();

  // Convert box bounds to x-axis values
  double a_x1 = xmin + (xmax - xmin) * f_x1;
  double a_x2 = xmin + (xmax - xmin) * f_x2;

  // Get the histogram maximum in this range, given as y-axis value
  double a_max_h = hobj->GetMaximum();

  // Convert this to a normalised frame value
  double f_max_h = (a_max_h - ymin) / (ymax - ymin);
  if (gPad->GetLogy()) {
    f_max_h = (log10(a_max_h) - log10(ymin)) / (log10(ymax) - log10(ymin));
  }

  if (f_y1 - f_max_h < frac) {
    double f_target = 1. - (f_y1 - frac);
    FixTopRange(pad, hobj,a_max_h,f_target);
  }
}
void FixOverlay() { gPad->GetFrame()->Draw(); gPad->RedrawAxis(); }
void FixTopRange(TPad *pad, TH1D *& hobj, double fix_y,double fraction) {
  //TH1* hobj = GetAxisHist(pad);
  double ymin = hobj->GetMinimum();
  hobj->SetMaximum((fix_y - fraction * ymin) / (1. - fraction));
  if (gPad->GetLogy()) {
    if (ymin == 0.) {
      std::cout
          << "Can't adjust log-scale y-axis range if the minimum is zero!\n";
      return;
    }
    double max =
        (std::log10(fix_y) - fraction * std::log10(ymin)) / (1 - fraction);
    max = std::pow(8, max);
    hobj->SetMaximum(max);
    hobj->GetYaxis()->SetNdivisions(512);
  }
}
