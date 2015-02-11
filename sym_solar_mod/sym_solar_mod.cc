#include<cmath>
#include<vector>
#include"sym_solar_mod.h"
#include"tridiagmatrix.h"
#include"interp.h"

using std::vector;
using std::abs;
using std::cout;
using std::endl;

unsigned sym_solar_mod::iearth = 0;
double sym_solar_mod::r[] = {0},
       sym_solar_mod::V[] = {0},
       sym_solar_mod::dVdr[] = {0},
       sym_solar_mod::dr2Vdr[] = {0};

int sym_solar_mod::phi_ini(double phi) {
  An = 0 == A ? 1 : A;
  k0 = 7.12782e-5 / phi; // According to Gleeson 1968, the relation between phi and rmax is phi=\int_{rmin}^{rmax}dr V(r)/(3k0)
  return 0;
}

double inline Vfunc(double r) {// from Potgieter et. al 2014 1302.1284
  return 2.6756e-6 * (1 - exp(- 13.3 * (r - 0.0046))); //r in Au/s  4e5/1.49498e11,  0.0046 is the radius of sun
}

double inline dVdrfunc(double r) {
  return - 2.6756e-6 * 13.3 * exp(- 13.3 * (r - 0.0046));//in s^-1
}

int sym_solar_mod::ini_r() {
  const double dlenth = 2.0 / 3,
        bk = pow(80, - dlenth),
        dpoints = 50,
        interval = bk / (dpoints - 1);
  for(int i = 0; i < dpoints; i++) {
    r[i] = i * interval;
    V[i] = Vfunc(r[i]);
    dVdr[i] = dVdrfunc(r[i]);
    dr2Vdr[i] = 2 * V[i] + dVdr[i] * r[i];
  }

  for(int i = 50; i < n_grid_r; i++) {
    r[i] = r[i - 1] * pow(80, 1.0 * (1 + dlenth) / (n_grid_r - dpoints));
    V[i] = Vfunc(r[i]);
    dVdr[i] = dVdrfunc(r[i]);
    dr2Vdr[i] = 2 * V[i] + dVdr[i] * r[i];
  }

  iearth = 149;
  return iearth;
}

sym_solar_mod::sym_solar_mod() {
  if (r[299] == 0) ini_r();
}
sym_solar_mod::sym_solar_mod(int A_, int Z_, double phi_, bool pflag_) : solar_mod(A_, Z_, pflag) {
  if (r[299] == 0) ini_r();
  phi_ini(phi_);
}
sym_solar_mod::sym_solar_mod(int A_, int Z_, double phi_) : solar_mod(A_, Z_) {
  if (r[299] == 0) ini_r();
  phi_ini(phi_);
}

inline void sym_solar_mod::gen_E(const spectrum &spec) {
  if(specE != spec.E) {
    specE = spec.E;

    const double factor = pow(spec.E.back() / spec.E[0], 1.0 / (n_grid_E - 1));
    double Etmp = specE[0],
           P, beta;

    for(unsigned i = 0; i < n_grid_E; i++) {
      E[i] = Etmp;
      P = (An / abs(Z)) * sqrt(Etmp  * (Etmp + 2 * m0));
      beta = sqrt(Etmp * (Etmp + 2 * m0)) / (Etmp + m0);
      lnP[i] = log(P);
      k[i] = k0 * beta * P;
      dalEdE[i] = (1 + m0 * m0 / (Etmp + m0) * (Etmp + m0));
      Etmp *= factor;
    }
  }
  
  static const double sq_rmax = sqrt(r[n_grid_r - 1]);
  interp intp_spec(spec);
  for(unsigned i = 0; i < n_grid_E; i++) umax[i] = sq_rmax * intp_spec.lnask(E[i]);
}

inline double sym_solar_mod::hij(unsigned i_r, unsigned i_E) const {
    return dr2Vdr[i_r] * r[i_r] / (3 * k[i_E]);
}

inline double sym_solar_mod::fij(unsigned i_r) const {
  return - r[i_r] * r[i_r];
}

inline double sym_solar_mod::gij(unsigned i_r, unsigned i_E) const {
  return (V[i_r] * r[i_r] / k[i_E] - 1) * r[i_r];
}

inline double sym_solar_mod::sij(unsigned i_r, unsigned i_E) const {
  return 0.25 + 1.5 * V[i_r] * r[i_r] / k[i_E] - r[i_r] * r[i_r] * dVdr[i_r] / k[i_E] - dr2Vdr[i_r] * dalEdE[i_E] / (3 * k[i_E]);
}

inline double sym_solar_mod::dr(unsigned i_r) const {
  return r[i_r + 1] - r[i_r];
}

inline double sym_solar_mod::dudr(unsigned i_r) const {
  return (u[i_r + 1] - u[i_r]) / dr(i_r);
}

int sym_solar_mod::mod(const spectrum &spec_o, spectrum &spec_t) {
  const double alpha = 0.5;
  gen_E(spec_o);

  for(int i = 0; i < n_grid_r; i++) u[i] = sqrt(r[i]) * spec_o.F.back();
  uearth[n_grid_E - 1] = u[iearth];

  const unsigned size = n_grid_r - 2;
  for(unsigned i_E = n_grid_E - 1; i_E >= 1; i_E--) {
    for(unsigned i_r = 1; i_r <= size; i_r++) {
      a[i_r - 1] = + gij(i_r, i_E) * (1 - alpha) / (dr(i_r - 1) * 2)
                   - fij(i_r) * (1 - alpha) * 2 / (dr(i_r - 1) * (r[i_r + 1] - r[i_r - 1]));

      b[i_r - 1] =   hij(i_r, i_E) / (lnP[i_E - 1] - lnP[i_E])
                   - gij(i_r, i_E) * (1- alpha) * (r[i_r + 1] - 2 * r[i_r] + r[i_r - 1]) / (2 * dr(i_r) * dr(i_r - 1))
                   - sij(i_r, i_E) * (1 - alpha)
                   + fij(i_r) * (1 - alpha) * 2 / (dr(i_r) * dr(i_r - 1));

      c[i_r - 1] = - gij(i_r, i_E) * (1 - alpha) / (dr(i_r) * 2)
                   - fij(i_r) * (1 - alpha) * 2 / (dr(i_r) * (r[i_r + 1] - r[i_r - 1]));

      d[i_r - 1] =   hij(i_r, i_E) * u[i_r] / (lnP[i_E - 1] - lnP[i_E])
                   + gij(i_r, i_E) * alpha * (dudr(i_r) + dudr(i_r - 1)) / 2
                   + sij(i_r, i_E) * alpha * u[i_r]
                   + fij(i_r) * alpha * (dudr(i_r) - dudr(i_r - 1)) * 2 / (r[i_r + 1] - r[i_r - 1]);
    }

    u[n_grid_r - 1] = umax[i_E];
    d[0] -= u[0] * a[0], d[size - 1] -= u[n_grid_r - 1] * c[size - 1];

    tridiagmatrix_solve(a, b, c, d, size);

    for(unsigned i_r = 0; i_r < size; i_r++) u[i_r + 1] = d[i_r];

    uearth[i_E] = u[iearth];
  }

  spec_t.ini(E + 1, uearth + 1, n_grid_E - 1); // drop the first point as we haven't calculated it
  if (pflag) spec_t.print();
  return 0;
}
