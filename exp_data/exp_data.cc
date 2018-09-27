#include <cmath>
#include <assert.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "interp.h"
#include "exp_data.h"
#include "oformat.h"

using namespace std;

const char exp_data::annota[2] = "#";

template<typename T> exp_data::exp_data(const T& object, double Eindx, bool pflag_) :
  pflag(pflag_), os(&cout) { init(object, Eindx); total_sigma = sigma; }
template<typename T> exp_data::exp_data(const T* object, double Eindx, bool pflag_) :
  pflag(pflag_), os(&cout) { init(object, Eindx); total_sigma = sigma; }

exp_data::exp_data() : pflag(false), os(&cout) {}
template exp_data::exp_data(const string&, double, bool);
template exp_data::exp_data(const char *, double, bool);
exp_data::exp_data(const exp_data& another) : 
  dataname_(another.dataname_), adopted(another.adopted), pflag(another.pflag), os(another.os),
  low_index(another.low_index), up_index(another.up_index), E(another.E), F(another.F),
  sigma(another.sigma), total_sigma(another.total_sigma) {}

int exp_data::addexperiment(const string& line)
{
  E.emplace_back(0);
  F.emplace_back(0);
  sigma.emplace_back(0);

  dataname_.push_back(line);
  adopted.push_back(true);
  return int(E.size()) - 1;
}

void exp_data::format_dataname()
{
  int max_size = 0;
  for (const auto& s : dataname_) max_size = fmax(max_size, s.size());
  for (auto& s : dataname_) s.resize(max_size, ' ');
}

bool exp_data::init(const string& filename, double Eindx)
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
        cerr << "The datafile " << filename << " wrong." << endl;
        exit(1);
      }
    }
  }

  if (dataname_.size() == 0)
    cerr << "Warning::The datafile " << filename << " is empty." << endl;

  set_range(-1, -1);
  format_dataname();
  return 0;
}

#ifndef NO_ROOT
template exp_data::exp_data(const TGraphErrors*, double, bool);
template exp_data::exp_data(const vector<TGraphErrors*>&, double, bool);

bool exp_data::add_TGraphErrors(const TGraphErrors* gr, double Eindx)
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
bool exp_data::init(const vector<TGraphErrors*>& grs, double Eindx)
{
  for (unsigned iset = 0; iset < grs.size(); iset++)
    add_TGraphErrors(grs[iset], Eindx);

  set_range(-1, -1);
  format_dataname();
  return true;
}
bool exp_data::init(const TGraphErrors* gr, double Eindx)
{
  add_TGraphErrors(gr, Eindx);
  set_range(-1, -1);
  format_dataname();
  return true;
}

TGraphErrors* exp_data::GetTGraphErrors(int setnum, double index) const
{
  double Ftmp[F[setnum].size()], sigmatmp[total_sigma[setnum].size()];

  for (unsigned i = 0; i < F[setnum].size(); i++) {
    Ftmp[i] = F[setnum][i] * pow(E[setnum][i], index);
    sigmatmp[i] = total_sigma[setnum][i] * pow(E[setnum][i], index);
  }

  TGraphErrors* result = new TGraphErrors(E[setnum].size(), &(E[setnum][0]), Ftmp, 0, sigmatmp);
  result->SetName(dataname_[setnum].c_str());
  result->SetTitle(dataname_[setnum].c_str());

  return result;
}
#endif

void exp_data::choose_subset_int(const vector<int>& setnums)
{
  adopted.assign(adopted.size(), false);

  for (const auto& setnum : setnums) {
    assert(setnum < adopted.size() && "The id of chosen set should be smaller than nset");
    adopted[setnum] = true;
  }
}

string& trim(string& str)
{
  size_t rbound = str.find_last_not_of(" \t\f\v\n\r");
  size_t lbound = str.find_first_not_of(" \t\f\v\n\r");
  if (string::npos != rbound) str = str .substr(lbound, rbound + 1 - lbound);
  else str.clear();

  return str;
}

