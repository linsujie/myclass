#include<iostream>
#include<cmath>
#include"propagator.h"
using namespace pro;
using std::cout;
using std::endl;
using std::abs;

/*********************************************************************
  The Green function in 0809.5268
*********************************************************************/

class G1dfunc {
private:
  double zob, L, nmax;

public:
  G1dfunc(double zob_, double L_, double nmax_): zob(zob_), L(L_), nmax(nmax_) {}

  double operator ()(double z, double lambdaD2) const{
    double sum, zn, zita, pn, kn, knprime,
           sqpilam = sqrt(PI * lambdaD2);
    zita = L * L / lambdaD2;
    sum = 0;

    if (zita >= 1) {
      for (int n = -nmax; n <= nmax; n++) {
        pn = (n % 2 ? -1 : 1);
        zn = 2 * L * n + pn * z;
        sum += pn / sqpilam * exp(-(zn - zob) * (zn - zob) / lambdaD2);
      }

    } else {
      for (int n = 1; n <= nmax; n++) {
        kn = (n - 0.5) * PI / L;
        knprime = n * PI / L;

        sum += (exp(-lambdaD2 * kn * kn / 4) * sin(kn * (L - abs(zob))) * sin(kn * (L - abs(z)))
                + exp(-lambdaD2 * knprime * knprime / 4) * sin(knprime * (L - zob)) * sin(knprime * (L - z))) / L;
      }
    }

    return sum;
  }
};

int Green::theta(double x) const {
  if (x > 0) return 1;

  else return 0;
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

double Green::G1d(double z, double lambdaD2) const{
  double sum, zn, zita, pn, kn, knprime,
         sqpilam = sqrt(PI * lambdaD2);
  zita = L * L / lambdaD2;
  sum = 0;

  if (zita >= 1) {
    for (int n = -nmax; n <= nmax; n++) {
      pn = (n % 2 ? -1 : 1);
      zn = 2 * L * n + pn * z;
      sum += pn / sqpilam * exp(-(zn - zob) * (zn - zob) / lambdaD2);
    }

  } else {
    for (int n = 1; n <= nmax; n++) {
      kn = (n - 0.5) * PI / L;
      knprime = n * PI / L;

      sum += (exp(-lambdaD2 * kn * kn / 4) * sin(kn * (L - abs(zob))) * sin(kn * (L - abs(z)))
              + exp(-lambdaD2 * knprime * knprime / 4) * sin(knprime * (L - zob)) * sin(knprime * (L - z))) / L;
    }
  }

  return sum;
}

Green::Green() {}
Green::Green(double K0_, double E0_, double delta_, double L_, double zob_, int nmax_): K0(K0_),
  E0(E0_), delta(delta_), L(L_), zob(zob_), nmax(nmax_) {}

int Green::create_tab(double precision) {
  cout << "<<creating table for Green function" << endl;
  G1dfunc g1df(zob, L, nmax);
  double range[4] = {1e-200, 0.1 * K0, 0.97 * L, 4 * pow(5e5, delta) * 2e4 * K0};
  cout << "range is " << range[0] << " " << range[1] << " " << range[2] << " " << range[3] << endl;
  intp.lncreating(g1df, range, precision);
  intp.del_map();
  cout << "table created" << endl;
  return 0;
}
double Green::G1dq(double z, double lambdaD2) const {
  return intp.lnask(abs(z), lambdaD2);
}

#define m_p 0.938
double Green::G(double r, double z, double t, double E) const {
  double K, lambdaD2;
  K = E < 25.0 ?
      K0 * pow(E / E0, delta) * E / sqrt(E * E + m_p * m_p) :
      K0 * pow(E / E0, delta);
  //beta would be almost 1 when E is large
  lambdaD2 = 4 * K * t;

  return theta(t) / (PI * lambdaD2) * exp(-r * r / lambdaD2) * G1d(z, lambdaD2);
}

#define ZMIN 1e-300
double Green::Gq(double r, double z, double t, double E) const {
  double K, lambdaD2;
  K = E < 25.0 ?
      K0 * pow(E / E0, delta) * E / sqrt(E * E + m_p * m_p) :
      K0 * pow(E / E0, delta);
  //beta would be almost 1 when E is large
  lambdaD2 = 4 * K * t;
  return theta(t) / (PI * lambdaD2) * exp(-r * r / lambdaD2) * intp.lnask(abs(z) + ZMIN, lambdaD2);
}

int Green::print() const {
  cout << "K0: " << K0 << endl
    << "E0: " << E0 << endl
    << "delta: " << delta << endl
    << "L: " << L << endl
    << "zob: " << zob << endl;

  if (intp.lntab.xaxis.size()) {
    Table2D::TabConsIter fiter = intp.lntab.value.begin(),
      siter = intp.lntab.value.end();
    siter--;

    cout  << "intp with: " << endl
      << "  lntab.xaxis: " << intp.lntab.xaxis.begin()->first << "... (size " << intp.lntab.xaxis.size() << ")" << endl
      << "  lntab.yaxis: " << intp.lntab.yaxis.begin()->first << "... (size " << intp.lntab.yaxis.size() << ")" << endl
      << "  lntab.value: " << fiter->second.begin()->second << "...  " << endl
      << "               ..." << endl
      << "               " << siter->second.begin()->second << "... " << endl;
  }

  return 0;
}
