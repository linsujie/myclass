#ifndef _CHISQ_H
#define _CHISQ_H
/*********************************************************************
  A class to calculate chi2.
  The experiment data should be got from a three column datafile.
  The third column is err for the second column.
  flag=1 when the second column in the file is Flux*E^2.7,
  flag=0 when the second column in the file is just Flux.
 *********************************************************************/
#include<sstream>
#include"spectrum.h"
class chisq {
private:
  static const char annota[2];
  std::vector <std::vector < std::vector<double> > > data;
  std::vector <std::string> dataname;
  int init(const std::string &filename, double Eindx);
public:
  chisq(const std::string &filename, double Eindx = 0);

  int printsizes() const;

  int printdat() const ;
  int printdat(int setnum, bool head = true) const ;

  double chi2(const spectrum &phi, bool flagp = false, const std::string &filename = "null",
              std::ios_base::openmode outmode = std::ios_base::out) const;
  double chi2(const spectrum &phi, bool flagp, std::ostringstream &os) const;

  double chi2(int setnum, const spectrum &phi, bool flagp = false, bool head = true, const std::string &filename = "null", std::ios_base::openmode mode = std::ios_base::out) const;
  double chi2(int setnum, const spectrum &phi, bool flagp, bool head, std::ostringstream &os) const;

  int dealoutput(const std::string &filename, const std::ostringstream &os, std::ios_base::openmode outmode) const;
};
#endif // for #ifndef _CHISQ_H
