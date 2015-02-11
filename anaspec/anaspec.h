#ifndef _ANASPEC_H
#define _ANASPEC_H
#include<string>
#include<vector>
#include"Interp2D.h"

#define ANASPEC_PROD_NUM 5
#define ANASPEC_BRANCH_NUM 11

/*********************************************************************
This class is used to calculate generation spectrum from the
annihilation of darkmatter Eg and mdm are both in [GeV], the result
returned by function ask is in [GeV^-1].
Modified from anaspec.f(yuan)    linsj20140308
The cumulated mode will return the value of \int_0^{E_0} dN/dE dE
*********************************************************************/
class anaspec {
public:
  enum product_choice {positron, antiproton, gamma, nu, deuterons};
  enum branch_choice {electron, mu, tau, W, up, charm, bottom, top, four_e, four_mu, four_tau};
  static const unsigned product_num, branch_num;
  static const std::vector <std::vector <std::string> > enum_names;
  //enum branch_choice {electron, mu, tau, W, up, bottom, top};

  anaspec();
  anaspec(anaspec::product_choice product_);
  anaspec(double *branch_, anaspec::product_choice product_);
  anaspec(const std::vector <double> &branch_, anaspec::product_choice product_);

  static unsigned getenum(const std::string &enum_name);
  int rebranch(double *branch_);
  int rebranch(const std::vector <double> &branch_);
  int choose(anaspec::product_choice product_);
  virtual double ask(double E, double mdm);
  virtual double ask(double E, double mdm, bool cumulate);

protected:
  anaspec::product_choice product;
  double branch[ANASPEC_BRANCH_NUM];

private:
  static const std::string prodname[ANASPEC_PROD_NUM], braname[ANASPEC_BRANCH_NUM];
  static const bool exist[ANASPEC_PROD_NUM][ANASPEC_BRANCH_NUM],
               withcum[ANASPEC_PROD_NUM][ANASPEC_BRANCH_NUM],
               newformat[ANASPEC_PROD_NUM][ANASPEC_BRANCH_NUM];
  Interp2D table[ANASPEC_PROD_NUM][ANASPEC_BRANCH_NUM],
           cumutable[ANASPEC_PROD_NUM][ANASPEC_BRANCH_NUM];
  bool loaded[ANASPEC_PROD_NUM][ANASPEC_BRANCH_NUM];

  int load(anaspec::product_choice prod, anaspec::branch_choice bra);
  int load(anaspec::product_choice prod, anaspec::branch_choice bra, bool cumutable);
};

#endif // for #ifndef _ANASPEC_H
