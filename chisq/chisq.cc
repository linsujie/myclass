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
chisq::chisq(const string &filename, double Eindx) {
  init(filename, Eindx);
}

int chisq::init(const string &filename, double Eindx) {
  double tmp;
  string line;
  vector <double> ldat;
  vector <vector<double> > dat;

  ifstream datfile(filename.c_str());

  for (int nline = 1; getline(datfile, line); nline++) {
    if (line.size() == 0) continue;

    else if (line.at(0) == annota[0]) {
      if (nline != 1) {
        data.push_back(dat);
        dat.clear();
      }

      dataname.push_back(line);

    } else {
      if (nline == 1) dataname.push_back(annota);

      ldat.clear();
      istringstream is(line);

      while (is >> tmp) ldat.push_back(tmp);

      if (Eindx) {
        ldat[1] = ldat[1] / pow(ldat[0], Eindx);
        ldat[2] = ldat[2] / pow(ldat[0], Eindx);
      }

      if (ldat.size() < 3) {
        cout << "The datafile " << filename << " wrong." << endl;
        exit(1);
      }

      dat.push_back(ldat);
    }
  }

  data.push_back(dat);

  if (data.size() != dataname.size())
    cout << "Warning::chisq::init::The size of data and dataname is not the same" << endl;

  return 0;
}

int chisq::printdat() const {
  cout << "#\t" << "E\t" << "F\t" << "err" << endl;

  for (unsigned i_set = 0; i_set < data.size(); i_set++) {
    printdat(i_set, false);
  }

  return 0;
}
int chisq::printdat(int setnum, bool head) const {
  if (head) cout << "#\t" << "E\t" << "F\t" << "err" << endl;

  cout << dataname[setnum] << endl;

  for (unsigned i = 0; i < data[setnum].size(); i++) {
    printf(FORMATE3, data[setnum][i][0], data[setnum][i][1], data[setnum][i][2]);
  }

  return 0;
}

int chisq::printsizes() const {
  for(unsigned i = 0; i < dataname.size(); i++)
    cout << dataname[i] << " with " << data[i].size() << " points" << endl;

  return 0;
}

double chisq::chi2(const spectrum &phi, bool flagp, const string &filename, ios_base::openmode outmode) const {
  ostringstream os;
  double result = chi2(phi, flagp, os);

  if (flagp) dealoutput(filename, os, outmode);

  return result;
}
double chisq::chi2(const spectrum &phi, bool flagp, ostringstream &os) const {
  double sum = 0;

  if (flagp) os << "#\t" << "E\t" << "datF\t" << "F\t" << "err" << endl;

  for (int i_set = 0; unsigned(i_set) < data.size(); i_set++) {
    sum += chi2(i_set, phi, flagp, false, os);
  }

  return sum;
}

double chisq::chi2(int setnum, const spectrum &phi, bool flagp, bool head, const string &filename, ios_base::openmode outmode) const {
  ostringstream os;

  double result = chi2(setnum, phi, flagp, head, os);

  if (flagp) dealoutput(filename, os, outmode);

  return result;
}
double chisq::chi2(int setnum, const spectrum &phi, bool flagp, bool head, ostringstream &os) const {
  const interp inp(phi.E, phi.F);
  double sum = 0;
  double f_calc, diff;

  if (flagp && head) os << "#\t" << "E\t" << "datF\t" << "F\t" << "err" << endl;

  if (flagp) os << dataname[setnum] << endl;
  os << setiosflags(ios::scientific) << setprecision(6);

  for (int i = 0; i < int(data[setnum].size()); i++) {
    f_calc = inp.lnask(data[setnum][i][0]);
    diff = (data[setnum][i][1] - f_calc) / data[setnum][i][2];
    sum += diff * diff;

    if (flagp)
      os << " " << data[setnum][i][0]
        << " " << data[setnum][i][1]
        << " " << f_calc
        << " " << data[setnum][i][2]
        << endl;
  }

  return sum;
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
