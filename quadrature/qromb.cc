#include<iostream>
#include <cmath>
#include<functional>
#include <vector>

#include "quadrature.h"

using namespace std;

class DErule {
private:
  double a, b, hmax, s;
  int n;
  bool a_delta_exist, b_delta_exist;
  function <double(const double)> func, func_a_delta, func_b_delta;
  // The func_a_delta() and func_b_delta are adopted to precisely calculate the singualrity
  // around the boundary a and b: func_a_delta(delta) = func(a + delta) and func_b_delta(delta) = func(b - delta)

public:
  DErule(const function <double(const double)>& func_, double a_, double b_, double hmax_ = 4.3)
    : func(func_), a(a_), b(b_), hmax(hmax_), s(0), n(0), a_delta_exist(false), b_delta_exist(false) {}

  void set_func_a(const function <double(const double)>& func_a_delta_) { func_a_delta = func_a_delta_; a_delta_exist = true; }
  void set_func_b(const function <double(const double)>& func_b_delta_) { func_b_delta = func_b_delta_; b_delta_exist = true; }

  double next() {
    double del, fact, q, sum, t, twoh;
    int it, j;
    n++;

    if (n == 1) {
      fact = 0.25;
      return s = hmax * 2.0 * (b - a) * fact * func(0.5 * (b + a));
    }

    for (it = 1, j = 1; j < n - 1; j++) it <<= 1;
    twoh = hmax / it;
    t = 0.5*twoh;

    for (sum = 0, j = 0; j < it; j++) {
      q = exp(-2 * sinh(t));
      del = (b - a) * q / (1 + q);
      fact = q / (1 + q) / (1 + q) * cosh(t);
      double result_a = a_delta_exist ? func_a_delta(del) : func(a + del),
             result_b = b_delta_exist ? func_b_delta(del) : func(b - del);
      sum += fact * (result_a + result_b);
      t += twoh;
    }

    return s = 0.5 * s + (b - a) * twoh * sum;
  }
};

double qromb(const function <double(const double)>& func, double a, double b, double err)
{

  const int JMAX=20, JMAXP=JMAX+1, K=5;
  double ss,dss;
  vector<double> s(JMAX),h(JMAXP),s_t(K),h_t(K);
  int i,j;

  DErule derule(func, a, b);

  h[0]=1.0;
  for (j=1;j<=JMAX;j++) {

    s[j-1]=derule.next();
    if (j >= K) {

      for (i=0;i<K;i++) {
        h_t[i]=h[j-K+i];
        s_t[i]=s[j-K+i];
      }

      polint(h_t,s_t,0.0,ss,dss);
      if (fabs(dss) <= err*fabs(ss)) return ss;

    }
    h[j]=0.25*h[j-1];

  }

  cerr << "#sum: " << ss << " err: " << dss << endl;
  throw(quadrature::too_many_steps_val);

  return 0.0;
}
