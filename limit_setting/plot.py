import uproot
import pandas as pd
import numpy as np
from skhep.math.vectors import LorentzVector
"""
Variables distribution:
"""

Channel = "mutau"
Region = ""
chiMass = 0.0


CutList = []



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
CutList.push_back("btag=0")
CutList.push_back("MTwindow")
CutList.push_back("Jets<1 ")
CutList.push_back("BDThigh")
CutList.push_back("BDTlow")
CutList.push_back("StauCuts")
CutList.push_back("StauCuts2")
CutList.push_back("MET>20")
CutList.push_back("MET>30")
CutList.push_back("MET>40")
CutList.push_back("MTsum>120")

const double MuMass = 0.105658367
double tauMass = 1.776
const double electronMass = 0.51100e-3
const double muonMass = 0.10565837
const double pionMass = 0.1396
const double bTagCut = 0.8000

double dzeta = -9999
double dzetaR = -9999
double pzetavis = -9999
double pzetamiss = -9999
double Mt2as = -1
double met = 0
double metx = 0
double mety = 0
double metphi = 0

  wScale0
  wScale1
  wScale2
  wScale3
  wScale4
  wScale5
  wScale6
  wScale7
  wScale8
  wPDFUp
  wPDFDown


def analyzer():
    """

    :return:
    """

    return


