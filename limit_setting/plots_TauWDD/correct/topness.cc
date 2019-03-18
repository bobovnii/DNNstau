#include "/nfs/dust/cms/user/dydukhle/CMSSW_9_4_0_patch1/src/DesyTauAnalyses/NTupleMaker/test/topness.h"

void minuitFunctionWrapper(int& nDim, double* gout, double& result, double* par, int flg) {
  result = topnessFunction(par[0],par[1],par[2],par[3],
                           par[4],par[5],par[6],par[7],
                           par[8],par[9],par[10],par[11],
                           par[12],par[13],par[14],par[15],
                           par[16],par[17],par[18],par[19]
                           );
}

void minuitFunctionWrapperV1(int& nDim, double* gout, double& result, double* par, int flg) {
  result = topnessFunctionV1(par[0],par[1],par[2],par[3],
                           par[4],par[5],par[6],par[7],
                           par[8],par[9],par[10],par[11],
                           par[12],par[13],par[14],par[15],
                           par[16],par[17],par[18],par[19]
                           );
}
void minuitFunctionWrapper1BTag(int& nDim, double* gout, double& result, double* par, int flg) {
  result = topnessFunction1BTag(par[0],par[1],par[2],par[3],
                           par[4],par[5],par[6],par[7],
                           par[8],par[9],par[10],par[11],
                           par[12],par[13],par[14],par[15]
                           );
}

