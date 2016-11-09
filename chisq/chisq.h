#ifndef _CHISQ_H
#define _CHISQ_H
/*********************************************************************
  A class to calculate chi2.
  The experiment data should be got from a three column datafile.
  The third column is err for the second column.
 *********************************************************************/
#include <sstream>

#ifndef NO_ROOT
#include <TGraphErrors.h>
#endif

#include "pArray.h"
#include "spectrum.h"
class chisq {
  protected:
  static const char annota[2];
  static const spectrum zerospec;
  std::vector<std::vector<double> > E, F, sigma, total_sigma;
  std::vector<std::string> dataname;
  int init(const std::string& filename, double Eindx);
#ifndef NO_ROOT
  bool init(TGraphErrors* gr, double Eindx = 0);
  bool init(const std::vector<TGraphErrors*>& grs, double Eindx = 0);
#endif
  int addexperiment(const std::string& line);

  int dealoutput(const std::string& filename, const std::ostringstream& os, std::ios_base::openmode outmode) const;

  public:
  chisq(const std::string& filename, double Eindx = 0);
  chisq(const chisq& another);

  int printsizes() const;

  int printdat(const std::string& filename = "null") const;
  int printdat(const std::vector<int>& setnums, const std::string& filename = "null") const;
  int printdat(int setnum, const std::string& filename = "null") const;

#ifndef NO_ROOT
  TGraphErrors* GetTGraphErrors(int setnum, double index = 0) const; // A TGraphErrors would be newed by this function, please delete it yourself
  chisq(TGraphErrors* gr, double Eindx = 0);
  chisq(const std::vector<TGraphErrors*>& grs, double Eindx = 0);
#endif

  int extra_sigma(const spectrum& sigma_ = zerospec);

  double chi2(const pArray& E_, const pArray& F_, bool pflag = false,
              const std::string& filename = "null", std::ios_base::openmode mode = std::ios_base::out) const;
  double chi2(const std::vector<int>& setnums, const pArray& E_, const pArray& F_, bool pflag = false,
              const std::string& filename = "null", std::ios_base::openmode mode = std::ios_base::out) const;
  double chi2(int setnum, const pArray& E_, const pArray& F_, bool pflag = false,
              const std::string& filename = "null", std::ios_base::openmode mode = std::ios_base::out) const;

  double chi2(const spectrum& phi, bool pflag = false,
              const std::string& filename = "null", std::ios_base::openmode mode = std::ios_base::out) const;
  double chi2(const std::vector<int>& setnums, const spectrum& phi, bool pflag = false,
              const std::string& filename = "null", std::ios_base::openmode mode = std::ios_base::out) const;
  double chi2(int setnum, const spectrum& phi, bool pflag = false,
              const std::string& filename = "null", std::ios_base::openmode mode = std::ios_base::out) const;

  private:
  double chi2(int setnum, const double* E_, const double* F_, int nsize, bool pflag = false,
              const std::string& filename = "null", std::ios_base::openmode mode = std::ios_base::out) const;
};
#endif // for #ifndef _CHISQ_H
