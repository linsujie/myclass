#include<cmath>
#include<cstdlib>
#include<iostream>
#include<fstream>
#include<sstream>
#include<cstring>
#include<vector>
#include<iomanip>
#include"interp.h"
#include"chisq.h"
#include"oformat.h"

using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::ostringstream;
using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::ios;
using std::ios_base;
using std::setiosflags;
using std::setprecision;

const char chisq::annota[2] = "#";

const vector <double> zerox = { 1, 2 },
      zeroy = { 0, 0 };
const spectrum chisq::zerospec(zerox, zeroy);

chisq::chisq(const string &filename, double Eindx) {
  init(filename, Eindx);
  extra_sigma();
}

chisq::chisq(const chisq &another) {
  E = another.E;
  F = another.F;
  sigma = another.sigma;
  total_sigma = another.total_sigma;
  dataname = another.dataname;
}

const vector <double> emptyvec;
int chisq::addexperiment(const string &line) {
  E.push_back(emptyvec);
  F.push_back(emptyvec);
  sigma.push_back(emptyvec);

  dataname.push_back(line);
  return int(E.size()) - 1;
}

int chisq::init(const string &filename, double Eindx) {
  int iset = -1;
  double Etmp, Ftmp, stmp;
  string line;

  ifstream datfile(filename.c_str());

  for (int nline = 1; getline(datfile, line); nline++) {
    if (line.size() == 0) continue;

    else if (line.at(0) == annota[0]) {
      iset = addexperiment(line);
    } else {
      if (-1 == iset) iset = addexperiment(annota);

      istringstream is(line);

      Etmp = Ftmp = stmp = 0;
      is >> Etmp >> Ftmp >> stmp;

      if (Eindx) {
        Ftmp = Ftmp / pow(Etmp, Eindx);
        stmp = stmp / pow(Etmp, Eindx);
      }
      E[iset].push_back(Etmp);
      F[iset].push_back(Ftmp);
      sigma[iset].push_back(stmp);

      if (0 == stmp) {
        cout << "The datafile " << filename << " wrong." << endl;
        exit(1);
      }
    }
  }
  if(dataname.size() == 0) cout << "The datafile " << filename << " is empty." << endl;

  return 0;
}

int chisq::printdat(const string &filename) const {
  vector <int> setnums;
  for (unsigned i = 0; i < dataname.size(); i++) setnums.push_back(i);
  return printdat(setnums, filename);
}

int chisq::printdat(const vector <int> &setnums, const string &filename) const {
  ostringstream os;
  os << "#\t" << "E\t" << "F\t" << "err" << endl;
  dealoutput(filename, os, ios_base::out);

  for (unsigned i = 0; i < setnums.size(); i++)
    printdat(setnums[i], filename);

  return 0;
}

int chisq::printdat(int setnum, const string &filename) const {
  ostringstream os;
  os << dataname[setnum] << endl
    << setiosflags(ios::scientific) << setprecision(6);

  for (unsigned i = 0; i < E[setnum].size(); i++)
    os << E[setnum][i] << " " << F[setnum][i] << " " << total_sigma[setnum][i] << endl;

  return dealoutput(filename, os, ios_base::app);
}

int chisq::printsizes() const {
  for(unsigned i = 0; i < dataname.size(); i++)
    cout << dataname[i] << " with " << E[i].size() << " points" << endl;

  return 0;
}

double chisq::chi2(const spectrum &phi, bool pflag, const string &filename, ios_base::openmode outmode) const {
  vector <int> setnums;

  for (unsigned i = 0; i < dataname.size(); i++) setnums.push_back(i);

  return chi2(setnums, phi, pflag, filename, outmode);
}

double chisq::chi2(const vector <int> &setnums, const spectrum &phi, bool pflag, const string &filename, ios_base::openmode outmode) const {
  if (pflag) {
    ostringstream os;
    os << "#\t" << "E\t" << "datF\t" << "F\t" << "err" << endl;
    dealoutput(filename, os, outmode);
  }

  double sum = 0;
  for (unsigned i = 0; i < setnums.size(); i++)
    sum += chi2(setnums[i], phi, pflag, filename, ios_base::app);

  return sum;
}
double chisq::chi2(int setnum, const spectrum &phi, bool pflag, const string &filename, ios_base::openmode outmode) const {
  static ostringstream os;

  const interp inp(phi.E, phi.F);
  double sum = 0;
  double f_calc, diff;

  if (pflag) {
    os.str("");
    os << dataname[setnum] << endl << setiosflags(ios::scientific) << setprecision(6);
  }

  for (int i = 0; i < int(E[setnum].size()); i++) {
    f_calc = inp.lnask(E[setnum][i]);
    diff = (F[setnum][i] - f_calc) / total_sigma[setnum][i];
    sum += diff * diff;

    if (pflag)
      os << " " << E[setnum][i]
        << " " << F[setnum][i]
        << " " << f_calc
        << " " << total_sigma[setnum][i]
        << endl;
  }

  if (pflag) {
    os << "# chi2: " << sum << endl;
    dealoutput(filename, os, outmode);
  }

  return sum;
}

int chisq::extra_sigma(const spectrum &sigma_) {
  const interp intpsigma(sigma_);
  total_sigma.clear();

  vector <double> tmpsigma;
  for (unsigned iset = 0; iset < sigma.size(); iset++) {
    tmpsigma.resize(sigma[iset].size());
    for (unsigned i = 0; i < sigma[iset].size(); i++) {
      double normal = sigma[iset][i],
             extra = intpsigma.lnask(E[iset][i]);
      tmpsigma[i] = sqrt(normal * normal + extra * extra);
    }
    total_sigma.push_back(tmpsigma);
  }

  return 0;
}

int chisq::dealoutput(const string &filename, const ostringstream &os, ios_base::openmode outmode) const {
  if (filename == "null") cout << os.str();
  else {
    ofstream of(filename, outmode);
    of << os.str();
    of.close();
  }

  return 0;
}
