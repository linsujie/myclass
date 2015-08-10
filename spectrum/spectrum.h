/*********************************************************************
recording, dealing with and printing a spectrum easily
*********************************************************************/
#ifndef _SPECTURM_H
#define _SPECTURM_H
#include<string>
#include<sstream>
#include"gfunction.h"

class spectrum {
public:
  enum errtype { ini_wrong };
  std::vector <double> E, F;
  double Es, Ee, factor;
  spectrum();
  spectrum(double *E_, double *F_, unsigned num);
  spectrum(const std::vector <double> &E_, const std::vector <double> &F_);
  spectrum(double Es_, double Ee_, double factor_);
  spectrum(const std::string &filename, double Eindx = 0);

  int ini();
  int ini(double *E_, double *F_, unsigned num);
  int ini(const std::vector <double> &E_, const std::vector <double> &F_);
  int ini(double Es_, double Ee_, double factor_);
  int ini(const std::string &filename, double Eindx = 0);

  int print(std::ostringstream &os) const;
  int comp(const spectrum &another, std::ostringstream &os) const;
  int compare(const spectrum &another, std::ostringstream &os) const;

  int print(const std::string &filename = "null") const;
  int comp(const spectrum &another, const std::string &filename = "null") const;
  int compare(const spectrum &another, const std::string &filename = "null") const;

  int dealoutput(const std::string &filename, const std::ostringstream &os) const;

  int dealing(gfunction *func);
  int add(const spectrum &rhs);
  int substra(const spectrum &rhs);
  int product(const spectrum &rhs);
  int divide(const spectrum &rhs);
  int del_p(unsigned pos);
  int add_p(double E, double F);
  double max() const;
  double min() const;

  spectrum& operator=(const spectrum& rhs);
#define SPECTURM_OPERATOR(opers,operse)\
  spectrum& operator operse(const double &rhs);\
  spectrum& operator operse(const spectrum &rhs);\
  spectrum operator opers(const double &rhs) const;\
  spectrum operator opers(const spectrum &rhs) const;\
  friend spectrum operator opers(const double &lhs,const spectrum &rhs);
  SPECTURM_OPERATOR(+, +=)
  SPECTURM_OPERATOR(-, -=)
  SPECTURM_OPERATOR(*, *=)
  SPECTURM_OPERATOR( /, /=)

private:
  int ini_check() const throw(errtype);
  inline int clear_lab();
  inline int clear_spec();
};

spectrum operator *(const double &lhs, const spectrum &rhs);
spectrum operator /(const double &lhs, const spectrum &rhs);
spectrum operator +(const double &lhs, const spectrum &rhs);
spectrum operator -(const double &lhs, const spectrum &rhs);
#endif // for #ifndef _SPECTURM_H