//version 1: without 'reco top' and 'CM' term
//all other version: full topness (including 'reco W')
float topnessMinimization(TLorentzVector met, TLorentzVector lep, TLorentzVector bjet1, TLorentzVector bjet2, int version) {

  cout.precision(11);
  TFitter *minimizer=new TFitter(4) ;
  double p1 = -1;
  minimizer->ExecuteCommand("SET PRINTOUT",&p1,1);
  if(version==1) minimizer->SetFCN(minuitFunctionWrapperV1);
  else minimizer->SetFCN(minuitFunctionWrapper);
  // get variables for Topness
  int mydigit = 3;
  double iLpx = JetUtil::round(lep.Px(),mydigit);
  double iLpy = JetUtil::round(lep.Py(),mydigit);
  double iLpz = JetUtil::round(lep.Pz(),mydigit);
  double iLpe = JetUtil::round(lep.E(),mydigit);
  double iB1px = JetUtil::round(bjet1.Px(),mydigit);
  double iB1py = JetUtil::round(bjet1.Py(),mydigit);
  double iB1pz = JetUtil::round(bjet1.Pz(),mydigit);
  double iB1pe = JetUtil::round(bjet1.E(),mydigit);
  double iB2px = JetUtil::round(bjet2.Px(),mydigit);
  double iB2py = JetUtil::round(bjet2.Py(),mydigit);
  double iB2pz = JetUtil::round(bjet2.Pz(),mydigit);
  double iB2pe = JetUtil::round(bjet2.E(),mydigit);
  double iMpx = JetUtil::round(met.Px(),mydigit);
  double iMpy = JetUtil::round(met.Py(),mydigit);
  double iMpz = JetUtil::round(met.Pz(),mydigit);
  double iMpe = JetUtil::round(met.E(),mydigit);

  // Define parameters [param number, param name, init val, estimated distance to min, bla, bla] // 300,3000,-3000,3000
  minimizer->SetParameter(0,"pwx",0,500,-3000,3000);
  minimizer->SetParameter(1,"pwy",0,500,-3000,3000);
  minimizer->SetParameter(2,"pwz",0,500,-3000,3000);
  minimizer->SetParameter(3,"pnz",0,500,-3000,3000);
  // fixed parameters
  minimizer->SetParameter(4,"plx",iLpx,0,iLpx-0.001,iLpx+0.001);
  minimizer->SetParameter(5,"ply",iLpy,0,iLpy-0.001,iLpy+0.001);
  minimizer->SetParameter(6,"plz",iLpz,0,iLpz-0.001,iLpz+0.001);
  minimizer->SetParameter(7,"ple",iLpe,0,iLpe-0.001,iLpe+0.001);
  minimizer->SetParameter(8,"pb1x",iB1px,0,iB1px-0.001,iB1px+0.001);
  minimizer->SetParameter(9,"pb1y",iB1py,0,iB1py-0.001,iB1py+0.001);
  minimizer->SetParameter(10,"pb1z",iB1pz,0,iB1pz-0.001,iB1pz+0.001);
  minimizer->SetParameter(11,"pb1e",iB1pe,0,iB1pe-0.001,iB1pe+0.001);
  minimizer->SetParameter(12,"pb2x",iB2px,0,iB2px-0.001,iB2px+0.001);
  minimizer->SetParameter(13,"pb2y",iB2py,0,iB2py-0.001,iB2py+0.001);
  minimizer->SetParameter(14,"pb2z",iB2pz,0,iB2pz-0.001,iB2pz+0.001);
  minimizer->SetParameter(15,"pb2e",iB2pe,0,iB2pe-0.001,iB2pe+0.001);
  minimizer->SetParameter(16,"pmx",iMpx,0,iMpx-0.001,iMpx+0.001);
  minimizer->SetParameter(17,"pmy",iMpy,0,iMpy-0.001,iMpy+0.001);
  minimizer->SetParameter(18,"pmz",iMpz,0,iMpz-0.001,iMpz+0.001);
  minimizer->SetParameter(19,"pme",iMpe,0,iMpe-0.001,iMpe+0.001);
  minimizer->FixParameter(4);
  minimizer->FixParameter(5);
  minimizer->FixParameter(6);
  minimizer->FixParameter(7);
  minimizer->FixParameter(8);
  minimizer->FixParameter(9);
  minimizer->FixParameter(10);
  minimizer->FixParameter(11);
  minimizer->FixParameter(12);
  minimizer->FixParameter(13);
  minimizer->FixParameter(14);
  minimizer->FixParameter(15);
  minimizer->FixParameter(16);
  minimizer->FixParameter(17);
  minimizer->FixParameter(18);
  minimizer->FixParameter(19);
  // Run the simplex minimizer to get close to the minimum [no good precision, but robust]
  // For signal regions, no difference seen with migrad, but more stable
  minimizer->ExecuteCommand("SIMPLEX",0,0);
  //Get the best fit values
  double pwx_fit = minimizer->GetParameter(0);
  double pwy_fit = minimizer->GetParameter(1);
  double pwz_fit = minimizer->GetParameter(2);
  double pnz_fit = minimizer->GetParameter(3);
  //cout << "output minimizer" << endl;
  //cout << "pwx " << pwx_fit << " pwy " << pwy_fit << " pwz " << pwz_fit << " pnz " << pnz_fit << endl;
  delete minimizer;
  // get the function value at best fit
  if(version==1){
    return log(topnessFunctionV1(pwx_fit,pwy_fit,pwz_fit,pnz_fit,
				 iLpx,iLpy,iLpz,iLpe,
				 iB1px,iB1py,iB1pz,iB1pe,
				 iB2px,iB2py,iB2pz,iB2pe,
				 iMpx,iMpy,iMpz,iMpe
				 ));
  }
  else {
    return log(topnessFunction(pwx_fit,pwy_fit,pwz_fit,pnz_fit,
                               iLpx,iLpy,iLpz,iLpe,
                               iB1px,iB1py,iB1pz,iB1pe,
                               iB2px,iB2py,iB2pz,iB2pe,
                               iMpx,iMpy,iMpz,iMpe
                               ));
  }
} // ~ end of Topness Minimization()

