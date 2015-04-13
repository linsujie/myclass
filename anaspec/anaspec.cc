#include"anaspec.h"
#include"mydebug.h"
#include"enum_utils.h"
#include<iostream>
#include<sstream>
#include<fstream>
#include<cmath>

using std::string;
using std::istringstream;
using std::ifstream;
using std::log10;
using std::vector;
using std::cout;
using std::endl;

const string anaspec::prodname[ANASPEC_PROD_NUM] = { "posi", "pbar", "gamma", "nu", "deuterons" },
    anaspec::braname[ANASPEC_BRANCH_NUM] = { "e", "mu", "tau", "ww", "uu", "cc", "bb", "tt", "foure", "fourmu", "fourtau" };

const vector <string> enum_name0 = {"positron", "antiproton", "gamma", "nu", "deuterons"},
      enum_name1 = {"electron", "mu", "tau", "W", "up", "charm", "bottom", "top", "four_e", "four_mu", "four_tau"};

const vector <vector <string> > anaspec::enum_names = { enum_name0, enum_name1 };

GETENUM(anaspec)

const bool anaspec::newformat[ANASPEC_PROD_NUM][ANASPEC_BRANCH_NUM] = { {1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0},
                                                                        {1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0},
                                                                        {1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0},
                                                                        {1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0},
                                                                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
      anaspec::exist[ANASPEC_PROD_NUM][ANASPEC_BRANCH_NUM] = { {1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0},
                                                               {0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0},
                                                               {1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0},
                                                               {1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0},
                                                               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
      anaspec::withcum[ANASPEC_PROD_NUM][ANASPEC_BRANCH_NUM] = { {1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0},
                                                                 {0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0},
                                                                 {1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0},
                                                                 {1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0},
                                                                 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} };

#define BRANCHING\
  for (int i = 0; i < ANASPEC_BRANCH_NUM; i++) branch[i] = branch_[i]
#define INILOADED memset(loaded, false, ANASPEC_BRANCH_NUM * ANASPEC_PROD_NUM * sizeof(bool))

anaspec::anaspec() {
  INILOADED;
}
anaspec::anaspec(anaspec::product_choice product_): product(product_) {
  INILOADED;
}
anaspec::anaspec(double *branch_, anaspec::product_choice product_): product(product_) {
  BRANCHING;
  INILOADED;
}
anaspec::anaspec(const vector <double> &branch_, anaspec::product_choice product_): product(product_) {
  BRANCHING;
  INILOADED;
}

int anaspec::rebranch(double *branch_) {
  BRANCHING;
  return 0;
}
int anaspec::rebranch(const vector <double> &branch_) {
  BRANCHING;
  return 0;
}
int anaspec::choose(anaspec::product_choice product_) {
  product = product_;
  return 0;
}

double anaspec::ask(double E, double mdm) {
  return ask(E, mdm, false);
}

double anaspec::ask(double E, double mdm, bool cumulate) {
  printDebugMsg("Routine", ">>ask: E, m_dm, cumulate = %f, %f, %d", E, mdm, cumulate);
  double sum = 0,
         realx;
  double upbound = newformat[product][0] ? 0.999999 : 0.9772;
  //Interp2D (*tmptable)[ANASPEC_BRANCH_NUM];
  //tmptable = cumulate ? cumutable : table;

  for (int i = 0; i < ANASPEC_BRANCH_NUM; i++)
    if (exist[product][i] && branch[i]) {
      if (!loaded[product][i]) load(product, (anaspec::branch_choice)i);
      if (cumulate && withcum[product][i] && E / mdm > 1e-5) {
        realx = (E / mdm) > 1 ? 1 : (E / mdm);
        sum += branch[i] * cumutable[product][i].lnask(realx, mdm);
      } else if(E / mdm > 1e-5 && E / mdm < upbound)
        sum += branch[i] * table[product][i].lnask(E / mdm, mdm) / mdm;
    }

  printDebugMsg("Routine", "<<ask: %f", sum);
  return sum;
}

int anaspec::load(anaspec::product_choice prod, anaspec::branch_choice branch) {
  load(prod, branch, false);
  if(withcum[prod][branch]) load(prod, branch, true);
  loaded[prod][branch] = true;
  return 0;
}

int anaspec::load(anaspec::product_choice prod, anaspec::branch_choice branch, bool cumulate) {
#ifdef DATDIR
  string filename = DATDIR;
#else
  string filename = ".";
#endif
  filename += "/" + prodname[prod] + "_" + braname[branch] + (cumulate ? "_cum" : "" ) + ".dat";
  printDebugMsg("Routine", ">>load: filename = %s", filename.c_str());
  ifstream dats(filename.c_str());

  const double mfactor = 2;
  Table2D tab;
  string line;
  double x, y, val;
  vector <double> yaxis;

  if(newformat[prod][branch]) {
    getline(dats, line);
    istringstream iss(line);
    while(iss >> y) yaxis.push_back(y);
  }

  while (getline(dats, line)) {
    istringstream iss(line);
    iss >> x;
    x *= log(10);

    if(newformat[prod][branch])
      for(unsigned i = 0; (iss >> val) && i < yaxis.size(); i++) tab.insval(x, yaxis[i] * log(10), val * log(10));
    else
      for (y = 1; iss >> val; y += log10(mfactor)) tab.insval(x, y * log(10), val * log(10)); //times log(10) to change the base from 10 to e
  }

  cumulate ? cumutable[prod][branch].lntabling(tab) : table[prod][branch].lntabling(tab);
  printDebugMsg("Routine", "<<load");
  return 0;
}
