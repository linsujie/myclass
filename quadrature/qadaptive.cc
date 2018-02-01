#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <functional>

#include "quadrature.h"

using namespace std;

class qGaus {
private:
  const static vector<double> xs, wsG7, wsK15;

  function <double(const double)> func;
  vector<double> func_value;
  double length;
  int nmid;

public:
  double s;

  qGaus(const function <double(const double)>& func_): func(func_) { func_value.resize(xs.size()); }

  void calc(double low, double up) {
    length = up - low;
    double xmid = (low + up) / 2;
    func_value[0] = func(xmid);

    for (unsigned i = 1; i < xs.size(); i++) {
      double xdelta = length / 2 * xs[i];
      func_value[i] = func(xmid - xdelta) + func(xmid + xdelta);
    }
  }

  double qG7() const {
    double sum = 0;
    for (unsigned i = 0; i < xs.size(); i++)
      if (wsG7[i] != 0) sum += wsG7[i] * func_value[i];

    return sum / 2 * length;
  }

  double qK15() const {
    double sum = 0;
    for (unsigned i = 0; i < xs.size(); i++)
      sum += wsK15[i] * func_value[i];

    return sum / 2 * length;
  }
};

const vector<double> qGaus::xs =    { 0,                 0.207784955007898, 0.405845151377397, 0.586087235467691, 0.741531185599394, 0.864864423359769, 0.949107912342759, 0.991455371120813 };
const vector<double> qGaus::wsG7 =  { 0.417959183673469, 0,                 0.381830050505119, 0,                 0.279705391489277, 0,                 0.129484966168870, 0 };
const vector<double> qGaus::wsK15 = { 0.209482141084728, 0.204432940075298, 0.190350578064785, 0.169004726639267, 0.140653259715525, 0.104790010322250, 0.063092092629979, 0.022935322010529 };

double qiter(qGaus& q, double low, double up, double err) {
  q.calc(low, up);
  double vG7 = q.qG7(),
         vK15 = q.qK15();

  if (fabs(vK15 - vG7) <= err * fabs(vK15)) return vK15;

  return qiter(q, low, (low + up) / 2, err) + qiter(q, (low + up) / 2, up, err);
}

double qadaptive(const function <double(const double)>& func, double low, double up, double err) {
  qGaus q(func);

  return qiter(q, low, up, err);
}

double qiter_positive(qGaus& q, double low, double up, double vG7, double vK15, double err) {
  if (fabs(vK15 - vG7) <= err * fabs(vK15) || fabs(vK15) < err * q.s) return vK15;

  q.s -= vK15;

  q.calc(low, (low + up) / 2);
  double vG7low = q.qG7(),
         vK15low = q.qK15();

  q.calc((low + up) / 2, up);
  double vG7up = q.qG7(),
         vK15up = q.qK15();

  q.s += vK15low + vK15up;

  return qiter_positive(q, low, (low + up) / 2, vG7low, vK15low, err)
    + qiter_positive(q, (low + up) / 2, up, vG7up, vK15up, err);
}

// If the function is ensured to be positive in the whole integration region, the adaptive method could be quicker
double qadaptive_positive(const function <double(const double)>& func, double low, double up, double err) {
  qGaus q(func);
  q.s = 0;

  q.calc(low, up);
  double vG7 = q.qG7(),
         vK15 = q.qK15();
  q.s = vK15;

  return qiter_positive(q, low, up, vG7, vK15, err);
}