float topnessMinimization1BTag(TLorentzVector met, TLorentzVector lep, TLorentzVector bjet1, int version) {

  cout.precision(11);
  TFitter *minimizer=new TFitter(4) ;
  double p1 = -1;
  minimizer->ExecuteCommand("SET PRINTOUT",&p1,1);
  minimizer->SetFCN(minuitFunctionWrapper1BTag);
  // get variables for Topness
  int mydigit = 3;
  double iLpx = JetUtil::round(lep.Px(),mydigit);
  double iLpy = JetUtil::round(lep.Py(),mydigit);
  double iLpz = JetUtil::round(lep.Pz(),mydigit);
  double iLpe = JetUtil::round(lep.E(),mydigit);
  double iB1px = JetUtil::round(bjet1.Px(),mydigit);
  double iB1py = JetUtil::round(bjet1.Py(),mydigit);
  double iB1pz = JetUtil::round(bjet1.Pz(),mydigit);
  double iB1pe = JetUtil::round(bjet1.E(),mydigit);
  double iMpx = JetUtil::round(met.Px(),mydigit);
  double iMpy = JetUtil::round(met.Py(),mydigit);
  double iMpz = JetUtil::round(met.Pz(),mydigit);
  double iMpe = JetUtil::round(met.E(),mydigit);

  // Define parameters [param number, param name, init val, estimated distance to min, bla, bla] // 300,3000,-3000,3000
  minimizer->SetParameter(0,"pwx",0,500,-3000,3000);
  minimizer->SetParameter(1,"pwy",0,500,-3000,3000);
  minimizer->SetParameter(2,"pwz",0,500,-3000,3000);
  minimizer->SetParameter(3,"pnz",0,500,-3000,3000);
  // fixed parameters
  minimizer->SetParameter(4,"plx",iLpx,0,iLpx-0.001,iLpx+0.001);
  minimizer->SetParameter(5,"ply",iLpy,0,iLpy-0.001,iLpy+0.001);
  minimizer->SetParameter(6,"plz",iLpz,0,iLpz-0.001,iLpz+0.001);
  minimizer->SetParameter(7,"ple",iLpe,0,iLpe-0.001,iLpe+0.001);
  minimizer->SetParameter(8,"pb1x",iB1px,0,iB1px-0.001,iB1px+0.001);
  minimizer->SetParameter(9,"pb1y",iB1py,0,iB1py-0.001,iB1py+0.001);
  minimizer->SetParameter(10,"pb1z",iB1pz,0,iB1pz-0.001,iB1pz+0.001);
  minimizer->SetParameter(11,"pb1e",iB1pe,0,iB1pe-0.001,iB1pe+0.001);
  minimizer->SetParameter(12,"pmx",iMpx,0,iMpx-0.001,iMpx+0.001);
  minimizer->SetParameter(13,"pmy",iMpy,0,iMpy-0.001,iMpy+0.001);
  minimizer->SetParameter(14,"pmz",iMpz,0,iMpz-0.001,iMpz+0.001);
  minimizer->SetParameter(15,"pme",iMpe,0,iMpe-0.001,iMpe+0.001);
  minimizer->FixParameter(4);
  minimizer->FixParameter(5);
  minimizer->FixParameter(6);
  minimizer->FixParameter(7);
  minimizer->FixParameter(8);
  minimizer->FixParameter(9);
  minimizer->FixParameter(10);
  minimizer->FixParameter(11);
  minimizer->FixParameter(12);
  minimizer->FixParameter(13);
  minimizer->FixParameter(14);
  minimizer->FixParameter(15);
  // Run the simplex minimizer to get close to the minimum [no good precision, but robust]
  // For signal regions, no difference seen with migrad, but more stable
  minimizer->ExecuteCommand("SIMPLEX",0,0);
  //Get the best fit values
  double pwx_fit = minimizer->GetParameter(0);
  double pwy_fit = minimizer->GetParameter(1);
  double pwz_fit = minimizer->GetParameter(2);
  double pnz_fit = minimizer->GetParameter(3);
  //cout << "output minimizer" << endl;
  //cout << "pwx " << pwx_fit << " pwy " << pwy_fit << " pwz " << pwz_fit << " pnz " << pnz_fit << endl;
  delete minimizer;
  // get the function value at best fit
  if(version==1){
    return log(topnessFunction1BTag(pwx_fit,pwy_fit,pwz_fit,pnz_fit,
				 iLpx,iLpy,iLpz,iLpe,
				 iB1px,iB1py,iB1pz,iB1pe,
				 iMpx,iMpy,iMpz,iMpe
				 ));
  }
  else {
    return log(topnessFunction1BTagFull(pwx_fit,pwy_fit,pwz_fit,pnz_fit,
                               iLpx,iLpy,iLpz,iLpe,
                               iB1px,iB1py,iB1pz,iB1pe,
			       iMpx,iMpy,iMpz,iMpe
                               ));
  }
} // ~ end of Topness Minimization()

