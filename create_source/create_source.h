#ifndef _CREATE_SOURCE_H
#define _CREATE_SOURCE_H
/*********************************************************************
To generate a injection point source randomly
 *********************************************************************/
#include"spectrum.h"

namespace pro {
class source {
private:
  double Es, Ee, factor, nu_nuc, sigma_nu, sigma_A;
  double get_z();
  double get_r();
  double get_nu();
  inline double Qr(double r, double r_sun, double alpha, double beta);
  //double gaussrand(double x0, double sigma);
  int create_injection(double delta);
  /*********************************************************************
    int test_ran(double (source::*ranfun)(),double range_a,double range_b,int n_bin,int num);
   *********************************************************************/
public:
  double r, z, theta, A0;
  source(double Es_, double Ee_, double factor_, double nu_nuc_, double sigma_nu_, double A0_, double sigma_A_);
  spectrum Q;
  int create_source();
};
}
#endif // for #ifndef _CREATE_SOURCE_H
