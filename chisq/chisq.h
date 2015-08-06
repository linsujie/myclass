#ifndef _CHISQ_H
#define _CHISQ_H
/*********************************************************************
  A class to calculate chi2.
  The experiment data should be got from a three column datafile.
  The third column is err for the second column.
 *********************************************************************/
#include<sstream>
#include"spectrum.h"
class chisq {
protected:
  static const char annota[2];
  static const spectrum zerospec;
  std::vector < std::vector<double> > E, F, sigma, total_sigma;
  std::vector <std::string> dataname;
  int init(const std::string &filename, double Eindx);
  int addexperiment(const std::string &line);

  int dealoutput(const std::string &filename, const std::ostringstream &os, std::ios_base::openmode outmode) const;
public:
  chisq(const std::string &filename, double Eindx = 0);

  int printsizes() const;

  int printdat(const std::string &filename = "null") const ;
  int printdat(const std::vector <int> &setnums, const std::string &filename = "null") const ;
  int printdat(int setnum, const std::string &filename = "null") const ;

  int extra_sigma(const spectrum &sigma_ = zerospec);

  double chi2(const spectrum &phi, bool pflag = false, const std::string &filename = "null",
              std::ios_base::openmode outmode = std::ios_base::out) const;
  double chi2(const std::vector <int> &setnums, const spectrum &phi, bool pflag = false,
              const std::string &filename = "null", std::ios_base::openmode mode = std::ios_base::out) const;
  double chi2(int setnum, const spectrum &phi, bool pflag = false,
              const std::string &filename = "null", std::ios_base::openmode mode = std::ios_base::out) const;
};
#endif // for #ifndef _CHISQ_H
