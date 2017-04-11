#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <iomanip>
#include "interp.h"
#include "chisq.h"
#include "oformat.h"

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

const vector<double> zerox = { 1, 2 },
                     zeroy = { 0, 0 };
const spectrum chisq::zerospec(zerox, zeroy);

chisq::chisq(const string& filename, double Eindx)
{
  init(filename, Eindx);
  extra_sigma();
}

chisq::chisq(const chisq& another)
{
  E = another.E;
  F = another.F;
  sigma = another.sigma;
  total_sigma = another.total_sigma;
  dataname = another.dataname;
}

const vector<double> emptyvec;
int chisq::addexperiment(const string& line)
{
  E.push_back(emptyvec);
  F.push_back(emptyvec);
  sigma.push_back(emptyvec);

  dataname.push_back(line);
  return int(E.size()) - 1;
}

int chisq::init(const string& filename, double Eindx)
{
  int iset = -1;
  double Etmp, Ftmp, stmp;
  string line;

  ifstream datfile(filename.c_str());

  for (int nline = 1; getline(datfile, line); nline++) {
    if (line.size() == 0)
      continue;

    else if (line.at(0) == annota[0]) {
      iset = addexperiment(line);
    } else {
      if (-1 == iset)
        iset = addexperiment(annota);

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
  if (dataname.size() == 0)
    cout << "The datafile " << filename << " is empty." << endl;

  return 0;
}

int chisq::printdat(const string& filename) const
{
  vector<int> setnums;
  for (unsigned i = 0; i < dataname.size(); i++)
    setnums.push_back(i);
  return printdat(setnums, filename);
}

int chisq::printdat(const vector<int>& setnums, const string& filename) const
{
  ostringstream os;
  os << "#\t"
     << "E\t"
     << "F\t"
     << "err" << endl;
  dealoutput(filename, os, ios_base::out);

  for (unsigned i = 0; i < setnums.size(); i++)
    printdat(setnums[i], filename);

  return 0;
}

int chisq::printdat(int setnum, const string& filename) const
{
  ostringstream os;
  os << dataname[setnum] << endl
     << setiosflags(ios::scientific) << setprecision(6);

  for (unsigned i = 0; i < E[setnum].size(); i++)
    os << E[setnum][i] << " " << F[setnum][i] << " " << total_sigma[setnum][i] << endl;

  return dealoutput(filename, os, ios_base::app);
}

#ifndef NO_ROOT
TGraphErrors* chisq::GetTGraphErrors(int setnum, double index) const
{
  double Ftmp[F[setnum].size()], sigmatmp[total_sigma[setnum].size()];

  for (unsigned i = 0; i < F[setnum].size(); i++) {
    Ftmp[i] = F[setnum][i] * pow(E[setnum][i], index);
    sigmatmp[i] = total_sigma[setnum][i] * pow(E[setnum][i], index);
  }

  TGraphErrors* result = new TGraphErrors(E[setnum].size(), &(E[setnum][0]), Ftmp, 0, sigmatmp);
  result->SetName(dataname[setnum].c_str());
  result->SetTitle(dataname[setnum].c_str());

  return result;
}

chisq::chisq(TGraphErrors* gr, double Eindx)
{
  init(gr, Eindx);
  extra_sigma();
}
bool chisq::init(TGraphErrors* gr, double Eindx)
{
  const string annotate = "#";
  int iset = addexperiment(annotate + gr->GetTitle());

  E[iset].resize(gr->GetN());
  F[iset].resize(gr->GetN());
  sigma[iset].resize(gr->GetN());
  for (int i = 0; i < gr->GetN(); i++) {
    E[iset][i] = gr->GetX()[i];
    F[iset][i] = gr->GetY()[i] / pow(E[iset][i], Eindx);
    sigma[iset][i] = gr->GetEY()[i] / pow(E[iset][i], Eindx);
  }

  return true;
}

chisq::chisq(const vector<TGraphErrors*>& grs, double Eindx)
{
  init(grs, Eindx);
  extra_sigma();
}
bool chisq::init(const vector<TGraphErrors*>& grs, double Eindx)
{
  for (unsigned iset = 0; iset < grs.size(); iset++)
    init(grs[iset], Eindx);
  return false;
}
#endif

int chisq::printsizes() const
{
  for (unsigned i = 0; i < dataname.size(); i++)
    cout << dataname[i] << " with " << E[i].size() << " points" << endl;

  return 0;
}

double chisq::chi2(const spectrum& phi, bool pflag, const string& filename, ios_base::openmode mode) const
{
  vector<int> setnums;

  for (unsigned i = 0; i < dataname.size(); i++)
    setnums.push_back(i);

  return chi2(setnums, phi, pflag, filename, mode);
}
double chisq::chi2(const vector<int>& setnums, const spectrum& phi, bool pflag, const string& filename, ios_base::openmode mode) const
{
  if (pflag) {
    ostringstream os;
    os << "#\t"
       << "E\t"
       << "datF\t"
       << "F\t"
       << "err" << endl;
    dealoutput(filename, os, mode);
  }

  double sum = 0;
  for (unsigned i = 0; i < setnums.size(); i++)
    sum += chi2(setnums[i], phi, pflag, filename, ios_base::app);

  return sum;
}
double chisq::chi2(int setnum, const spectrum& phi, bool pflag, const string& filename, ios_base::openmode mode) const
{
  return chi2(setnum, &(phi.E[0]), &(phi.F[0]), phi.E.size(), pflag, filename, mode);
}

double chisq::chi2(const pArray& E_, const pArray& F_, bool pflag, const string& filename, ios_base::openmode mode) const
{
  vector<int> setnums;

  for (unsigned i = 0; i < dataname.size(); i++)
    setnums.push_back(i);

  return chi2(setnums, E_, F_, pflag, filename, mode);
}
double chisq::chi2(const vector<int>& setnums, const pArray& E_, const pArray& F_, bool pflag, const string& filename, ios_base::openmode mode) const
{
  if (pflag) {
    ostringstream os;
    os << "#\t"
       << "E\t"
       << "datF\t"
       << "F\t"
       << "err" << endl;
    dealoutput(filename, os, mode);
  }

  double sum = 0;
  for (unsigned i = 0; i < setnums.size(); i++)
    sum += chi2(setnums[i], E_, F_, pflag, filename, ios_base::app);

  return sum;
}
double chisq::chi2(int setnum, const pArray& E_, const pArray& F_, bool pflag, const string& filename, ios_base::openmode mode) const
{
  return chi2(setnum, E_.a, F_.a, E_.GetLength(), pflag, filename, mode);
}

double chisq::chi2(int setnum, const double* E_, const double* F_, int nsize, bool pflag, const std::string& filename, std::ios_base::openmode mode) const
{
  static ostringstream os;

  interp inp(E_, F_, nsize);
  double sum = 0;
  double f_calc, diff;

  if (pflag) {
    os.str("");
    os << dataname[setnum] << endl << setiosflags(ios::scientific) << setprecision(6);
  }

  for (int i = 0; i < int(E[setnum].size()); i++) {
    f_calc = inp.lnask_check(E[setnum][i]);
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
    dealoutput(filename, os, mode);
  }

  return sum;
}

int chisq::enlarge_sigma(double scale) {
  cout << total_sigma[0][0] << endl;
  for (auto& sigma_set : total_sigma)
    for (auto& sigma_value : sigma_set)
      sigma_value *= scale;
  return true;
}

int chisq::extra_sigma(const spectrum& sigma_)
{
  interp intpsigma(sigma_);
  total_sigma.clear();

  vector<double> tmpsigma;
  for (unsigned iset = 0; iset < sigma.size(); iset++) {
    tmpsigma.resize(sigma[iset].size());
    for (unsigned i = 0; i < sigma[iset].size(); i++) {
      double normal = sigma[iset][i],
             extra = intpsigma.lnask_check(E[iset][i]);
      tmpsigma[i] = sqrt(normal * normal + extra * extra);
    }
    total_sigma.push_back(tmpsigma);
  }

  return 0;
}

int chisq::dealoutput(const string& filename, const ostringstream& os, ios_base::openmode mode) const
{
  if (filename == "null")
    cout << os.str();
  else {
    ofstream of(filename, mode);
    of << os.str();
    of.close();
  }

  return 0;
}
