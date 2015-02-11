#include<cmath>
#include"interp.h"
#include"ffd_solar_mod.h"

using std::abs;
using std::cout;
using std::endl;

int ffd_solar_mod::phi_ini(double phi_) {
  phi = phi_ * abs(double(Z)) / (0 == A ? 1 : A);
  return 0;
}

ffd_solar_mod::ffd_solar_mod() : solar_mod(0, 0, 0), phi(0) {};
ffd_solar_mod::ffd_solar_mod(int A_, int Z_, double phi_, bool pflag_):
  solar_mod(A_, Z_, pflag_) {
    phi_ini(phi_);
}


ffd_solar_mod::ffd_solar_mod(int A_, int Z_, double phi_):
  solar_mod(A_, Z_, false) {
    phi_ini(phi_);
}

int ffd_solar_mod::mod(const spectrum &spec_o, spectrum &spec_t) {
  const interp intp_ori(spec_o);

  if (phi >= 0) { //modulation
    for (unsigned i = 0; i < spec_t.E.size(); i++) {
      spec_t.F[i] = intp_ori.lnask(spec_t.E[i] + phi)
                    * spec_t.E[i] * (spec_t.E[i] + 2 * m0)
                    / ((spec_t.E[i] + phi) * (spec_t.E[i] + phi + 2 * m0));
    }

  } else { // demodulation
    for (unsigned i = 0; i < spec_t.E.size(); i++) {
      if (spec_t.E[i] + phi <= 0) spec_t.F[i] = 0;

      else {
        spec_t.F[i] = intp_ori.lnask(spec_t.E[i] + phi)
                      * spec_t.E[i] * (spec_t.E[i] + 2 * m0)
                      / ((spec_t.E[i] + phi) * (spec_t.E[i] + phi + 2 * m0));
      }
    }

    while (spec_t.E.size() > 0 && spec_t.F[0] == 0)
      spec_t.del_p(0);

    if (spec_t.E.size() == 0) cout << "All these flux should not come from the outside Galaxy" << endl;
  }

  if (pflag) spec_t.print();

  return 0;
}
