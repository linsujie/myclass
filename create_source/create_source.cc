#include<cstdlib>
#include<iostream>
#include<cmath>
#include"create_source.h"

using namespace pro;
source::source(double Es_, double Ee_, double factor_, double nu_nuc_, double sigma_nu_, double A0_, double sigma_A_):
  Es(Es_), Ee(Ee_), factor(factor_),
  nu_nuc(nu_nuc_), sigma_nu(sigma_nu_),
  sigma_A(sigma_A_), A0(A0_) {
  Q.ini(Es, Ee, factor);
}

/*********************************************************************
Generating a number acording to distribution q(z)=exp(-|z|/Z_s)/Z_s
*********************************************************************/
double source::get_z() {
  int sign;
  double Z_s = 0.2;

  if (rand() / double(RAND_MAX) > 0.5) {
    sign = 1;

  } else {
    sign = -1;
  }

  return sign * Z_s * log(1 - rand() / double(RAND_MAX));
}

/*********************************************************************
Generating a number acording to Q(r)=r*q(r)
where q(r)=(r/r_sun)^alpha*exp(-beta*(r-r_sun)/r_sun)
with a cutoff in r>15kpc
*********************************************************************/
inline double source::Qr(double r, double r_sun, double alpha, double beta) {
  return pow(r, alpha + 1) * exp(-beta * r / r_sun);
}
double source::get_r() {
  long double r_sun = 8.33,
              r_max = 15,
              alpha = 1.25,
              beta = 3.56;
  long double ran_num;
  long double r_peak = (alpha + 1) / beta * r_sun,
              Q_max = Qr(r_peak, r_sun, alpha, beta);
  int mark = 0;

  while (mark == 0) {
    ran_num = rand() / double(RAND_MAX) * r_max;

    if (rand() / double(RAND_MAX)*Q_max < Qr(ran_num, r_sun, alpha, beta)) {
      mark = 1;
    }
  }

  return ran_num;
}

/*********************************************************************
Generating a number acording to Gaussian distribution
*********************************************************************/
double source::gaussrand(double x0, double sigma) {
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

double source::get_nu() {
  double result = gaussrand(nu_nuc, sigma_nu);

  while (result < 1.5 || result > 4) {
    result = gaussrand(nu_nuc, sigma_nu);
  }

  return result;
}

/*********************************************************************
normalize the inject flux by the totle energy, assuming the max envergy
is 100TeV
 *********************************************************************/
#define EMIN 1
#define EMAX 100000
int source::create_injection(double delta) {
  double A = -1;

  while (A < 0) A = gaussrand(A0, sigma_A);

  double norm = (delta != 1.) ? (pow(EMAX, -delta + 2) - pow(EMIN, -delta + 2)) / (-delta + 2)  : log(EMAX / EMIN); //E^(-delta+2)/(-delta+2) or ln(EMAX/EMIN)

  for (unsigned i = 0; i < Q.E.size(); i++) {
    Q.F[i] = pow(Q.E[i], -delta) * A / norm;
  }

  return 0;
}

#define PI 3.14159265358
int source::create_source() {
  z = get_z();
  r = get_r();
  theta = rand() / double(RAND_MAX) * 2 * PI;
  double nu = get_nu();
  create_injection(nu);
  return 0;
  //Q->print();
}

/*********************************************************************
int source::test_ran(double (source::*ranfun)(),double range_a,double range_b,int n_bin,int num){
  int F[n_bin];
  double l_bin=(range_b-range_a)/n_bin;
  double ran_num;
  for(int i=0;i<n_bin;i++) F[i]=0;
  for(int i=1;i<=num;i++){
    ran_num=(this->*ranfun)();
    for(int j=0;j<n_bin;j++){
      if(ran_num>range_a+j*l_bin && ran_num<range_a+(j+1)*l_bin)
        F[j]++;
    }
  }

  double norm=0;
  for(int i=0;i<n_bin;i++){
    norm+=l_bin*Qr(range_a+i*l_bin,8.33,1.25,3.56);
  }

  for(int j=0;j<n_bin;j++){
    double E=range_a+j*l_bin;
    cout<<E<<" "<<F[j]<<" "<<F[j]/l_bin/num<<" "<<exp(-pow(E,2)/2)/sqrt(3.1415926*2)<<endl;
  }
}
*********************************************************************/
