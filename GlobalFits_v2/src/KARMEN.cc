#include "KARMEN.h"

int KARMEN::Init(std::string dataLoc, bool debug){

  std::vector < std::vector < double > > sinSqDeltaGrid, sinSqDeltaGrid2;
  sinSqDeltaGrid.resize(dm2VecMaxDim, std::vector<double>(nBins));
  sinSqDeltaGrid2.resize(dm2VecMaxDim, std::vector<double>(nBins));

  double observed[] = {3.,4.,1.,3.,3.,1.,0.,0.,0.};
  double bkg[] = {3.4,3.7,3.4,2.3,1.1,0.8,0.6,0.4,0.1};

  double EMin = 16.; double EMax = 52.;
  double energyMin[nBins], energyMax[nBins];
  for(int iK = 0; iK < nBins; iK++){
      energyMin[iK] = EMin + (double(iK)/double(nBins)) * (EMax - EMin);
      energyMax[iK] = EMin + (double(iK + 1)/double(nBins)) * (EMax - EMin);
  }

  // Now, let's find the KARMEN integrals!
  double fullMixHighDm2KarmenTotal = 2913.;
  double sum = 0.;
  integralFuncsKarmen integralFuncs;

  for(int iK = 0; iK < nBins; iK++){
      EMin = energyMin[iK];
      EMax = energyMax[iK];

      ROOT::Math::Functor1D wf(&integralFuncs,&integralFuncsKarmen::normFunc);
      ROOT::Math::GSLIntegrator ig(ROOT::Math::IntegrationOneDim::kADAPTIVE);
      ig.SetAbsTolerance(1*pow(10,-4));
      ig.SetRelTolerance(1*pow(10,-4));
      ig.SetFunction(wf);
      sum += ig.Integral(EMin,EMax);
  }
  pack.norm = fullMixHighDm2KarmenTotal / sum;

  for(int k = 0; k < dm2VecMaxDim; k++){
      for(int iK = 0; iK < nBins; iK++){
          integralFuncs._dm2 = dm2Vec[k];
          EMin = energyMin[iK];
          EMax = energyMax[iK];

          ROOT::Math::Functor1D wf1(&integralFuncs, &integralFuncsKarmen::sinSqFunction);
          ROOT::Math::Functor1D wf2(&integralFuncs, &integralFuncsKarmen::sinSqFunctionCPV);
          ROOT::Math::GSLIntegrator ig(ROOT::Math::IntegrationOneDim::kADAPTIVE);
          ig.SetAbsTolerance(1*pow(10,-4));
          ig.SetRelTolerance(1*pow(10,-4));
          ig.SetFunction(wf1);
          pack.sinSqDeltaGrid[k][iK] = ig.Integral(EMin, EMax);
          ig.SetFunction(wf2);
          pack.sinSqDeltaGrid2[k][iK] = ig.Integral(EMin, EMax);
      }
  }
  ndf += nBins;
  if(debug) std::cout << "Karmen initialized. Bins: " << nBins << std::endl;
  return pack;




}


// KARMEN integral functions
double KARMEN::sinSqFunction(const double x){

  double baseline = 17.7;
  double deltaL = 1.5;
  double l1 = baseline - deltaL; double l2 = baseline + deltaL;
  double fitPar[4] = {0.778,2.96,4.39,51.5};
  double positronSpectrum;

  if(x < fitPar[3])
    positronSpectrum = fitPar[0] * pow(fitPar[3] - x,fitPar[1]) * exp(-(fitPar[3] - x)/fitPar[2]);
  else
    positronSpectrum = 0.;

  double Enu = x + 1.8;   // Add 1.8 to positron energy
  double a = 1.27 * _INTdm2;
  double n = 1.27 * _INTdm2 * baseline / (Enu * TMath::Pi());
  double EnuNext = 1.27 * _INTdm2 * baseline / ((n + .5) * TMath::Pi());

  // Check if the oscillations are faster than the energy resolution can keep up with
  if( abs(Enu - EnuNext) >= 0.115 * sqrt(x)){
    double num1 = Enu - Enu * cos(2. * a * l1 / Enu) - 2. * a * l1 * sineInt(2. * a * l1 / Enu);
    double num2 = Enu - Enu * cos(2. * a * l2 / Enu) - 2. * a * l2 * sineInt(2. * a * l2 / Enu);
    double den1 = 2. * Enu * l1;  double den2 = 2. * Enu * l2;
    double term1 = num1 / den1;    double term2 = num2 / den2;

  	return (term1 - term2) * positronSpectrum;
  }
  else return .5 * positronSpectrum * (l2 - l1) / (l1 * l2);
}

double KARMEN::sinSqFunctionCPV(const double x){
  double baseline = 17.7;
  double deltaL = 1.5;
  double l1 = baseline - deltaL; double l2 = baseline + deltaL;
  double fitPar[4] = {0.778,2.96,4.39,51.5};
  double positronSpectrum;

  if(x < fitPar[3])
    positronSpectrum = fitPar[0] * pow(fitPar[3] - x,fitPar[1]) * exp(-(fitPar[3] - x)/fitPar[2]);
  else
    positronSpectrum = 0.;

  double Enu = x + 1.8;   // Add 1.8 to positron energy
  double a = 1.27 * _INTdm2;
  double n = 1.27 * _INTdm2 * baseline / (Enu * TMath::Pi());
  double EnuNext = 1.27 * _INTdm2 * baseline / ((n + .5) * TMath::Pi());

  // Check if the oscillations are faster than the energy resolution can keep up with
  if( abs(Enu - EnuNext) > 0.115 * sqrt(x)){
    double num1 = 2. * Enu * sin(2. * a * l1 / Enu) - 4. * a * l1 * cosineInt(2. * a * l1 / Enu);
    double num2 = 2. * Enu * sin(2. * a * l2 / Enu) - 4. * a * l2 * cosineInt(2. * a * l2 / Enu);
    double den1 = 2. * Enu * l1;  double den2 = 2. * Enu * l2;
    double term1 = num1 / den1;    double term2 = num2 / den2;

    return (term1 - term2) * positronSpectrum;
  }
  else
    return 0;
}

double KARMEN::normFunc(const double x){
  double baseline = 17.7;
  double deltaL = 1.5;
  double fitPar[4] = {0.778, 2.96, 4.39, 51.5};
  double positronSpectrum;

  if(x < fitPar[3])
    positronSpectrum = fitPar[0] * pow(fitPar[3] - x, fitPar[1]) * exp(-(fitPar[3] - x)/fitPar[2]);
  else
    positronSpectrum = 0.;

  return 0.5 * positronSpectrum * (2. * deltaL) / ((baseline + deltaL) * (baseline - deltaL));
}
