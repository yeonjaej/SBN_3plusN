#include "NOMAD.h"

int NOMAD::Init(std::string dataLoc, Oscillator osc, bool debug){

  sinSqDeltaGrid.resize(dm2VecMaxDim, std::vector<double>(30));
  sinSqDeltaGrid2.resize(dm2VecMaxDim, std::vector<double>(30));
  Norm.resize(30);
  SigmaRemu.ResizeTo(30,30);
  Signal.resize(30);
  Observed.resize(30);
  Bkg.resize(30);

  double temp0[] = {0.00547,0.00446,0.00500,0.00523,0.00530,0.00592,0.00555,0.01111,0.01887,0.01051,0.00401,0.00454,0.00459,0.00545,0.00904,0.01519,0.01932,0.02306,0.02040,0.01002,0.00511,0.00488,0.00759,
                  0.01364,0.02365,0.03866,0.03610,0.03885,0.02489,0.01131};
  double temp1[] = {0.00516,0.00467,0.00493,0.00502,0.00489,0.00555,0.00660,0.01007,0.02049,0.01172,0.00489,0.00382,0.00492,0.00561,0.00846,0.01359,0.01899,0.02468,0.02243,0.01126,0.00463,0.00521,0.00724,0.01203,
                  0.02187,0.03343,0.03990,0.03893,0.02270,0.01092};
  for(int i = 0; i < maxEnergyBins * maxRadialBins; i++){
    Observed[i] = temp0[i];
    Bkg[i] = temp1[i];
  }

  double sigmaSystRemu[30][30] = {
      {0.0150,0.0043,0.0045,0.0046,0.0045,0.0051,0.0060,0.0092,0.0187,0.0107,0.0142,0.0035,0.0045,0.0051,0.0077,0.0124,0.0173,0.0225,0.0204,0.0103,0.0134,0.0047,0.0066,0.0110,0.0199,0.0305,0.0364,0.0355,0.0207,0.0100},
      {0.0043,0.0091,0.0041,0.0041,0.0040,0.0046,0.0054,0.0083,0.0169,0.0097,0.0040,0.0074,0.0041,0.0046,0.0070,0.0112,0.0157,0.0204,0.0185,0.0093,0.0038,0.0101,0.0060,0.0099,0.0180,0.0276,0.0329,0.0321,0.0187,0.0090},
      {0.0045,0.0041,0.0092,0.0044,0.0043,0.0048,0.0057,0.0088,0.0178,0.0102,0.0043,0.0033,0.0092,0.0049,0.0074,0.0118,0.0165,0.0215,0.0195,0.0098,0.0040,0.0045,0.0135,0.0105,0.0190,0.0291,0.0347,0.0339,0.0197,0.0095},
      {0.0046,0.0041,0.0044,0.0096,0.0043,0.0049,0.0058,0.0089,0.0182,0.0104,0.0043,0.0034,0.0044,0.0107,0.0075,0.0120,0.0168,0.0219,0.0199,0.0100,0.0041,0.0046,0.0064,0.0229,0.0194,0.0296,0.0354,0.0345,0.0201,0.0097},
      {0.0045,0.0040,0.0043,0.0043,0.0097,0.0048,0.0057,0.0087,0.0177,0.0101,0.0042,0.0033,0.0042,0.0048,0.0168,0.0117,0.0164,0.0213,0.0193,0.0097,0.0040,0.0045,0.0062,0.0104,0.0435,0.0288,0.0344,0.0336,0.0196,0.0094},
      {0.0051,0.0046,0.0048,0.0049,0.0048,0.0135,0.0065,0.0099,0.0201,0.0115,0.0048,0.0037,0.0048,0.0055,0.0083,0.0329,0.0186,0.0242,0.0220,0.0110,0.0045,0.0051,0.0071,0.0118,0.0214,0.0811,0.0391,0.0381,0.0222,0.0107},
      {0.0060,0.0054,0.0057,0.0058,0.0057,0.0065,0.0199,0.0117,0.0239,0.0136,0.0057,0.0044,0.0057,0.0065,0.0099,0.0158,0.0573,0.0287,0.0261,0.0131,0.0054,0.0061,0.0084,0.0140,0.0255,0.0389,0.1204,0.0453,0.0264,0.0127},
      {0.0092,0.0083,0.0088,0.0089,0.0087,0.0099,0.0117,0.0464,0.0364,0.0208,0.0087,0.0068,0.0087,0.0100,0.0150,0.0241,0.0337,0.1137,0.0398,0.0200,0.0082,0.0093,0.0129,0.0214,0.0388,0.0594,0.0709,0.1793,0.0403,0.0194},
      {0.0187,0.0169,0.0178,0.0182,0.0177,0.0201,0.0239,0.0364,0.1749,0.0424,0.0177,0.0138,0.0178,0.0203,0.0306,0.0491,0.0686,0.0892,0.1914,0.0407,0.0167,0.0188,0.0262,0.0435,0.0790,0.1208,0.1442,0.1407,0.1937,0.0395},
      {0.0107,0.0097,0.0102,0.0104,0.0101,0.0115,0.0136,0.0208,0.0424,0.1355,0.0101,0.0079,0.0102,0.0116,0.0175,0.0281,0.0393,0.0510,0.0464,0.1301,0.0096,0.0108,0.0150,0.0249,0.0452,0.0691,0.0825,0.0805,0.0469,0.1263},
      {0.0142,0.0040,0.0043,0.0043,0.0042,0.0048,0.0057,0.0087,0.0177,0.0101,0.0134,0.0033,0.0042,0.0048,0.0073,0.0117,0.0164,0.0213,0.0194,0.0097,0.0127,0.0045,0.0062,0.0104,0.0189,0.0289,0.0344,0.0336,0.0196,0.0094},
      {0.0035,0.0074,0.0033,0.0034,0.0033,0.0037,0.0044,0.0068,0.0138,0.0079,0.0033,0.0061,0.0033,0.0038,0.0057,0.0092,0.0128,0.0166,0.0151,0.0076,0.0031,0.0083,0.0049,0.0081,0.0147,0.0225,0.0269,0.0262,0.0153,0.0074},
      {0.0045,0.0041,0.0092,0.0044,0.0042,0.0048,0.0057,0.0087,0.0178,0.0102,0.0042,0.0033,0.0092,0.0049,0.0073,0.0118,0.0165,0.0214,0.0195,0.0098,0.0040,0.0045,0.0135,0.0104,0.0190,0.0290,0.0346,0.0338,0.0197,0.0095},
      {0.0051,0.0046,0.0049,0.0107,0.0048,0.0055,0.0065,0.0100,0.0203,0.0116,0.0048,0.0038,0.0049,0.0119,0.0084,0.0134,0.0188,0.0244,0.0222,0.0111,0.0046,0.0052,0.0072,0.0256,0.0216,0.0331,0.0395,0.0385,0.0225,0.0108},
      {0.0077,0.0070,0.0074,0.0075,0.0168,0.0083,0.0099,0.0150,0.0306,0.0175,0.0073,0.0057,0.0073,0.0084,0.0291,0.0203,0.0284,0.0368,0.0335,0.0168,0.0069,0.0078,0.0108,0.0180,0.0753,0.0499,0.0596,0.0581,0.0339,0.0163},
      {0.0124,0.0112,0.0118,0.0120,0.0117,0.0329,0.0158,0.0241,0.0491,0.0281,0.0117,0.0092,0.0118,0.0134,0.0203,0.0806,0.0455,0.0592,0.0538,0.0270,0.0111,0.0125,0.0173,0.0288,0.0524,0.1983,0.0956,0.0933,0.0544,0.0262},
      {0.0173,0.0157,0.0165,0.0168,0.0164,0.0186,0.0573,0.0337,0.0686,0.0393,0.0164,0.0128,0.0165,0.0188,0.0284,0.0455,0.1650,0.0827,0.0751,0.0377,0.0155,0.0175,0.0242,0.0403,0.0733,0.1120,0.3466,0.1304,0.0761,0.0366},
      {0.0225,0.0204,0.0215,0.0219,0.0213,0.0242,0.0287,0.1137,0.0892,0.0510,0.0213,0.0166,0.0214,0.0244,0.0368,0.0592,0.0827,0.2786,0.0976,0.0490,0.0201,0.0227,0.0315,0.0524,0.0952,0.1456,0.1737,0.4394,0.0988,0.0476},
      {0.0204,0.0185,0.0195,0.0199,0.0193,0.0220,0.0261,0.0398,0.1914,0.0464,0.0194,0.0151,0.0195,0.0222,0.0335,0.0538,0.0751,0.0976,0.2096,0.0445,0.0183,0.0206,0.0286,0.0476,0.0865,0.1323,0.1579,0.1540,0.2121,0.0432},
      {0.0103,0.0093,0.0098,0.0100,0.0097,0.0110,0.0131,0.0200,0.0407,0.1301,0.0097,0.0076,0.0098,0.0111,0.0168,0.0270,0.0377,0.0490,0.0445,0.1250,0.0092,0.0104,0.0144,0.0239,0.0434,0.0664,0.0792,0.0773,0.0451,0.1213},
      {0.0134,0.0038,0.0040,0.0041,0.0040,0.0045,0.0054,0.0082,0.0167,0.0096,0.0127,0.0031,0.0040,0.0046,0.0069,0.0111,0.0155,0.0201,0.0183,0.0092,0.0120,0.0043,0.0059,0.0098,0.0178,0.0273,0.0326,0.0318,0.0185,0.0089},
      {0.0047,0.0101,0.0045,0.0046,0.0045,0.0051,0.0061,0.0093,0.0188,0.0108,0.0045,0.0083,0.0045,0.0052,0.0078,0.0125,0.0175,0.0227,0.0206,0.0104,0.0043,0.0113,0.0067,0.0111,0.0201,0.0307,0.0367,0.0358,0.0209,0.0100},
      {0.0066,0.0060,0.0135,0.0064,0.0062,0.0071,0.0084,0.0129,0.0262,0.0150,0.0062,0.0049,0.0135,0.0072,0.0108,0.0173,0.0242,0.0315,0.0286,0.0144,0.0059,0.0067,0.0198,0.0154,0.0279,0.0427,0.0509,0.0497,0.0290,0.0139},
      {0.0110,0.0099,0.0105,0.0229,0.0104,0.0118,0.0140,0.0214,0.0435,0.0249,0.0104,0.0081,0.0104,0.0256,0.0180,0.0288,0.0403,0.0524,0.0476,0.0239,0.0098,0.0111,0.0154,0.0549,0.0464,0.0710,0.0847,0.0826,0.0482,0.0232},
      {0.0199,0.0180,0.0190,0.0194,0.0435,0.0214,0.0255,0.0388,0.0790,0.0452,0.0189,0.0147,0.0190,0.0216,0.0753,0.0524,0.0733,0.0952,0.0865,0.0434,0.0178,0.0201,0.0279,0.0464,0.1945,0.1290,0.1539,0.1502,0.0876,0.0421},
      {0.0305,0.0276,0.0291,0.0296,0.0288,0.0811,0.0389,0.0594,0.1208,0.0691,0.0289,0.0225,0.0290,0.0331,0.0499,0.1983,0.1120,0.1456,0.1323,0.0664,0.0273,0.0307,0.0427,0.0710,0.1290,0.4880,0.2353,0.2296,0.1339,0.0644},
      {0.0364,0.0329,0.0347,0.0354,0.0344,0.0391,0.1204,0.0709,0.1442,0.0825,0.0344,0.0269,0.0346,0.0395,0.0596,0.0956,0.3466,0.1737,0.1579,0.0792,0.0326,0.0367,0.0509,0.0847,0.1539,0.2353,0.7283,0.2740,0.1598,0.0769},
      {0.0355,0.0321,0.0339,0.0345,0.0336,0.0381,0.0453,0.1793,0.1407,0.0805,0.0336,0.0262,0.0338,0.0385,0.0581,0.0933,0.1304,0.4394,0.1540,0.0773,0.0318,0.0358,0.0497,0.0826,0.1502,0.2296,0.2740,0.6933,0.1559,0.0750},
      {0.0207,0.0187,0.0197,0.0201,0.0196,0.0222,0.0264,0.0403,0.1937,0.0469,0.0196,0.0153,0.0197,0.0225,0.0339,0.0544,0.0761,0.0988,0.2121,0.0451,0.0185,0.0209,0.0290,0.0482,0.0876,0.1339,0.1598,0.1559,0.2147,0.0437},
      {0.0100,0.0090,0.0095,0.0097,0.0094,0.0107,0.0127,0.0194,0.0395,0.1263,0.0094,0.0074,0.0095,0.0108,0.0163,0.0262,0.0366,0.0476,0.0432,0.1213,0.0089,0.0100,0.0139,0.0232,0.0421,0.0644,0.0769,0.0750,0.0437,0.1177}};

// Statistical Errors
  double sigmaStatRemu[] = {0.00077,0.00063,0.00062,0.00053,0.00052,0.00057,0.00061,0.00078,0.00085,0.00103,0.00050,0.00049,0.00046,0.00047,0.00070,0.00113,0.00152,0.00136,0.00077,0.00089,0.00056,
                  0.00056,0.00073,0.00106,0.00180,0.00272,0.00287,0.00221,0.00093,0.00111};

  // Put the errors together in a matrix
  TMatrix sigmaRemu_inv(30,30);
  for(int i = 0; i < 30; i ++){
    for(int j = 0; j < 30; j++){
      if(i == j)
        sigmaRemu_inv(i,j) = sigmaSystRemu[i][j] + pow(sigmaStatRemu[i],2) * pow(10,5);
      else
        sigmaRemu_inv(i,j) = sigmaSystRemu[i][j];
    }
  }
  SigmaRemu = sigmaRemu_inv.Invert();
  for(int i = 0; i < 30; i++){
    for(int j = 0; j < 30; j++){
      SigmaRemu[i][j] *= pow(10,5);
    }
  }

  double fullMixHighDm2Remu[] = {0.4406,0.4327,0.4055,0.4073,0.3979,0.3968,0.4030,0.3823,0.3636,0.3427,0.4252,0.3991,0.4041,0.3914,0.4028,0.3887,0.3771,0.3777,0.3605,0.3186,0.4292,0.4187,0.4138,0.4075,
                  0.3976,0.4013,0.4003,0.3795,0.3797,0.2960};

  // First, find the norm
  for(int i = 0; i < 30; i++)
    Norm[i] = 0;

  for(int i = 0; i < maxEnergyBins; i++){
    double EMin = energyMin[i];
    double EMax = energyMax[i];
    for(int j = 0; j < maxRadialBins; j++){
      int iN = maxEnergyBins*j + i;
      double res = (l2 - l1) * (EMax - EMin);
      Norm[iN] = 2. * fullMixHighDm2Remu[iN] / res;
    }
  }

  // Now, fill the sinsq grid vectors
  integralFuncsNomad integralFuncs;
  for(int i = 0; i < maxEnergyBins; i++){
    double EMin = energyMin[i];
    double EMax = energyMax[i];
    integralFuncs._EnuAvg = (energyMax[i] + energyMin[i])/2.;
    for(int j = 0; j < maxRadialBins; j++){
      int iN = maxEnergyBins*j + i;
      for(int k = 0; k < dm2VecMaxDim; k++){
        integralFuncs._dm2 = osc.dm2Vec[k];

        ROOT::Math::Functor1D wf1(&integralFuncs, &integralFuncsNomad::sinSqFunction);
        ROOT::Math::Functor1D wf2(&integralFuncs, &integralFuncsNomad::sinSqFunctionCPV);
        ROOT::Math::GSLIntegrator ig(ROOT::Math::IntegrationOneDim::kADAPTIVE);
        ig.SetAbsTolerance(1*pow(10,-4));
        ig.SetRelTolerance(1*pow(10,-4));

        ig.SetFunction(wf1);
        sinSqDeltaGrid[k][iN] = ig.Integral(EMin, EMax);// (EMax - EMin);
        ig.SetFunction(wf2);
        sinSqDeltaGrid2[k][iN] = ig.Integral(EMin, EMax);// (EMin - EMax);
      }
    }
  }

  dof = maxEnergyBins * maxRadialBins;

  //Initialize output tree
  chi2Nt = new OutTree("NOMAD");

  if(debug) std::cout << "NOMAD initialized. Bins: " << maxEnergyBins*maxRadialBins << std::endl;

  return dof;
}

