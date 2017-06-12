#ifndef _CHISQ_H
#define _CHISQ_H
/*********************************************************************
  A class to calculate chi2.
  The experiment data should be got from a three column datafile.
  The third column is err for the second column.
 *********************************************************************/
#include <sstream>
#include <tuple>
#include <functional>

#ifndef NO_ROOT
#include <TGraphErrors.h>
#endif

#include "pArray.h"
#include "spectrum.h"
#include "interp.h"

template<typename T>
class chisq_interface
{
public:
  void set_nums(const std::vector<int>& setnums_) { whole_setnums = setnums_; }
  void setfunction(const std::function<T(const std::vector<int>&, interp intp, bool, const std::string&)>& func_) { func = func_; }

  T operator()(const spectrum& phi, bool pflag = false, const std::string& filename = "null") const {
    return func(whole_setnums, interp(&(phi.E[0]), &(phi.F[0]), phi.E.size()), pflag, filename);
  }
  T operator()(int setnum, const spectrum& phi, bool pflag = false, const std::string& filename = "null") const {
    return func({ setnum }, interp(&(phi.E[0]), &(phi.F[0]), phi.E.size()), pflag, filename);
  }
  T operator()(const std::vector<int>& setnums, const spectrum& phi, bool pflag = false, const std::string& filename = "null") const {
    return func(setnums, interp(&(phi.E[0]), &(phi.F[0]), phi.E.size()), pflag, filename);
  }

  T operator()(const interp& intp, bool pflag = false, const std::string& filename = "null") const {
    return func(whole_setnums, intp, pflag, filename);
  }
  T operator()(int setnum, const interp& intp, bool pflag = false, const std::string& filename = "null") const {
    return func({ setnum }, intp, pflag, filename);
  }
  T operator()(const std::vector<int>& setnums, const interp& intp, bool pflag = false, const std::string& filename = "null") const {
    return func(setnums, intp, pflag, filename);
  }

  T operator()(const pArray& E_, const pArray& F_, bool pflag = false, const std::string& filename = "null") const {
    return func(whole_setnums, interp(E_.a, F_.a, E_.GetLength()), pflag, filename);
  }
  T operator()(int setnum, const pArray& E_, const pArray& F_, bool pflag = false, const std::string& filename = "null") const {
    return func({ setnum }, interp(E_.a, F_.a, E_.GetLength()), pflag, filename);
  }
  T operator()(const std::vector<int>& setnums, const pArray& E_, const pArray& F_, bool pflag = false, const std::string& filename = "null") const {
    return func(setnums, interp(E_.a, F_.a, E_.GetLength()), pflag, filename);
  }

private:
  std::vector<int> whole_setnums;
  std::function<T(const std::vector<int>&, interp intp, bool, const std::string& filename)> func;
};

class chisq
{
protected:
  static const char annota[2];
  static const spectrum zerospec;
  std::vector<std::string> dataname;
  int init(const std::string& filename, double Eindx);
#ifndef NO_ROOT
  bool init(TGraphErrors* gr, double Eindx = 0);
  bool init(const std::vector<TGraphErrors*>& grs, double Eindx = 0);
#endif
  int addexperiment(const std::string& line);

  int dealoutput(const std::string& filename, const std::ostringstream& os) const;

public:
  std::vector<std::vector<double> > E, F, sigma, total_sigma;
  chisq(const std::string& filename, double Eindx = 0);
  chisq(const chisq& another);

  int printsizes() const;
  unsigned size() const;
  unsigned size(int setnum) const;
  unsigned size(const std::vector<int>& setnums) const;

  int printdat(const std::string& filename = "null") const;
  int printdat(const std::vector<int>& setnums,
               const std::string& filename = "null") const;
  int printdat(int setnum, const std::string& filename = "null") const;

  chisq_interface<double> chi2;
  chisq_interface<std::tuple<double,double> > chi2_RHOVALUE;

  static std::tuple<double,double> RHOVALUE_distribution(const std::vector<unsigned>& Ns);

#ifndef NO_ROOT
  TGraphErrors* GetTGraphErrors(int setnum, double index = 0) const;
  // A TGraphErrors would be newed by this function, please delete it yourself
  chisq(TGraphErrors* gr, double Eindx = 0);
  chisq(const std::vector<TGraphErrors*>& grs, double Eindx = 0);
#endif

  int extra_sigma(const spectrum& sigma_ = zerospec);
  int enlarge_sigma(double scale);

private:
  double chi2_calc(const std::vector<int>& setnums, interp intp, bool pflag, const std::string& filename) const;
  std::tuple<double,double> chi2_RHOVALUE_calc(const std::vector<int>& setnums, interp intp, bool pflag, const std::string& filename) const;
};
#endif // for #ifndef _CHISQ_H
