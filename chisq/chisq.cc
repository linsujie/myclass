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
using std::tuple;
using std::bind;
using std::make_tuple;

const char chisq::annota[2] = "#";

const vector<double> zerox = { 1, 2 },
                     zeroy = { 0, 0 };
const spectrum chisq::zerospec(zerox, zeroy);

inline vector<int> seq(int imin, int imax)
{
  vector<int> result; result.reserve(imax - imin);
  for (int i = imin; i < imax; i++) result.push_back(i);
  return result;
}

chisq::chisq(const string& filename, double Eindx)
{
  init(filename, Eindx);
  extra_sigma();
  chi2.setfunction(bind(&chisq::chi2_calc, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
  chi2.set_nums(seq(0, dataname.size()));

  chi2_RHOVALUE.setfunction(bind(&chisq::chi2_RHOVALUE_calc, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
  chi2_RHOVALUE.set_nums(seq(0, dataname.size()));
}

chisq::chisq(const chisq& another)
{
  E = another.E;
  F = another.F;
  sigma = another.sigma;
  total_sigma = another.total_sigma;
  dataname = another.dataname;

  chi2.setfunction(bind(&chisq::chi2_calc, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
  chi2.set_nums(seq(0, dataname.size()));

  chi2_RHOVALUE.setfunction(bind(&chisq::chi2_RHOVALUE_calc, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
  chi2_RHOVALUE.set_nums(seq(0, dataname.size()));
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
  return printdat(seq(0, dataname.size()), filename);
}
int chisq::printdat(const vector<int>& setnums, const string& filename) const
{
  cout << "printing " << setnums << endl;
  ostringstream os;
  os << "#\t"
     << "E\t"
     << "F\t"
     << "err" << endl;

  os << setiosflags(ios::scientific) << setprecision(6);

  for (auto setnum : setnums) {
    os << dataname[setnum] << endl;
    for (unsigned i = 0; i < E[setnum].size(); i++)
      os << E[setnum][i] << " " << F[setnum][i] << " " << total_sigma[setnum][i] << endl;
  }

  dealoutput(filename, os);
  return 0;
}
int chisq::printdat(int setnum, const string& filename) const
{
  vector<int> setnums = { setnum };
  return printdat(setnums, filename);
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

unsigned chisq::size(int setnum) const { return E[setnum].size(); }
unsigned chisq::size(const vector<int>& setnums) const
{
  unsigned sum = 0;
  for (auto i : setnums) sum += E[i].size();
  return sum;
}
unsigned chisq::size() const { return size(seq(0, E.size())); }

tuple<double, double> chisq::RHOVALUE_distribution(const vector<unsigned>& Ns)
{
  unsigned N = 0, Nbio = Ns.size();
  for (auto i : Ns) N += i;

  double mu = - 1.0 / N * (N - Nbio) / (N - 1),
         sigma2 = (N - 2.0) * (N - 2.0) / (N * N * (N - 1)) * (N - Nbio) / (N - 1),
         sigma = sqrt(sigma2);

  return make_tuple(mu, sigma);
}

double chisq::chi2_calc(const vector<int>& setnums, interp intp, bool pflag, const std::string& filename) const
{
  static ostringstream os;
  if (pflag) {
    os.str("");
    os << "#\t"
       << "E\t"
       << "datF\t"
       << "F\t"
       << "err" << endl;
    os << setiosflags(ios::scientific) << setprecision(6);
  }

  double sum = 0;
  double f_calc, diff;

  for (auto setnum : setnums) {
    if (pflag) os << dataname[setnum] << endl;
    for (int i = 0; i < int(E[setnum].size()); i++) {
      f_calc = intp.lnask_check(E[setnum][i]);
      diff = (F[setnum][i] - f_calc) / total_sigma[setnum][i];
      sum += diff * diff;

      if (pflag)
        os << " " << E[setnum][i]
          << " " << F[setnum][i]
          << " " << f_calc
          << " " << total_sigma[setnum][i]
          << endl;
    }
  }

  if (pflag) {
    os << "# chi2: " << sum << endl;
    dealoutput(filename, os);
  }
  return sum;
}

tuple<double,double> chisq::chi2_RHOVALUE_calc(const vector<int>& setnums, interp intp, bool pflag, const string& filename) const
{
  static ostringstream os;
  if (pflag) {
    os.str("");
    os << "#\t"
       << "E\t"
       << "datF\t"
       << "F\t"
       << "err" << endl;
    os << setiosflags(ios::scientific) << setprecision(6);
  }

  double chi = 0, rho = 0;

  for (auto setnum : setnums) {
    if (pflag) os << dataname[setnum] << endl;

    double last_diff = 0;
    for (int i = 0; i < int(E[setnum].size()); i++) {
      double f_calc = intp.lnask_check(E[setnum][i]);
      double diff = (F[setnum][i] - f_calc) / total_sigma[setnum][i];
      chi += diff * diff;
      if (i >= 1) rho += last_diff * diff;
      last_diff = diff;

      if (pflag)
        os << " " << E[setnum][i]
          << " " << F[setnum][i]
          << " " << f_calc
          << " " << total_sigma[setnum][i]
          << endl;
    }
  }
  rho /= chi;

  if (pflag) {
    os << "# chi2: " << chi << "  rho: " << rho << endl;
    dealoutput(filename, os);
  }
  return make_tuple(chi, rho);
}

int chisq::enlarge_sigma(double scale)
{
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

int chisq::dealoutput(const string& filename, const ostringstream& os) const
{
  if (filename == "null")
    cout << os.str();
  else {
    ofstream of(filename);
    of << os.str();
    of.close();
  }

  return 0;
}