float NOMAD::Chi2(Oscillator osc, neutrinoModel model, bool debug){

  float chi2 = 0.f;

  oscContribution oscCont;

  ROOT::Math::Interpolator dif(dm2VecMaxDim);
  double numerator, denominator, sinSq, sinSq2;
  double sinSqDeltaVec[dm2VecMaxDim], sinSqDeltaVec2[dm2VecMaxDim];

  // Loop over energy bins
  for(int ii = 0; ii < maxEnergyBins; ii++){
    // Loop over radial bins
    for(int jj = 0; jj < maxRadialBins; jj++){
      int iN = maxEnergyBins*jj + ii;

      for(int k = 0; k < dm2VecMaxDim; k++){
        sinSqDeltaVec[k] = sinSqDeltaGrid[k][iN];
        sinSqDeltaVec2[k] = sinSqDeltaGrid2[k][iN];
      }

      // First, let's do Nue appearance
      numerator = 0.;
      oscCont = getOscContributionsNueApp(model, false, true);

      for(int iContribution = 0; iContribution < 6; iContribution++){
        // Now, add the latest contribution to the predicted signal vector

        dif.SetData(dm2VecMaxDim,osc.dm2Vec,sinSqDeltaVec);
        if(oscCont.dm2[iContribution] == 0.)    sinSq = 0;
        else    sinSq = dif.Eval(oscCont.dm2[iContribution]);
        dif.SetData(dm2VecMaxDim,osc.dm2Vec,sinSqDeltaVec2);
        if(oscCont.dm2[iContribution] == 0.)    sinSq2 = 0;
        else    sinSq2 = dif.Eval(oscCont.dm2[iContribution]);

        numerator += oscCont.aMuE[iContribution]*sinSq + oscCont.aMuE_CPV[iContribution] * sinSq2;
      }
  		//std::cout << "Numerator: " << numerator << std::endl;

      // Now, muon disappearance
      denominator = (l2 - l1) * (energyMax[ii] - energyMin[ii]);
      oscCont = getOscContributionsNumuDis(model);

      for(int iContribution = 0; iContribution < 6; iContribution++){
        // Now, add the latest contribution to the predicted signal vector
        dif.SetData(dm2VecMaxDim,osc.dm2Vec,sinSqDeltaVec);
        if(oscCont.dm2[iContribution] == 0.)    sinSq = 0;
        else sinSq = dif.Eval(oscCont.dm2[iContribution]);
        denominator += oscCont.aMuMu[iContribution]*sinSq;
      }
      if(denominator < 0) std::cout << "Got a problem with NOMAD. Denominator is negative!" << std::endl;
  		//std::cout << "Denominator: " << denominator << std::endl;

  		// Electron to muon ratio
      Signal[iN] = Norm[iN] * (numerator / denominator);
    }
  }

  for(int i = 0; i < 30; i++){
    for(int j = 0; j < 30; j++){
      if(i == j)
        chi2 += (Observed[i] - (Bkg[i] + Signal[i])) * SigmaRemu[i][j] * (Observed[j] - (Bkg[j] + Signal[j]));
    }
  }

  // Fill output tree
  chi2Nt->Fill(chi2,dof,model);

  if(debug)
    std::cout << "NOMAD Chi2: " << chi2 << std::endl;
  return chi2;
}

