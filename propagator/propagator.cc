#ifndef _PROPAGATOR_CC
#define _PROPAGATOR_CC
#include<iostream>
#include<cmath>
#include"propagator.h"
using namespace pro;
using std::cout;
using std::endl;
using std::abs;

class G1dfunc : public gfunction {
private:
  double zob, L, nmax;
  double phi_exp(int n, double z, double lambdaD2, int prime, int flag) const {
    double kn;

    if (prime == 0) {
      kn = (n - 0.5) * PI / L;

      //cout<<"kn is "<<kn<<endl;
      if (flag == 0)
        return sin(kn * (L - abs(z)));

      else
        return exp(-lambdaD2 * kn * kn / 4);

    } else {
      kn = n * PI / L;

      if (flag == 0)
        return sin(kn * (L - z));

      else
        return exp(-lambdaD2 * kn * kn / 4);
    }
  }

public:
  G1dfunc(double zob_, double L_, double nmax_): zob(zob_), L(L_), nmax(nmax_) {}
  double operator()(double z, double lambdaD2) const {
    double sum, zn, zita;
    zita = L * L / lambdaD2;
    sum = 0;

    if (zita >= 1) {
      for (int n = -nmax; n <= nmax; n++) {
        zn = 2 * L * n + pow(-1., n) * z;
        sum += pow(-1., n) / sqrt(PI * lambdaD2) * exp(-(zn - zob) * (zn - zob) / lambdaD2);
      }

    } else {
      for (int n = 1; n <= nmax; n++) {
        sum += (phi_exp(n, z, lambdaD2, 0, 1) * phi_exp(n, z, lambdaD2, 0, 0) * phi_exp(n, zob, lambdaD2, 0, 0)
                + phi_exp(n, z, lambdaD2, 1, 1) * phi_exp(n, z, lambdaD2, 1, 0) * phi_exp(n, zob, lambdaD2, 1, 0)) / L;
      }
    }

    return sum;
  }
};

int Green::theta(double x) {
  if (x > 0) return 1;

  else return 0;
}
double Green::phi_exp(int n, double z, double lambdaD2, int prime, int flag) {
  double kn;

  if (prime == 0) {
    kn = (n - 0.5) * PI / L;

    //cout<<"kn is "<<kn<<endl;
    if (flag == 0)
      return sin(kn * (L - abs(z)));

    else
      return exp(-lambdaD2 * kn * kn / 4);

  } else {
    kn = n * PI / L;

    if (flag == 0)
      return sin(kn * (L - z));

    else
      return exp(-lambdaD2 * kn * kn / 4);
  }
}

/*********************************************************************
  The first equation of this function should be right in the whole
  parameter space.

  The effective digit of double float is just 15~16, so the err of
  the largest term is about 1e-15 or more. When the sum of all the terms
  decrease to this level, what we see is just the error of the computer.

  So we replace the first equation with the second one in when lambdaD2
  is large.
 *********************************************************************/
double Green::G1d(double z, double lambdaD2, int flag) {
  double sum, zn;
  sum = 0;

  if (flag == 0) {
    for (int n = -nmax; n <= nmax; n++) {
      zn = 2 * L * n + pow(-1., n) * z;
      sum += pow(-1., n) / sqrt(PI * lambdaD2) * exp(-(zn - zob) * (zn - zob) / lambdaD2);
    }

  } else if (flag == 1) {
    for (int n = 1; n <= nmax; n++) {
      sum += (phi_exp(n, z, lambdaD2, 0, 1) * phi_exp(n, z, lambdaD2, 0, 0) * phi_exp(n, zob, lambdaD2, 0, 0)
              + phi_exp(n, z, lambdaD2, 1, 1) * phi_exp(n, z, lambdaD2, 1, 0) * phi_exp(n, zob, lambdaD2, 1, 0)) / L;
    }
  }

  return sum;
}

double Green::G1d(double z, double lambdaD2) {
  double sum, zn, zita;
  zita = L * L / lambdaD2;
  sum = 0;

  if (zita >= 1) {
    for (int n = -nmax; n <= nmax; n++) {
      zn = 2 * L * n + pow(-1., n) * z;
      sum += pow(-1., n) / sqrt(PI * lambdaD2) * exp(-(zn - zob) * (zn - zob) / lambdaD2);
    }

  } else {
    for (int n = 1; n <= nmax; n++) {
      sum += (phi_exp(n, z, lambdaD2, 0, 1) * phi_exp(n, z, lambdaD2, 0, 0) * phi_exp(n, zob, lambdaD2, 0, 0)
              + phi_exp(n, z, lambdaD2, 1, 1) * phi_exp(n, z, lambdaD2, 1, 0) * phi_exp(n, zob, lambdaD2, 1, 0)) / L;
    }
  }

  return sum;
}

Green::Green(double K0_in, double E0_in, double delta_in, double L_in, double zob_in, int nmax_in) {
  K0 = K0_in;
  E0 = E0_in;
  delta = delta_in;
  L = L_in;
  zob = zob_in;
  nmax = nmax_in;
}

int Green::create_tab() {
  cout << "<<creating table for Green function" << endl;
  G1dfunc g1df(zob, L, nmax);
  double range[4] = {1e-200, 0.1 * K0, 0.97 * L, 4 * pow(5e5, delta) * 2e4 * K0};
  cout << "range is " << range[0] << " " << range[1] << " " << range[2] << " " << range[3] << endl;
  intp.lncreating(&g1df, range, 1);
  cout << "table created" << endl;
  return 0;
}
double Green::G1dq(double z, double lambdaD2) {
  return intp.lnask(abs(z), lambdaD2);
}

#define m_p 0.938
double Green::G(double r, double z, double t, double E) {
  double K, lambdaD2;
  K = E < 25.0 ?
      K0 * pow(E / E0, delta) * E / sqrt(E * E + m_p * m_p) :
      K0 * pow(E / E0, delta);
  //To save the calculation of beta when E is large
  lambdaD2 = 4 * K * t;
  return theta(t) / (PI * lambdaD2) * exp(-r * r / lambdaD2) * G1d(z, lambdaD2);
}

#define ZMIN 1e-300
double Green::Gq(double r, double z, double t, double E) {
  double K, lambdaD2;
  K = K0 * pow(E / E0, delta);
  lambdaD2 = 4 * K * t;
  return theta(t) / (PI * lambdaD2) * exp(-r * r / lambdaD2) * intp.lnask(abs(z) + ZMIN, lambdaD2);
}
#endif // for #ifndef _PROPAGATOR_CC