// full function
double topnessFunction(double pwx_, double pwy_, double pwz_, double pnz_,
                       double plx_, double ply_, double plz_, double ple_,
                       double pb1x_, double pb1y_, double pb1z_, double pb1e_,
                       double pb2x_, double pb2y_, double pb2z_, double pb2e_,
                       double pmx_, double pmy_, double pmz_, double pme_) {
  const double mW = 81.;
  const double mT = 172.;
  const double aW = 5.;
  const double aT = 15.;
  const double aCM = 1000.;
    
  // construct the lorentz vectors
  TLorentzVector vW; vW.SetPxPyPzE(pwx_,pwy_,pwz_,(sqrt((mW*mW)+(pwx_*pwx_)+(pwy_*pwy_)+(pwz_*pwz_)))) ;
  TLorentzVector vL; vL.SetPxPyPzE(plx_,ply_,plz_,ple_);
  TLorentzVector vB1; vB1.SetPxPyPzE(pb1x_,pb1y_,pb1z_,pb1e_);
  TLorentzVector vB2; vB2.SetPxPyPzE(pb2x_,pb2y_,pb2z_,pb2e_);
  TLorentzVector vMET; vMET.SetPxPyPzE(pmx_,pmy_,pmz_,pme_);
  TLorentzVector vN; vN.SetPxPyPzE((pmx_-pwx_),(pmy_-pwy_),pnz_,(sqrt(pow((pmx_-pwx_),2)+pow((pmy_-pwy_),2)+pow(pnz_,2))));
  // construct the w-term (lost)
  //double tWM = ( pow( ((mW*mW) - (vW.M2())),2) ) / (pow(aW,4));//zero by construction
  // construct the w-term (lep)
  double tWL = ( pow( ((mW*mW) - ((vL+vN).M2())),2) ) / (pow(aW,4));
  // construct the tL-term [seen lepton]
  double tTL = ( pow( ((mT*mT) - ((vL+vB1+vN).M2())),2) ) / (pow(aT,4));
  // construct the tM-term [miss lepton]
  double tTM = ( pow( ((mT*mT) - ((vB2+vW).M2())),2) ) / (pow(aT,4));
  // construct the CM-term
  double tCM = ( pow( ((4*(mT*mT)) - ((vL+vN+vW+vB1+vB2).M2())),2) ) / (pow(aCM,4));
  // calculate Topness
  double Topness = tWL + tTL + tTM + tCM;
  return Topness;
}

