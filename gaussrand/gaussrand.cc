#include<iostream>
#include<cmath>
#include<cstdlib>
#include<map>
#include"gaussrand.h"

using std::map;
using std::cout;
using std::endl;
double gaussrand(double x0, double sigma) {
  static double gauss_V1, gauss_V2, gauss_S;
  static int phase = 0;
  double X;

  if (phase == 0) {
    do {
      double U1 = (double)rand() / RAND_MAX;
      double U2 = (double)rand() / RAND_MAX;
      gauss_V1 = 2 * U1 - 1;
      gauss_V2 = 2 * U2 - 1;
      gauss_S = gauss_V1 * gauss_V1 + gauss_V2 * gauss_V2;
    } while (gauss_S >= 1 || gauss_S == 0);

    X = gauss_V1 * sqrt(-2 * log(gauss_S) / gauss_S);

  } else
    X = gauss_V2 * sqrt(-2 * log(gauss_S) / gauss_S);

  phase = 1 - phase;
  return X * sigma + x0;
}

void testrand(double func(), double x_min, double x_max, double x_step, unsigned n_sample) {
  unsigned binnum = (x_max - x_min) / x_step;
  map <double, unsigned> bin;
  for (unsigned i_bin = 0; i_bin < binnum - 1; i_bin++) bin[x_min + i_bin * x_step] = 0;

  for(unsigned i_sample = 0; i_sample < n_sample; i_sample++) bin.lower_bound(func()) -> second += 1;

  for (map <double, unsigned>::const_iterator it = bin.begin(); it != bin.end(); it++)
    cout << it->first << "\t" << double(it->second) / n_sample << endl;
}
