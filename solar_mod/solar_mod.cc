#include<cmath>
#include"interp.h"
#include"solar_mod.h"

using std::abs;
using std::cout;
using std::endl;

#define m0A 0 == A ? 0.511e-3 : 0.9382

solar_mod::solar_mod() : pflag(false), A(0), Z(0) {};
solar_mod::solar_mod(int A_, int Z_, bool pflag_): pflag(pflag_), m0(m0A), A(A_), Z(Z_) {}


solar_mod::solar_mod(int A_, int Z_): pflag(false), m0(m0A), A(A_), Z(Z_) {}

int solar_mod::ini(int A_, int Z_, double phi_, bool pflag_) {
  A = A_;
  Z = Z_;
  m0 = (m0A);
  phi_ini(phi_);
  pflag = pflag_;
  return 0;
}
int solar_mod::ini(int A_, int Z_, double phi_) {
  return ini(A_, Z_, phi_, false);
}

int solar_mod::mod(spectrum &spec) {
  spectrum spec_tmp(spec);
  return mod(spec_tmp, spec);
}
int solar_mod::mod(spectrum &spec, double phi_) {
  phi_ini(phi_);
  spectrum spec_tmp(spec);
  return mod(spec_tmp, spec);
}

int solar_mod::mod(const spectrum &spec_o, spectrum &spec_t) {
  cout << "solar_mod::mod::The modulating function is not called" << endl;
  return 0;
}
int solar_mod::mod(const spectrum &spec_o, spectrum &spec_t, double phi_) {
  phi_ini(phi_);
  return mod(spec_o, spec_t);
}

int solar_mod::phi_ini(double phi_) {
  cout << "solar_mod::phi_ini::The phi is not changed" << endl;
  return 0;
}
