"""
Python script for the analyzermutau.C

"""

import pandas as pd
import uproot 
import numpy as np
from skhep.math.vectors import LorentzVector


#1.Define list of cuts 
CutList = []
CutList.clear() 
CutList.push_back("Nocut") 
CutList.push_back("PU") 
CutList.push_back("#mu#tau") 
CutList.push_back("2ndlepV") 
CutList.push_back("3rdlepV") 
CutList.push_back("TriggerSF") 
CutList.push_back("LeptonSF") 
CutList.push_back("Tau-id") 
CutList.push_back("TransferR") 
CutList.push_back("LeptonTauFakeRate") 
CutList.push_back("METrecoil") 
CutList.push_back("ZpTcorrection") 
CutList.push_back("topPtRwgt") 
CutList.push_back("Jets<2 ") 
CutList.push_back("btag=0") 
CutList.push_back("MTwindow")  //15
CutList.push_back("Jets<1 ") 
CutList.push_back("BDThigh") 
CutList.push_back("BDTlow") 
CutList.push_back("StauCuts") 
CutList.push_back("StauCuts2") 
CutList.push_back("MET>20") 
CutList.push_back("MET>30") 
CutList.push_back("MET>40") 
CutList.push_back("MTsum>120")

CHANNEL = "mutau"

for (int i=0 i < CutNumb  i++){
            
        CFCounter[i] = 0 
		CFCounter_[i] = 0 
        iCFCounter[i] = 0 
}

 
isData = False
isTT = False
isWJ = False
isDY = False
isDYee = False
isDYmm = False
isDYnunu = False
isDYhigh = False
isDYlow = False
isZTT = False
isDYNJ = False
isWNJ = False
isNJ = False
isSignal = False
isSUSY = False
cut_btag = True
t = True
f = False

InvertLeptonIso = LEPTONHERE

#Make cut on mu_relISo[0]>0.15
#Fill Muon and Tau 4-Vector
MuV  = LorentzVector()
TauV = LorentzVector()
if (muon_index>-1):


