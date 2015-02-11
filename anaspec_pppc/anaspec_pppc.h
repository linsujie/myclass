#ifndef _ANASPEC_PPPC_H
#define _ANASPEC_PPPC_H
#include"anaspec.h"

/*********************************************************************
This class is used to calculate generation spectrum from the
annihilation of darkmatter Eg and mdm are both in [GeV], the result
returned by function ask is in [GeV^-1].
With the result of 1012.4515.
The cumulated mode will return the value of \int_0^{E_0} dN/dE dE
*********************************************************************/

#define PPPC_BRANCH_NUM 28
#define PPPC_PROD_NUM 7

using std::string;

namespace pppc {
enum pppc_branch { ele_L, ele_R, ele, mu_L, mu_R, mu, tau_L, tau_R, tau, quark, charm, bottom, top, W_L, W_T, W_boson, Z_L, Z_T, Z_boson, gluon, gamma_branch, higgs, nu_e, nu_mu, nu_tau, four_e, four_mu, four_tau };
enum pppc_product {positrons, antiprotons, gammas, neutrinos_e, neutrinos_mu, neutrinos_tau, antideuterons};

const string product_file[PPPC_PROD_NUM] = { "positrons", "antiprotons", "gammas", "neutrinos_e", "neutrinos_mu", "neutrinos_tau", "antideuterons" };
}

class anaspec_pppc : public anaspec {
public:
  //enum branch_choice {electron, mu, tau, W, up, charm, bottom, top, four_e, four_mu, four_tau};
  anaspec_pppc();
  anaspec_pppc(anaspec::product_choice product_);
  anaspec_pppc(double *branch_, anaspec::product_choice product_);
  anaspec_pppc(const std::vector <double> &branch_, anaspec::product_choice product_);

  double ask(double E, double mdm);
  double ask(double E, double mdm, bool cumulate);

private:
  static const bool exist[ANASPEC_PROD_NUM][ANASPEC_BRANCH_NUM];
  Interp2D table[ANASPEC_PROD_NUM][ANASPEC_BRANCH_NUM],
           cumutable[ANASPEC_PROD_NUM][ANASPEC_BRANCH_NUM];
  std::vector <bool> loaded;
  double low_x;

  double pppc_ask(double E, double mdm, pppc::pppc_product prod, bool cumulate);
  int load(anaspec::product_choice prod);
  int load(pppc::pppc_product prod);
};
#endif // for #ifndef _ANASPEC_PPPC_H