double topnessFunction1BTagFull(double pwx_, double pwy_, double pwz_, double pnz_,
                       double plx_, double ply_, double plz_, double ple_,
                       double pb1x_, double pb1y_, double pb1z_, double pb1e_,
                       double pmx_, double pmy_, double pmz_, double pme_) {
  const double mW = 81.;
  const double mT = 172.;
  const double aW = 5.;
  const double aT = 15.;
  const double aCM = 1000.;
    
  // construct the lorentz vectors
  TLorentzVector vW; vW.SetPxPyPzE(pwx_,pwy_,pwz_,(sqrt((mW*mW)+(pwx_*pwx_)+(pwy_*pwy_)+(pwz_*pwz_)))) ;
  TLorentzVector vL; vL.SetPxPyPzE(plx_,ply_,plz_,ple_);
  TLorentzVector vB1; vB1.SetPxPyPzE(pb1x_,pb1y_,pb1z_,pb1e_);
  TLorentzVector vMET; vMET.SetPxPyPzE(pmx_,pmy_,pmz_,pme_);
  TLorentzVector vN; vN.SetPxPyPzE((pmx_-pwx_),(pmy_-pwy_),pnz_,(sqrt(pow((pmx_-pwx_),2)+pow((pmy_-pwy_),2)+pow(pnz_,2))));
  // construct the w-term (lost)
  //double tWM = ( pow( ((mW*mW) - (vW.M2())),2) ) / (pow(aW,4));//zero by construction
  // construct the w-term (lep)
  double tWL = ( pow( ((mW*mW) - ((vL+vN).M2())),2) ) / (pow(aW,4));
  // construct the tL-term [seen lepton]
  double tTL = ( pow( ((mT*mT) - ((vL+vB1+vN).M2())),2) ) / (pow(aT,4));
  // construct the tM-term [miss lepton]
  //double tTM = ( pow( ((mT*mT) - ((vB2+vW).M2())),2) ) / (pow(aT,4));
  // construct the CM-term
  double tCM = ( pow( ((4*(mT*mT)) - ((vL+vN+vW+vB1).M2())),2) ) / (pow(aCM,4));
  // calculate Topness
  double Topness = tWL + tTL + tCM;
  return Topness;
}
double topnessFunction1BTag(double pwx_, double pwy_, double pwz_, double pnz_,
			 double plx_, double ply_, double plz_, double ple_,
			 double pb1x_, double pb1y_, double pb1z_, double pb1e_,
			 double pmx_, double pmy_, double pmz_, double pme_) {
  const double mW = 81.;
  const double mT = 172.;
  const double aW = 5.;
  const double aT = 15.;
  //const double aCM = 1000.;
    
  // construct the lorentz vectors
  TLorentzVector vW; vW.SetPxPyPzE(pwx_,pwy_,pwz_,(sqrt((mW*mW)+(pwx_*pwx_)+(pwy_*pwy_)+(pwz_*pwz_)))) ;
  TLorentzVector vL; vL.SetPxPyPzE(plx_,ply_,plz_,ple_);
  TLorentzVector vB1; vB1.SetPxPyPzE(pb1x_,pb1y_,pb1z_,pb1e_);
  TLorentzVector vMET; vMET.SetPxPyPzE(pmx_,pmy_,pmz_,pme_);
  TLorentzVector vN; vN.SetPxPyPzE((pmx_-pwx_),(pmy_-pwy_),pnz_,(sqrt(pow((pmx_-pwx_),2)+pow((pmy_-pwy_),2)+pow(pnz_,2))));
  // construct the w-term (lost)
  //double tWM = ( pow( ((mW*mW) - (vW.M2())),2) ) / (pow(aW,4));//zero by construction
  // construct the w-term (lep)
  double tWL = ( pow( ((mW*mW) - ((vL+vN).M2())),2) ) / (pow(aW,4));
  // construct the tL-term [seen lepton]
 double tTL = ( pow( ((mT*mT) - ((vL+vB1+vN).M2())),2) ) / (pow(aT,4));
  // construct the tM-term [miss lepton]
 // double tTM = ( pow( ((mT*mT) - ((vB2+vW).M2())),2) ) / (pow(aT,4));
  // construct the CM-term
  //double tCM = ( pow( ((4*(mT*mT)) - ((vL+vN+vW+vB1+vB2).M2())),2) ) / (pow(aCM,4));
  // calculate Topness
  double Topness = tWL + tTL;
  return Topness;
}

