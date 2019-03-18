//#pragma once

#include "TFitter.h"
#include "TMath.h"
#include <vector>
#include "Math/LorentzVector.h"
#include <math.h>
#include "TLorentzVector.h"
#include "/nfs/dust/cms/user/alkaloge/TauAnalysis/new/new/StauAnalysis/New8025/CMSSW_8_0_25/src/DesyTauAnalyses/NTupleMaker/interface/JetUtil.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

using namespace std;

// comments, see .C file

float topnessMinimization(TLorentzVector met, TLorentzVector lep, TLorentzVector bjet1, TLorentzVector bjet2, int version);
float topnessMinimization1BTag(TLorentzVector met, TLorentzVector lep, TLorentzVector bjet1, int version);

double topnessFunction(double pwx_, double pwy_, double pwz_, double pnz_,
                       double plx_, double ply_, double plz_, double ple_,
                       double pb1x_, double pb1y_, double pb1z_, double pb1e_,
                       double pb2x_, double pb2y_, double pb2z_, double pb2e_,
                       double pmx_, double pmy_, double pmz_, double pme_);

double topnessFunction1BTag(double pwx_, double pwy_, double pwz_, double pnz_,
                       double plx_, double ply_, double plz_, double ple_,
                       double pb1x_, double pb1y_, double pb1z_, double pb1e_,
                       double pmx_, double pmy_, double pmz_, double pme_);

void minuitFunctionWrapper(int& nDim, double* gout, double& result, double* par, int flg);

double topnessFunctionV1(double pwx_, double pwy_, double pwz_, double pnz_,
                       double plx_, double ply_, double plz_, double ple_,
                       double pb1x_, double pb1y_, double pb1z_, double pb1e_,
                       double pb2x_, double pb2y_, double pb2z_, double pb2e_,
                       double pmx_, double pmy_, double pmz_, double pme_);


double topnessFunction1BTagFull(double pwx_, double pwy_, double pwz_, double pnz_,
                       double plx_, double ply_, double plz_, double ple_,
                       double pb1x_, double pb1y_, double pb1z_, double pb1e_,
                       double pmx_, double pmy_, double pmz_, double pme_);

void minuitFunctionWrapperV1(int& nDim, double* gout, double& result, double* par, int flg);

void minuitFunctionWrapper1BTag(int& nDim, double* gout, double& result, double* par, int flg);


float CalcTopness( int version, LorentzVector MET,       LorentzVector lep, vector<LorentzVector> bjets,  vector<LorentzVector> addjets);
float CalcTopness_(int version, float MET, float METphi, LorentzVector lep, vector<LorentzVector> bjets,  vector<LorentzVector> addjets);
float CalcTopness1BTag( int version, LorentzVector MET,       LorentzVector lep, vector<LorentzVector> bjets,  vector<LorentzVector> addjets);
float CalcTopness1BTag_(int version, float MET, float METphi, LorentzVector lep, vector<LorentzVector> bjets,  vector<LorentzVector> addjets);

//#endif
