#include<vector>
#include<cstdio>
#include"gfunction.h"

/*********************************************************************
This class calculate solve the 1d stable differential equation u(t) by
iteration procedure, with boundary u(t_min) and u(t_max) specified
*********************************************************************/

class iteration {
private:
  std::vector <double> x;
  gfunction *func;
  double errs;
  int print(FILE *stream) const;

public:
  std::vector <double> u;
  unsigned size;

  iteration();
  iteration(double u_tmin, double u_tmax, const std::vector <double> x_, gfunction *func_);
  iteration(double u_tmin, double u_tmax, const std::vector <double> x_);
  
  int iniaxis(double u_tmin, double u_tmax, const std::vector <double> t_);
  int inifunc(gfunction *func_);
  int inibound(double u_tmin, double u_tmax);
  int seterr(double err_);

  int run();
  int print() const;
  int print(const char *fname) const;
};
