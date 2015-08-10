#include<iostream>
#include<sstream>
#include<fstream>
#include<cmath>
#include<cstdlib>
#include<iomanip>
#include"vec_utils.h"
#include"spectrum.h"
#include"interp.h"
#include"oformat.h"

using std::vector;
using std::string;
using std::istringstream;
using std::ostringstream;
using std::ifstream;
using std::ofstream;
using std::abs;
using std::cout;
using std::endl;
using std::ios;
using std::setiosflags;
using std::setprecision;

spectrum::spectrum() : Es(0), Ee(0), factor(0) {}
spectrum::spectrum(double *E_, double *F_, unsigned num): E(E_, E_ + num), F(F_, F_ + num), Es(0), Ee(0), factor(0) {}
spectrum::spectrum(const vector <double> &E_, const vector <double> &F_):
  E(E_), F(F_), Es(0), Ee(0), factor(0) {
  ini_check();
}
spectrum::spectrum(double Es_, double Ee_, double factor_):
  Es(Es_), Ee(Ee_), factor(factor_) {
  for (double Ex = Es; Ex < Ee; Ex *= factor) {
    E.push_back(Ex);
    F.push_back(0);
  }
}
spectrum::spectrum(const string &filename, double Eindx): Es(0), Ee(0), factor(0) {
  ini(filename, Eindx);
}



inline int spectrum::clear_lab() {
  Es = 0;
  Ee = 0;
  factor = 0;
  return 0;
}
inline int spectrum::clear_spec() {
  E.clear();
  F.clear();
  return 0;
}


int spectrum::ini() {
  clear_lab();
  clear_spec();
  return 0;
}
int spectrum::ini(double *E_, double *F_, unsigned num) {
  ini();

  for (unsigned i = 0; i < num; i++) {
    E.push_back(E_[i]);
    F.push_back(F_[i]);
  }

  return 0;
}
int spectrum::ini(const vector <double> &E_, const vector <double> &F_) {
  clear_lab();
  E = E_;
  F = F_;
  ini_check();
  return 0;
}
int spectrum::ini(double Es_, double Ee_, double factor_) {
  Es = Es_;
  Ee = Ee_;
  factor = factor_;
  clear_spec();

  for (double Ex = Es; Ex < Ee; Ex *= factor) {
    E.push_back(Ex);
    F.push_back(0);
  }

  return 0;
}
int spectrum::ini(const string &filename, double Eindx) {
  const char annota[2] = "#";
  string line;
  double tmpE, tmpF;

  ifstream datfile(filename);
  if (datfile.fail() || datfile.bad()) {
    cout << "ERROR::spectrum::spectrum:fail to read from file " << filename << endl;
    exit(1);
  }

  for (unsigned nline = 1; getline(datfile, line); nline++) {
    if (line.size() == 0 || line.at(0) == annota[0]) continue;

    istringstream is(line);

    is >> tmpE, E.push_back(tmpE);
    is >> tmpF, F.push_back(tmpF / pow(tmpE, Eindx));
  }

  datfile.close();

  if(!ini_check()) cout << "reading emptyfile " << filename << endl;
  return 0;
}

int spectrum::ini_check() const throw(errtype) {
  if (E.size() != F.size()) {
    cout << "ERROR::spectrum::spectrum:spectrum initializing with wrong vectors" << endl;
    throw(ini_wrong);
  }

  if (E.size() == 0 || F.size() == 0) {
    cout << "WARNING::spectrum::spectrum:you are inputing empty vectors to a spectrum" << endl;
    return 0;
  }
  return 1;
}

int spectrum::comp(const spectrum &another, ostringstream &os) const {
  if (E.size() != another.E.size()) {
    compare(another, os);

  } else {
    os << "#\tE\tF1\tF2" << endl;
    os << setiosflags(ios::scientific) << setprecision(6);
    for (unsigned i = 0; i < E.size(); i++)
      os << E[i] << "\t" << F[i] << "\t" << another.F[i] << endl;
  }
  return 0;
}
int spectrum::comp(const spectrum &another, const string &filename) const {
  ostringstream os;
  comp(another, os);
  return dealoutput(filename, os);
}

int spectrum::compare(const spectrum &another, ostringstream &os) const {
  interp intp(another.E, another.F);

  os << "#\tE\tF1\tF2" << endl;
  os << setiosflags(ios::scientific) << setprecision(6);
  for (unsigned i = 0; i < E.size(); i++)
    os << E[i] << "\t" << F[i] << "\t" << intp.lnask(E[i]) << endl;

  return 0;
}
int spectrum::compare(const spectrum &another, const string &filename) const {
  ostringstream os;
  compare(another, os);
  return dealoutput(filename, os);
}

