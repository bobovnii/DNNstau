
from ROOT import Math
from ROOT import TLorentzVector


MuMass = 0.105658367
tauMass = 1.776
electronMass = 0
muonMass = 0.10565837
pionMass = 0.1396

def dPhiFrom2P(Px1, Py1,Px2,Py2) {


    prod = Px1 * Px2 + Py1 * Py2
    mod1 = TMath.Sqrt(Px1 * Px1 + Py1 * Py1)
    mod2 = TMath.Sqrt(Px2 * Px2 + Py2 * Py2)

    cosDPhi = prod / (mod1 * mod2)

    return TMath.ACos(cosDPhi)

    }