void exp_data::choose_subset_str(const vector<string>& setnames)
{
  adopted.assign(adopted.size(), false);

  for (const auto& setname : setnames) {
    auto setiter = find_if(dataname_.begin(), dataname_.end(),
        [&setname](string fullname) -> bool { fullname.erase(0,1); return trim(fullname) == setname; });
    unsigned setnum = setiter - dataname_.begin();

    assert(setnum < dataname_.size() && "The given setname should be exist");
    adopted[setnum] = true;
  }
}

void exp_data::set_range(double lowcut, double upcut)
{
  low_index.resize(E.size());
  up_index.resize(E.size());

  for (unsigned iset = 0; iset < E.size(); iset++) {
    low_index[iset] = lowcut < 0 ? 0 : (lower_bound(E[iset].begin(), E[iset].end(), lowcut) - E[iset].begin());
    up_index[iset] = upcut < 0 ? E[iset].size() : (lower_bound(E[iset].begin(), E[iset].end(), upcut) - E[iset].begin());
  }
}

void exp_data::set_print(bool pflag_, ostream& os_)
{
  pflag = pflag_;
  os = &os_;
}

void exp_data::enlarge_sigma(double scale, int setnum)
{
  assert(setnum < (int)total_sigma.size());

  if (setnum >= 0) {
    for (auto& sigma_value : total_sigma[setnum]) sigma_value *= scale;
  } else {
    for (auto& sigma_set : total_sigma)
      for (auto& sigma_value : sigma_set)
        sigma_value *= scale;
  }
}

void exp_data::extra_sigma(const spectrum& sigma_, int setnum)
{
  assert(setnum < (int)total_sigma.size());

  interp intpsigma(sigma_);
  for (unsigned i = 0; i < total_sigma[setnum].size(); i++) {
    double normal = total_sigma[setnum][i],
    extra = intpsigma.lnask_check(E[setnum][i]);
    total_sigma[setnum][i] = sqrt(normal * normal + extra * extra);
  }
}

void exp_data::reset_sigma() { total_sigma = sigma; }

const string& exp_data::dataname(int setnum) const
{
  assert(setnum < dataname_.size() && "The id of required set should be smaller than nset");
  return dataname_[setnum];
}

int exp_data::summary() const
{
  for (unsigned i = 0; i < dataname_.size(); i++)
    cout << dataname_[i] << ": " << E[i].size() << "[" << size(i) << "] points" << (adopted[i] ? " *" : "") << endl;

  return 0;
}

unsigned exp_data::size(int setnum) const{ return up_index[setnum] - low_index[setnum]; }
unsigned exp_data::size() const
{
  unsigned sum = 0;
  for (unsigned i = 0; i < dataname_.size(); i++)
    if (adopted[i]) sum += size(i);

  return sum;
}

ostream& operator<<(ostream& os, const exp_data& data)
{
  os << "#\tEnergy\tFlux\tError" << endl
    << setiosflags(ios::scientific) << setprecision(6);

  for (unsigned iset = 0; iset < data.adopted.size(); iset++)
    if (data.adopted[iset]) {
      os << data.dataname_[iset] << endl;
      for (unsigned i = data.low_index[iset]; i < data.up_index[iset]; i++)
          os << data.E[iset][i] << " " << data.F[iset][i] << " " << data.total_sigma[iset][i] << endl;
    }

  return os;
}

tuple<double, double> exp_data::rho_distribution(const vector<unsigned>& Ns)
{
  unsigned N = 0, Nbio = Ns.size();
  for (auto i : Ns) N += i;

  double mu = - 1.0 / N * (N - Nbio) / (N - 1),
         sigma2 = (N - 2.0) * (N - 2.0) / (N * N * (N - 1)) * (N - Nbio) / (N - 1),
         sigma = sqrt(sigma2);

  return make_tuple(mu, sigma);
}

