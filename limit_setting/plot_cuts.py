import uproot
import pandas as pd
#from sklearn.math.vectors import LorentzVector
import numpy as np
import math
from ROOT import TLorentzVector
from SUSY_functions import dPhiFrom2P
#Fill MuV, TauV, METV
#def dataloader():
#    file_list = "listmutau"
#    f = open(file_list)
dirname = "../stau2016_for_training/"
#datafile = f.readline()
#if os.path.exists(dirname+filename)
#else print()
filename = "DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0Parton_B_OS.root"
root_file = uproot.open(dirname+filename)

    #Extract:
Mu = ['mu_pt', 'mu_eta', 'mu_phi']
Tau = [ 'ta_pt', 'ta_eta', 'ta_phi']
MET  = [ 'met_pt','met_phi', 'met_ex', 'met_ey', 'met_ez']
INDEX = ['taus_index','muon_index']


index = root_file['mutau/T'].arrays(INDEX)
index_df = pd.DataFrame(index)

met = root_file['mutau/T'].arrays(MET)
met_df = root_file['mutau/T'].arrays(MET)

muons = root_file['mutau/T'].arrays(Mu)
muons_df = pd.DataFrame(muons)

taus = root_file['mutau/T'].arrays(Tau)
taus_df = pd.DataFrame(taus)

#Iterate in order to get particle info by index:
#@iterate
muonMass = 0.10565837
for i in range(0, taus_df.shape()[0]):

    #Feature selection
    if(muon_index>-1):
        MuV = TLorentzVector()
        MuV.SetPxPyPzE(0, 0, 0, 0)
    #Fill Muon4Vector
    if(tau_index>-1):  #Fill Tau4Vector
        TauV = LorentzVector()
        TauV.SetPxPyPzE(0, 0, 0, 0)

    MuV.SetPtEtaPhiM(mu_pt[0][0], mu_eta[0][0], mu_phi[0][0], muonMass);
    TauV.SetPtEtaPhiM(ta_pt[taus_index], ta_eta[taus_index], ta_phi[taus_index], ta_mass[taus_index]);

    METV.SetPx(met_ex);
    METV.SetPy(met_ey);
    met = sqrt(met_ex * met_ex + met_ey * met_ey);

    process(MuV, TauV, MET)


def process(MuV, TauV, MET):

    dEta = MuV.Eta() - TauV.Eta();

    dPhiT = dPhiFrom2P(TauV.Px(), TauV.Py(), METV.Px(), METV.Py());

    MT = sqrt(2 * DiL.Pt() * met * (1 - TMath::Cos(dPhi)));
    MTt = sqrt(2 * TauV.Pt() * met * (1 - TMath::Cos(dPhiT)));
    mttotal = sqrt(MT * MT + MTt * MTt);


    dPhiMuMET = dPhiFrom2P(MuV.Px(), MuV.Py(), METV.Px(), METV.Py());
    dPhiTauMET = dPhiFrom2P(TauV.Px(), TauV.Py(), METV.Px(), METV.Py());

    Mt2as = asymm_mt2_lester_bisect::get_mT2(muonMass, MuV.Px(), MuV.Py(), tauMass, TauV.Px(), TauV.Py(), METV.Px(),
                         METV.Py(), 0, 0, 0)

    tauUnitX = TauV.Px() / TauV.Pt()
    tauUnitY = TauV.Py() / TauV.Pt()

    muonUnitX = MuV.Px() / MuV.Pt()
    muonUnitY = MuV.Py() / MuV.Pt()


    zetaX = tauUnitX + muonUnitX
    zetaY = tauUnitY + muonUnitY

    normZeta = math.sqrt(zetaX * zetaX + zetaY * zetaY)
    zetaX = zetaX / normZeta
    zetaY = zetaY / normZeta


    vectorVisX = MuV.Px() + TauV.Px()
    vectorVisY = MuV.Py() + TauV.Py()


    pzetamiss = METV.Px() * zetaX + METV.Py() * zetaY
    pzetavis = vectorVisX * zetaX + vectorVisY * zetaY
    dzeta = pzetamiss - 0.85 * pzetavis

    DiL = MuV + TauV

    mcta = sqrt(2 * MuV.Pt() * TauV.Pt() * (1 + cos(MuV.Phi() - TauV.Phi())))


    Dr = deltaR(MuV.Eta(), MuV.Phi(),
    TauV.Eta(), TauV.Phi());


    (*inputVec)[0] = METV.Pt()
    (*inputVec)[1] = MuV.Pt()
    (*inputVec)[2] = TauV.Pt()
    (*inputVec)[3] = dEta
    (*inputVec)[4] = mttotal
    (*inputVec)[5] = dzeta
    (*inputVec)[6] = DiL.M()
    (*inputVec)[7] = Dr
    (*inputVec)[8] = MT
    (*inputVec)[9] = mcta
    (*inputVec)[10] = Mt2as
    (*inputVec)[11] = dPhiMuMET
    (*inputVec)[12] = dPhiTauMET

    ##CallPython

    return RESPONSE