//version 1: without 'reco top' and 'CM' term
double topnessFunctionV1(double pwx_, double pwy_, double pwz_, double pnz_,
			 double plx_, double ply_, double plz_, double ple_,
			 double pb1x_, double pb1y_, double pb1z_, double pb1e_,
			 double pb2x_, double pb2y_, double pb2z_, double pb2e_,
			 double pmx_, double pmy_, double pmz_, double pme_) {
  const double mW = 81.;
  const double mT = 172.;
  const double aW = 5.;
  const double aT = 15.;
  //const double aCM = 1000.;
    
  // construct the lorentz vectors
  TLorentzVector vW; vW.SetPxPyPzE(pwx_,pwy_,pwz_,(sqrt((mW*mW)+(pwx_*pwx_)+(pwy_*pwy_)+(pwz_*pwz_)))) ;
  TLorentzVector vL; vL.SetPxPyPzE(plx_,ply_,plz_,ple_);
  TLorentzVector vB1; vB1.SetPxPyPzE(pb1x_,pb1y_,pb1z_,pb1e_);
  TLorentzVector vB2; vB2.SetPxPyPzE(pb2x_,pb2y_,pb2z_,pb2e_);
  TLorentzVector vMET; vMET.SetPxPyPzE(pmx_,pmy_,pmz_,pme_);
  TLorentzVector vN; vN.SetPxPyPzE((pmx_-pwx_),(pmy_-pwy_),pnz_,(sqrt(pow((pmx_-pwx_),2)+pow((pmy_-pwy_),2)+pow(pnz_,2))));
  // construct the w-term (lost)
  //double tWM = ( pow( ((mW*mW) - (vW.M2())),2) ) / (pow(aW,4));//zero by construction
  // construct the w-term (lep)
  double tWL = ( pow( ((mW*mW) - ((vL+vN).M2())),2) ) / (pow(aW,4));
  // construct the tL-term [seen lepton]
  //double tTL = ( pow( ((mT*mT) - ((vL+vB1+vN).M2())),2) ) / (pow(aT,4));
  // construct the tM-term [miss lepton]
  double tTM = ( pow( ((mT*mT) - ((vB2+vW).M2())),2) ) / (pow(aT,4));
  // construct the CM-term
  //double tCM = ( pow( ((4*(mT*mT)) - ((vL+vN+vW+vB1+vB2).M2())),2) ) / (pow(aCM,4));
  // calculate Topness
  double Topness = tWL + tTM;
  return Topness;
}