double exp_data::compare(const pArray& E, const pArray& F, exp_data_statistic& result) const
{
  interp intp(E.a, F.a, E.GetLength());
  return compare(intp, result);
}
double exp_data::compare(const spectrum& spec, exp_data_statistic& result) const
{
  interp intp(&(spec.E[0]), &(spec.F[0]), spec.E.size());
  return compare(intp, result);
}
double exp_data::compare(interp& intp, exp_data_statistic& result) const
{
  if (!E.empty()) intp.lnask_check(E[0][0]);
  auto func = [&intp](double x) -> double { return intp.lnask(x); };

  return compare(func, result);
}
double exp_data::compare(const function<double(double)>& func, exp_data_statistic& result) const
{
  result.diff_table.resize(E.size());
  result.set_adopted(&adopted);

  for (unsigned iset = 0; iset < E.size(); iset++)
    if (adopted[iset]) {
      result.diff_table[iset].assign(E[iset].size(), 0);
      for (unsigned i = low_index[iset]; i < up_index[iset]; i++)
        result.diff_table[iset][i] = (F[iset][i] - func(E[iset][i])) / total_sigma[iset][i];
    }

  if (pflag) {
    bool first_line = true;
    for (unsigned iset = 0; iset < E.size(); iset++)
      if (adopted[iset]) {
        if (first_line) (*os) << "#\tE\tdata_F\terr\t|\tF\tdiff" << endl << setiosflags(ios::scientific) << setprecision(6);
        first_line = false;
        cout << dataname_[iset] << endl;
        for (unsigned i = low_index[iset]; i < up_index[iset]; i++)
          (*os) << " " << E[iset][i] << " " << F[iset][i] << " " << total_sigma[iset][i] << " | "
            << F[iset][i] - result.diff_table[iset][i] * total_sigma[iset][i] << " " << result.diff_table[iset][i] << endl;
      }
  }

  double chi2 = result.chi2();
  if (pflag)
    (*os) << "# chi2: " << chi2 << " | rho: " << result.rho() << endl;

  return chi2;
}

void exp_data_statistic::set_adopted(const vector<bool> *adopted_) { adopted = adopted_; }

double exp_data_statistic::chi2()
{
  chi2_values.assign(diff_table.size(), 0);
  double sum = 0;

  for (unsigned iset = 0; iset < diff_table.size(); iset++)
    if ((*adopted)[iset]) {
      for (const auto& d : diff_table[iset])
        if (d) chi2_values[iset] += d * d;
      sum += chi2_values[iset];
    }

  return sum;
}

double exp_data_statistic::rho()
{
  rho_values.assign(diff_table.size(), 0);
  double total_rho = 0,
         total_chi2 = 0;

  for (unsigned iset = 0; iset < diff_table.size(); iset++)
    if ((*adopted)[iset]) {
      for (unsigned i = 1; i < diff_table[iset].size(); i++)
        if (diff_table[iset][i - 1] && diff_table[iset][i]) rho_values[iset] += diff_table[iset][i - 1] * diff_table[iset][i];

      total_rho += rho_values[iset];
      total_chi2 += chi2_values[iset];
    }

  return total_rho / total_chi2;
}

ostream& operator<<(ostream& os, const exp_data_statistic& result)
{
  double total_chi2 = 0, total_rho = 0;

  for (unsigned iset = 0; iset < result.diff_table.size(); iset++)
    if ((*result.adopted)[iset]) total_chi2 += result.chi2_values[iset];


  for (unsigned iset = 0; iset < result.diff_table.size(); iset++)
    if ((*result.adopted)[iset]) {
      os << setiosflags(ios::scientific) << setprecision(6);
      os << result.chi2_values[iset] << " | " << result.rho_values[iset] / total_chi2 << endl << "# ";
      for (const auto& v : result.diff_table[iset])
        if (v != 0) os << " " << v;
      os << endl;
      total_rho += result.rho_values[iset] / total_chi2;
    }

  os << "# in total:" << endl << "# chi2: " << total_chi2 << " | rho: " << total_rho;
  return os;
}
