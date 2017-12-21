#include <cmath>
#include <iostream>
#include <vector>
#include <functional>

#include "quadrature.h"

using namespace std;

class Midpoint {
private:
  double a, b, s;
  int n;

public:
  function <double(const double)> func;

  Midpoint(const function <double(const double)>& func_, const double a_, const double b_) :
    func(func_), a(a_), b(b_)
  { n = 0; }

  double next() {
    int it, j;
    double x, tnm, sum, del, ddel;
    n++;

    if (n == 1) {
      return  (s = (b - a) * func(0.5 * (a + b)));
    } else {
      for (it = 1, j = 1; j < n - 1; j++) it *= 3;

      tnm = it;
      del = (b - a) / (3 * tnm);
      ddel = del + del;
      x = a + 0.5 * del;
      sum = 0;

      for (j = 0; j < it; j++) {
        sum += func(x);
        x += ddel;
        sum += func(x);
        x += del;
      }
      s = (s + (b - a) * sum / tnm) / 3.0;
      return s;
    }
  }
};

double qromo(const function <double(const double)>& func, const double a, const double b, double err)
{

  const int JMAX=14, JMAXP=JMAX+1, K=5;
  int i,j;
  double ss,dss;
  vector<double> h(JMAXP),s(JMAX),h_t(K),s_t(K);
  Midpoint pt(func, a, b);

  h[0]=1.0;
  for (j=1;j<=JMAX;j++) {

    s[j-1]=pt.next();
    if (j >= K) {

      for (i=0;i<K;i++) {

        h_t[i]=h[j-K+i];
        s_t[i]=s[j-K+i];

      }
      polint(h_t,s_t,0.0,ss,dss);
      if (fabs(dss) <= err*fabs(ss)) return ss;

    }
    h[j]=h[j-1]/9.0;

  }

  throw(quadrature::too_many_steps_val);
  return 0.0;
}
