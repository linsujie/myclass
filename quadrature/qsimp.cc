#include <cmath>
#include <functional>
#include <iostream>

#include "quadrature.h"

using namespace std;
#ifdef QSIMP_PFLAG
#define COUTMESSAGE(var) cout << var
#else
#define COUTMESSAGE(var)
#endif

// The simple quadrature with trapezoidal rule, using the factor nfold to control the division of the region
double qtrap_once(const function <double(const double)>& func, double low, double up, int nfold)
{
  double ndivision = pow(2, nfold);
  double step = (up - low) / ndivision;

  double sum = (func(low) + func(up)) * step / 2;
  for (unsigned i = 1; i < ndivision; i++) sum += func(low + i * step) * step;

  COUTMESSAGE("qtrap_once::low " << low << ", up " << up << ", nfold " << nfold << ", result " << sum << endl);

  return sum;
}

// The quadrature  with trapezoidal rule, using a tolerance factor to control the precision
double qtrap(const function <double(const double)>& func, double low, double up, double tol)
{
  const int istart = 4, imax = 17;
  double os = qtrap_once(func, low, up, istart - 1),
         s;

  for (int i = istart; i < imax; i++) {
    s = qtrap_once(func, low, up, i);
    if (abs(s - os) < tol * abs(os) || (s == 0 && os == 0)) return s;
    os = s;
  }

  throw(quadrature::too_many_steps_val);
}

// The quadrature with simpson's rule, using a tolerance factor to control the precision
double qsimp(const function <double(const double)>& func, double low, double up, double tol)
{
  const int istart = 4,
        imax = 17;
  double ost = qtrap_once(func, low, up, istart - 2),
         st = qtrap_once(func, low, up, istart - 1),
         s = (4 * st - ost) / 3,
         os;

  for (int i = istart; i < imax; i++) {
    os = s;
    ost = st;

    st = qtrap_once(func, low, up, i);
    s = (4 * st - ost) / 3;
    if (abs(s - os) < tol * abs(os) || (s == 0 && os == 0)) return s;
    if (s != s) throw(quadrature::not_a_number_val);
  }

  cout << "The last two values are: " << s << " " << os << endl;

  throw(quadrature::too_many_steps_val);
}