// Integral functions

double integralFuncsNomad::sinSqFunction(const double x){
  double l1 = .422; double l2 = .837; double baseline = (l1 + l2)/2;

  double a = 1.27 * _dm2;
  double arg1 = a * l1 / x;
  double arg2 = a * l2 / x;
  double num = (TMath::Pi() / 2.) * (1. / (1.27 * _dm2 * baseline)) * pow(_EnuAvg,2);
  double den = 1. + (TMath::Pi() / 2.) * (1. / (1.27 * _dm2 * baseline)) * _EnuAvg;
  double deltaEnu = num / den;
  // Check if oscillations are very fast relative to energy resolution
  if(deltaEnu >= .032 * sqrt(_EnuAvg) + .01 * _EnuAvg){
    double term;
    if(a > 0)
      term = .5 * (1. - .5 * (sin(2. * arg1) - sin(2. * arg2)) / (arg1 - arg2)) * (l2 - l1);
    else term = 0.;
    return term;
  }
  else return .5 * (l2 - l1);
}

double integralFuncsNomad::sinSqFunctionCPV(const double x){
  double l1 = .422; double l2 = .837; double baseline = (l1 + l2)/2;

  double a = 1.27 * _dm2;
  double arg1 = a * l1 / x;
  double arg2 = a * l2 / x;
  double num = (TMath::Pi() / 2.) * (1. / (1.27 * _dm2 * baseline)) * pow(_EnuAvg,2);
  double den = 1. + (TMath::Pi() / 2.) * (1. / (1.27 * _dm2 * baseline)) * _EnuAvg;
  double deltaEnu = num / den;
  // Check if oscillations are very fast relative to energy resolution
  if(deltaEnu >= .032 * sqrt(_EnuAvg) + .01 * _EnuAvg){
    double term;
    if(a > 0)
      term = x / (2. * a) * (cos(2. * arg1) - cos(2. * arg2));
    else term = 0.;
    return term;
  }
  else return 0.;
}
