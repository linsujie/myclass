#ifndef _PROPAGATOR_H
#define _PROPAGATOR_H
/*********************************************************
 *propagator of protons in the Galaxy                    *
 *output propagator: kpc^-3                              *
 *Modified from posi_propagator                          *
 *Reference: Lavalle et al., 2008, A&A, 479, 427         *
 *********************************************************/

/*********************************************************************
r is relative radial distance from source to observer
z is absolute height of source
zob is the height of obsever
  *z is different from r as we only consider the bundary in z dimension
E is observational energy originates from source energy Es, in GeV
propagation parameters:
      [K0] = kpc^2/kyr
      [delta]= 1
      [l] = kpc
*********************************************************************/

#include"Interp2D.h"
#ifndef PI
#define PI 3.1415926536
#endif

namespace pro {
class Green {
private:
  double K0, E0, delta, L, zob;
  int nmax;
  Interp2D intp;
  int theta(double x);
  double phi_exp(int n, double z, double lambdaD2, int prime, int flag);

  double G1d(double z, double lambdaD2, int flag);
  double G1d(double z, double lambdaD2);
public:

  Green(double K0_in, double E0_in, double delta_in, double L_in, double zob_in, int nmax_in);

  int create_tab();
  double G1dq(double z, double lambdaD2);

  double G(double r, double z, double t, double E);
  double Gq(double r, double z, double t, double E);
};
}
#endif // for #ifndef _PROPAGATOR_H
