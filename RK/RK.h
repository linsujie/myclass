/*********************************************************************
This class is to integrate a function with RK method
*********************************************************************/

#ifndef _RK_H
#define _RK_H
#include"gfunction.h"

#define RK_PARA_DIM 4
#define RK_STMAX 100000000
class RK {
private:
  std::vector <double> k[RK_PARA_DIM];
  double stlen, b, a, fac, errmax;
  bool sequence(double a, double b, double c);
  std::vector <double> ytmp, yini;
  static const double param_c[RK_PARA_DIM], param_b[RK_PARA_DIM], param_a[RK_PARA_DIM][RK_PARA_DIM];

public:
  std::vector <double> y;
  int setstl(double stlen_, double factor_, double errmax_); //Set step length errmax is to set the max step locally with the limit stlen<=errmax/k[0], when errmax is 0, the step would not be limited
  int setini(double start, double end, double y0);
  int setini(double start, double end, const std::vector <double> &y0); //Set Range (a_,b_) and initial value y_

  double dork(gfunction *func, int print_inval);
  double dork(gfunction *func);
  std::vector <std::vector <double> > dork_record(gfunction *func);
};

#endif // for #ifndef _RK_H
