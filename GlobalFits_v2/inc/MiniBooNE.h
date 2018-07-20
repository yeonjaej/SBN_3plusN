#ifndef MINIBOONE_H
#define MINIBOONE_H

#include "datasets.h"

class MiniBooNE: public dataset{
  public:
    MiniBooNE(bool _nubar){
      nubar = _nubar;
    }
    int Init(std::string dataLoc, bool debug);
    float Chi2(Oscillator osc, neutrinoModel nu, bool debug);

  private:
    bool nubar;
    const short nBins_e = 11;
    const short nBins_mu = 8;
    const int nFOscEvts = 17204;

    std::vector < float > Prediction, Signal, FullData;
    std::vector < std::vector < float > > Full_fractCovMatrix, Lib_sin, Lib_sinsq;

    TMatrixT <float> covMatrix, full_covMatrix, cov;
};

#endif
