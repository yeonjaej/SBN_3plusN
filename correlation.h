#ifndef CORRELATION_H_
#define CORRELATION_H_

#include <cmath>
#include <vector>
#include "TMatrixT.h"
#include <iostream>
#include "model.h"
TMatrixT<double> sys_fill_direct(int dim, bool detsys);

void sys_fill(TMatrixT <double> & M, bool detsys);
void sys_fill2(int dim, TMatrixT<float> * ans);
void fake_fill(TMatrixT <double>&  M);
void stats_fill(TMatrixT <double>&  M, std::vector<double> diag);

std::vector<double >  calc_signal_events(struct neutrinoModel &nuModel);


void contract_signal(TMatrixT <double> & M, TMatrixT <double> &Mc);
void contract_signal2(TMatrixT <double> & M, TMatrixT <double> &Mc);
void contract_signal2_anti(TMatrixT <double> & M, TMatrixT <double> &Mc);
std::vector<std::vector<double >> to_vector(TMatrixT <double> McI);

void contract_signal_layer1(TMatrixT <double> & M, TMatrixT <double> & Mc);
void contract_signal_layer2(TMatrixT <double> & M, TMatrixT <double> & Mc);
void contract_signal_layer3(TMatrixT <double> & M, TMatrixT <double> & Mc);

class correlation_matrix 
{
	


};



#endif
