#ifndef EXP_DATA_H
#define EXP_DATA_H
/*********************************************************************
  A class to load the experiment data and calculate the chi2 or rho
  with a spectrum given.
 *********************************************************************/
#include <iostream>
#include <ostream>
#include <sstream>
#include <tuple>
#include <functional>

#ifndef NO_ROOT
#include <TGraphErrors.h>
#endif

#include "pArray.h"
#include "spectrum.h"
#include "interp.h"

class exp_data_statistic
{
  private:
    const std::vector<bool> *adopted;

  public:
    std::vector<std::vector<double> > diff_table;
    std::vector<double> chi2_values, rho_values;

    void set_adopted(const std::vector<bool> *adopted_);

    double chi2();
    double rho();
    friend std::ostream& operator<<(std::ostream& os, const exp_data_statistic& result);
};

class exp_data
{
protected:
  static const char annota[2];
  std::vector<std::string> dataname_;
  std::vector<bool> adopted;
  bool pflag;
  std::ostream *os;
  std::vector<unsigned> low_index, up_index;

  bool init(const std::string& filename, double Eindx);
#ifndef NO_ROOT
  bool add_TGraphErrors(const TGraphErrors* gr, double Eindx = 0);
  bool init(const TGraphErrors* gr, double Eindx = 0);
  bool init(const std::vector<TGraphErrors*>& grs, double Eindx = 0);
#endif
  int addexperiment(const std::string& line);
  void format_dataname();

public:
  std::vector<std::vector<double> > E, F, sigma, total_sigma;
  exp_data();
  template<typename T> exp_data(const T& object, double Eindx = 0, bool pflag_ = false);
  template<typename T> exp_data(const T* object, double Eindx = 0, bool pflag_ = false);
  exp_data(const exp_data& another);

#ifndef NO_ROOT
  TGraphErrors* GetTGraphErrors(int setnum, double index = 0) const;
  // A TGraphErrors would be newed by this function, please delete it yourself
#endif

  void choose_subset_str(const std::vector<std::string>& setnames);
  void choose_subset_int(const std::vector<int>& setnums);
  void set_range(double lowcut, double upcut);
  void set_print(bool pflag_ = true, std::ostream& os_ = std::cout);

  void enlarge_sigma(double scale, int setnum = -1);
  void extra_sigma(const spectrum& sigma_, int setnum);
  void reset_sigma();

  const std::string& dataname(int setnum = 0) const;
  int summary() const;
  unsigned size(int setnum) const;
  unsigned size() const;
  friend std::ostream& operator<<(std::ostream& os, const exp_data& data);

  // Comparing the data with input spectrum, obtain the result statistic information and
  // return chisq between them.
  double compare(const pArray& E, const pArray& F, exp_data_statistic& result) const;
  double compare(const spectrum& spec, exp_data_statistic& result) const;
  double compare(interp& intp, exp_data_statistic& result) const;
  double compare(const std::function<double(double)>& func, exp_data_statistic& result) const;

  static std::tuple<double,double> rho_distribution(const std::vector<unsigned>& Ns);
};

#endif /* EXP_DATA_H */
