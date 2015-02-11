#ifndef _SYM_SOLAR_MOD_H
#define _SYM_SOLAR_MOD_H
/*********************************************************************
 * Unit: phi(GV) E(GV) r(AU) k(AU^2/s) V(AU/s)
 * E is kinetic energy per nucleon
 *
 * The equation solved here is the eq 3 in Fisk 1971:
 * [3Vr/2k - r^2V'/k + 1/4 - 1/3kr * d(r^2V)/dr * (1 + E^2/(E+E0)^2)]u
 *          + (rV/k - 1)r u' - r^2 u'' = 1/3k * (dr^2V)/dr * dudlnp
 *
 * The calculated variable is u = r^(1/2) * flux.
 *
 * Thus we can derive the inner bound naturally, the boundary conditions
 * are therefore:
 *   r_min = 0, r_max = 80 AU
 *   u(r_max, E) = r_max^(1/2) * fLIS(E)
 *   u(0, E) = 0
 *   u(r, E_max) = r^(1/2) * fLIS(E_max)
 *
 * The grid of r dimension is dense around r ~ 0 to reduce the influence
 * of inner boundary. In addition, r[i] is designed to include an element
 * equal to 1, thus we can get the value u[earth] without interpolation.
*********************************************************************/

#include"solar_mod.h"

class sym_solar_mod : public solar_mod {
private:
  enum sizes {n_grid_r = 300, n_grid_E = 500};
  static unsigned iearth; // the index where r = 1AU; 
  double k0;
  static double r[n_grid_r], V[n_grid_r], dVdr[n_grid_r], dr2Vdr[n_grid_r]; // dr2Vdr stands for the term 1/r * d(r^2 V)/dr
  double u[n_grid_r],
         lnP[n_grid_E], k[n_grid_E], E[n_grid_E], dalEdE[n_grid_E], // dalEdE is d(alpha * E)/dE
         umax[n_grid_E], uearth[n_grid_E];
  long double a[n_grid_r - 2], b[n_grid_r - 2], c[n_grid_r - 2], d[n_grid_r - 2];

  std::vector <double> specE;
  int phi_ini(double phi_);

  int ini_r();
  inline void gen_E(const spectrum &spec);

  inline double hij(unsigned i_r, unsigned i_E) const;
  inline double fij(unsigned i_r) const;
  inline double gij(unsigned i_r, unsigned i_E) const;
  inline double sij(unsigned i_r, unsigned i_E) const;
  inline double dr(unsigned i_r) const;
  inline double dudr(unsigned i_r) const;

public:
  int An; // An is the particle number inside the nuclear
  sym_solar_mod();
  sym_solar_mod(int A_, int Z_, double phi_, bool pflag_);
  sym_solar_mod(int A_, int Z_, double phi_);

  int mod(const spectrum &spec_o, spectrum &spec_t);
};
#endif // for #ifndef _SYM_SOLAR_MOD_H