//wrapper function to get topness using correct jets as input - full calculation needs JetUtil CSV ordering, see looper
//version: ==1: reduced topness (recommended), else: full topness
float CalcTopness(int version, LorentzVector MET, LorentzVector lep, vector<LorentzVector> bjets,  vector<LorentzVector> addjets){


  if((bjets.size()+addjets.size())<2) return -999.;
  TLorentzVector l, met, b1, b2;
  l.SetPxPyPzE(lep.Px(),lep.Py(),lep.Pz(),lep.E());
  met.SetPxPyPzE(MET.Px(),MET.Py(),MET.Pz(),MET.E());
  float topness = 1e6;
  for(unsigned int n = 0; n<bjets.size(); ++n){
    b1.SetPxPyPzE(bjets[n].Px(),bjets[n].Py(),bjets[n].Pz(),bjets[n].E());
    for(unsigned int m = n+1; m<bjets.size(); ++m){
      b2.SetPxPyPzE(bjets[m].Px(),bjets[m].Py(),bjets[m].Pz(),bjets[m].E());
      float tmptop = topnessMinimization(met,l,b1,b2,version);
      if(tmptop<topness) topness = tmptop;
      //cout << "topness " << tmptop << " for b1 " << b1.Pt() << " and b2 " << b2.Pt() << endl;
      tmptop = topnessMinimization(met,l,b2,b1,version);
      if(tmptop<topness) topness = tmptop;
      //cout << "topness " << tmptop << " for b1 " << b2.Pt() << " and b2 " << b1.Pt() << endl;
    }
    for(unsigned int m = 0; m<addjets.size(); ++m){
      b2.SetPxPyPzE(addjets[m].Px(),addjets[m].Py(),addjets[m].Pz(),addjets[m].E());
      float tmptop = topnessMinimization(met,l,b1,b2,version);
      if(tmptop<topness) topness = tmptop;
      //cout << "topness " << tmptop << " for b1 " << b1.Pt() << " and a2 " << b2.Pt() << endl;
      tmptop = topnessMinimization(met,l,b2,b1,version);
      if(tmptop<topness) topness = tmptop;
      //cout << "topness " << tmptop << " for a1 " << b2.Pt() << " and b2 " << b1.Pt() << endl;
    }
  }
  if(bjets.size()==0){
    b1.SetPxPyPzE(addjets[0].Px(),addjets[0].Py(),addjets[0].Pz(),addjets[0].E());
    for(unsigned int m = 1; m<addjets.size(); ++m){
      b2.SetPxPyPzE(addjets[m].Px(),addjets[m].Py(),addjets[m].Pz(),addjets[m].E());
      float tmptop = topnessMinimization(met,l,b1,b2,version);
      if(tmptop<topness) topness = tmptop;
      tmptop = topnessMinimization(met,l,b2,b1,version);
      if(tmptop<topness) topness = tmptop;
    }
  }
  return topness;
}
float CalcTopness1BTag(int version, LorentzVector MET, LorentzVector lep, vector<LorentzVector> bjets,  vector<LorentzVector> addjets){

  if((bjets.size()+addjets.size())<2) return -999.;
  TLorentzVector l, met, b1, b2;
  l.SetPxPyPzE(lep.Px(),lep.Py(),lep.Pz(),lep.E());
  met.SetPxPyPzE(MET.Px(),MET.Py(),MET.Pz(),MET.E());
  float topness = 1e6;
  for(unsigned int n = 0; n<bjets.size(); ++n){
    b1.SetPxPyPzE(bjets[n].Px(),bjets[n].Py(),bjets[n].Pz(),bjets[n].E());
    //for(unsigned int m = n+1; m<bjets.size(); ++m){
     // b2.SetPxPyPzE(bjets[m].Px(),bjets[m].Py(),bjets[m].Pz(),bjets[m].E());
      float tmptop = topnessMinimization1BTag(met,l,b1,version);
      if(tmptop<topness) topness = tmptop;
      //cout << "topness " << tmptop << " for b1 " << b1.Pt() << " and b2 " << b2.Pt() << endl;
      //tmptop = topnessMinimization1BTag(met,l,b2,b1,version);
      //if(tmptop<topness) topness = tmptop;
      //cout << "topness " << tmptop << " for b1 " << b2.Pt() << " and b2 " << b1.Pt() << endl;
    //}
    for(unsigned int m = 0; m<addjets.size(); ++m){
      b2.SetPxPyPzE(addjets[m].Px(),addjets[m].Py(),addjets[m].Pz(),addjets[m].E());
      float tmptop = topnessMinimization1BTag(met,l,b2,version);
      if(tmptop<topness) topness = tmptop;
      //cout << "topness " << tmptop << " for b1 " << b1.Pt() << " and a2 " << b2.Pt() << endl;
//      tmptop = topnessMinimization1BTag(met,l,b2,b1,version);
  //    if(tmptop<topness) topness = tmptop;
      //cout << "topness " << tmptop << " for a1 " << b2.Pt() << " and b2 " << b1.Pt() << endl;
    }
  }
  if(bjets.size()==0){
    b1.SetPxPyPzE(addjets[0].Px(),addjets[0].Py(),addjets[0].Pz(),addjets[0].E());
    for(unsigned int m = 1; m<addjets.size(); ++m){
      b2.SetPxPyPzE(addjets[m].Px(),addjets[m].Py(),addjets[m].Pz(),addjets[m].E());
      float tmptop = topnessMinimization1BTag(met,l,b1,version);
      if(tmptop<topness) topness = tmptop;
      tmptop = topnessMinimization1BTag(met,l,b2,version);
      if(tmptop<topness) topness = tmptop;
    }
  }
  return topness;
}

//wrapper function with MET and METPhi instead of METlv
float CalcTopness_(int version, float MET, float METphi, LorentzVector lep, vector<LorentzVector> bjets,  vector<LorentzVector> addjets){
  float metx = MET*TMath::Cos(METphi);
  float mety = MET*TMath::Sin(METphi);
  LorentzVector metlv; metlv.SetPxPyPzE(metx,mety,0,MET);
  return CalcTopness(version,metlv,lep,bjets,addjets);
}

//wrapper function with MET and METPhi instead of METlv
float CalcTopness1BTag_(int version, float MET, float METphi, LorentzVector lep, vector<LorentzVector> bjets,  vector<LorentzVector> addjets){
  float metx = MET*TMath::Cos(METphi);
  float mety = MET*TMath::Sin(METphi);
  LorentzVector metlv; metlv.SetPxPyPzE(metx,mety,0,MET);
  return CalcTopness1BTag(version,metlv,lep,bjets,addjets);
}