int spectrum::print(ostringstream &os) const {
  os << setiosflags(ios::scientific) << setprecision(6);
  for (unsigned i = 0; i < E.size(); i++)
    os << E[i] << "\t" << F[i] << endl;

  return 0;
}
int spectrum::print(const string &filename) const {
  ostringstream os;
  print(os);
  return dealoutput(filename, os);
}

int spectrum::dealing(gfunction *func) {
  for (unsigned i = 0; i < E.size(); i++) {
    F[i] = (*func)(E[i], F[i]);
  }

  return 0;
}

int spectrum::add(const spectrum &rhs) {
  interp intp(rhs.E, rhs.F);

  for (unsigned i = 0; i < E.size(); i++) F[i] += intp.lnask(E[i]);

  return 0;
}
int spectrum::substra(const spectrum &rhs) {
  interp intp(rhs.E, rhs.F);

  for (unsigned i = 0; i < E.size(); i++) F[i] -= intp.lnask(E[i]);

  return 0;
}

int spectrum::product(const spectrum &rhs) {
  interp intp(rhs.E, rhs.F);

  for (unsigned i = 0; i < E.size(); i++) F[i] *= intp.lnask(E[i]);

  return 0;
}

int spectrum::divide(const spectrum &rhs) {
  interp intp(rhs.E, rhs.F);

  for (unsigned i = 0; i < E.size(); i++) F[i] /= intp.lnask(E[i]);

  return 0;
}

int spectrum::del_p(unsigned pos) {
  if (pos >= E.size() || pos < 0) {
    cout << "ERROR::spectrum::del_p:The point you want to delete is out of range" << endl;
    return 1;
  }

  E.erase(E.begin() + pos);
  F.erase(F.begin() + pos);
  return 0;
}

extern int get_index(const vector <double> &vec, double x);
int spectrum::add_p(double Ein, double Fin) {
  int pos = get_index(E, Ein);
  E.insert(E.begin() + pos + 1, Ein);
  F.insert(F.begin() + pos + 1, Fin);
  return 0;
}

double spectrum::max() const {
  if (E.size() == 0)
    cout << "ERROR::spectrum::max:You are asking for the max flux of an empty spectrum" << endl;

  double maxflux = F[0];

  for (unsigned i = 1; i < E.size(); i++)
    if (abs(maxflux) < abs(F[i]))
      maxflux = F[i];

  return maxflux;
}
double spectrum::min() const {
  if (E.size() == 0)
    cout << "ERROR::spectrum::min:You are asking for the min flux of an empty spectrum" << endl;

  double minflux = F[0];

  for (unsigned i = 1; i < E.size(); i++)
    if (abs(minflux) > abs(F[i]))
      minflux = F[i];

  return minflux;
}

int spectrum::dealoutput(const string &filename, const ostringstream &os) const {
  if(filename == "null") cout << os.str();
  else {
    ofstream of(filename);
    of << os.str();
    of.close();
  }
  return 0;
}

spectrum& spectrum::operator=(const spectrum& rhs) {
  Es = rhs.Es;
  Ee = rhs.Ee;
  factor = rhs.factor;
  E = rhs.E;
  F = rhs.F;
  return *this;
}

#define NOT_DIVIDE(stms,rhs,res) stms
#define IS_DIVIDE(stms,rhs,res)\
  if(rhs==0) res=0;\
  else stms\

#define OPERFUNC(opers,operse,div_or_not)\
  spectrum operator opers(const double &lhs,const spectrum &rhs){\
    spectrum result(rhs);\
    for(unsigned i=0;i<result.E.size();i++){\
      div_or_not(result.F[i]=lhs opers rhs.F[i],rhs.F[i],result.F[i]);\
    }\
    return result;\
  }\
  spectrum& spectrum::operator operse(const spectrum &rhs){\
    if(E!=rhs.E){\
      cout<<"ERROR::spectrum::"<<#opers<<" or "<<#operse<<":the spectrum in the left hand side and right hand side is different"<<endl\
      << "left: " << E << endl << "right: " << rhs.E << endl;\
      exit(1);\
    }\
    for(unsigned i=0;i<E.size();i++){\
      div_or_not(F[i] operse rhs.F[i],rhs.F[i],F[i]);\
    }\
    return *this;\
  }\
  spectrum& spectrum::operator operse(const double &rhs){\
    for(unsigned i=0;i<E.size();i++){\
      div_or_not(F[i] operse rhs,rhs,F[i]);\
    }\
    return *this;\
  }\
  spectrum spectrum::operator opers(const spectrum &rhs) const{\
    spectrum result(*this);\
    result operse rhs;\
    return result;\
  }\
  spectrum spectrum::operator opers(const double &rhs) const{\
    spectrum result(*this);\
    result operse rhs;\
    return result;\
  }

OPERFUNC(*, *=, NOT_DIVIDE)
OPERFUNC(-, -=, NOT_DIVIDE)
OPERFUNC(+, +=, NOT_DIVIDE)
OPERFUNC( /, /=, IS_DIVIDE)