def FillHists(CutIndex, EvWeight, elV, muV, tauV, JetV, MetV,
             ChiMass, mIntermediate, Sel, mIndex, eIndex, tIndex, dogenMET):
    """
    Fill Histogram
    :return:
    """

    met = []

    if(dogenMET):
        met = []

    if( _check_selection == "mutau"):

        sumMuonpT = 0
        sumElpT = 0
        sumTaupT = 0

        hnJet[CutIndex]->Fill(njets, EvWeight)
        hnpartons[CutIndex]->Fill(npartons, EvWeight)
        hnMu[CutIndex]->Fill(mu_count, EvWeight)
        hnTau[CutIndex]->Fill(ta_count, EvWeight)
        hnEl[CutIndex]->Fill(el_count, EvWeight)

        hWeights[CutIndex]->Fill(EvWeight)
        hnpv[CutIndex]->Fill(npv, EvWeight)
        hnpu[CutIndex]->Fill(npu, EvWeight)
        hEventSign[CutIndex]->Fill(event_sign, EvWeight)

    #Fill Some kind of histograms
    if(channel=="mutau"):
        sminn = SMin(muV, tauV, met, 0.)
        hSmin[CutIndex]->Fill(sminn, EvWeight)
        tauMass = tauV.M()
        deta = deltaEta(muV.Px(), muV.Py(), muV.Pz(), tauV.Px(), tauV.Py(), tauV.Pz())
        hdEtaDil[CutIndex]->Fill(deta, EvWeight)

        LorentzVector t1
        t1 = MetV + muV
        double detaM = muV.Eta() - t1.Eta()
        hdEtaMuMET[CutIndex]->Fill(detaM, EvWeight)
        double dPhiD = dPhiFrom2P(muV.Px(), muV.Py(), tauV.Px(), tauV.Py())
        hdPhiDil[CutIndex]->Fill(dPhiD, EvWeight)

        double v1[4] = {muV.Px(), muV.Py(), muV.Pz(), muonMass}
        double v2[4] = {tauV.Px(), tauV.Py(), tauV.Pz(), tauMass}
        double ecm = 13000.0
        double mxlo = Chimass
        double ptm[2] = {metx, mety}
        double vds[4] = {0, 0, 0, 0}

        Mt2ass = 0
        Mt2ass = asymm_mt2_lester_bisect::get_mT2(muV.M(), muV.Px(), muV.Py(), tauMass, tauV.Px(), tauV.Py(), metx, mety,
                                                  Chimass, Chimass, 0)
        mcta = sqrt(2 * muV.Pt() * tauV.Pt() * (1 + cos(muV.Phi() - tauV.Phi())))
        pa[3] = {muonMass, muV.Px(), muV.Py()}
        pb[3] = {tauMass, tauV.Px(), tauV.Py()}
        pmiss[3] = {0., metx, mety}

        MT2v = 0
        MT2v = mt2(v1, v2, vds, ptm, ecm, mxlo)

        hMt2mutau[CutIndex]->Fill(MT2v, EvWeight)
        hMt2lestermutau[CutIndex]->Fill(Mt2as, EvWeight)
        hMt2lestermutauFB[CutIndex]->Fill(Mt2as, EvWeight)
        hMCTmutau[CutIndex]->Fill(mct(v1, v2), EvWeight)
        hMCTxmutau[CutIndex]->Fill(mcx(v1, v2, vds, ptm), EvWeight)
        hMCTymutau[CutIndex]->Fill(mcy(v1, v2, vds, ptm), EvWeight)
        hMCTbmutau[CutIndex]->Fill(mcta, EvWeight)

        MCTcorr = mctcorr(v1, v2, vds, ptm, ecm, mxlo)
        hMCTcor[CutIndex]->Fill(MCTcorr, EvWeight)


        Centr = Centrality(AllJets_Lepton_noMet)
        hCentrality[CutIndex]->Fill(Centr, EvWeight)

        mTB = 0
        mTtrue = Lester::mTTrue(muV.M(), muV.Px(), muV.Py(), muV.Pz(), tauMass, tauV.Px(), tauV.Py(), tauV.Pz(), metx,
                                mety, mintermediate)
        hMTBoundmutau[CutIndex]->Fill(mTB, EvWeight)
        hMTTruemutau[CutIndex]->Fill(mTtrue, EvWeight)


        Dr = muV.DeltaR(tauV)
        TLorentzVector   DiL = muV + tauV
        double dPhi = dPhiFrom2P(DiL.Px(), DiL.Py(), metx, mety)
        double MTDil = TMath::Sqrt(2 * DiL.Pt() * met * (1 - np.cos(dPhi)))
        hMTmutau[CutIndex]->Fill(MTDil, EvWeight)
        hInvMassMuTau[CutIndex]->Fill(DiL.M(), EvWeight)
        hdR_mutau[CutIndex]->Fill(Dr, EvWeight)

        dPhi = dPhiFrom2P(mu_px[mIndex], mu_py[mIndex], metx, mety)
        hdPhiMuMET[CutIndex]->Fill(dPhi, EvWeight)

        MT = np.sqrt(2 * mu_pt[mIndex] * met * (1 - np.cos(dPhi)))
        dPhi = dPhiFrom2P(tauV.Px(), tauV.Py(), metx, mety)
        MTt = np.sqrt(2 * tauV.Pt() * met * (1 - np.cos(dPhi)))
        hMTsum[CutIndex]->Fill(MT + MTt, EvWeight)
        hMTmax[CutIndex]->Fill(max(MT, MTt), EvWeight)
        hMTmin[CutIndex]->Fill(min(MT, MTt), EvWeight)
        mttotal = sqrt(MT * MT + MTt * MTt)
        hMTtot[CutIndex]->Fill(mttotal, EvWeight)
        hMTdiff[CutIndex]->Fill(MTDil - MT, EvWeight)


        tauUnitX = tauV.Px() / tauV.Pt()
        tauUnitY = tauV.Py() / tauV.Pt()

        muonUnitX = muV.Px() / muV.Pt()
        muonUnitY = muV.Py() / muV.Pt()

        zetaX = tauUnitX + muonUnitX
        zetaY = tauUnitY + muonUnitY

        normZeta = np.sqrt(zetaX * zetaX + zetaY * zetaY)

        zetaX = zetaX / normZeta
        zetaY = zetaY / normZeta

        vectorX = metx + muV.Px() + tauV.Px()
        vectorY = mety + muV.Py() + tauV.Py()

        vectorVisX = muV.Px() + tauV.Px()
        vectorVisY = muV.Py() + tauV.Py()


        pzetamiss = metx * zetaX + mety * zetaY
        pzetavis = vectorVisX * zetaX + vectorVisY * zetaY
        dzeta = pzetamiss - 0.85 * pzetavis

        ###The end  - line number 958