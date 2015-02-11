/*********************************************************************
recording, dealing with and printing a spectrum easily
*********************************************************************/
#ifndef _SPECTURM_H
#define _SPECTURM_H
#include<string>
#include"gfunction.h"

class spectrum {
private:
  int ini_check() const;
  int print(FILE *stream) const;
  int comp(const spectrum &another, FILE *stream) const;
  int compare(const spectrum &another, FILE *stream) const;
  inline int clear_lab();
  inline int clear_spec();
public:
  std::vector <double> E, F;
  double Es, Ee, factor;
  spectrum();
  spectrum(double *E_, double *F_, unsigned num);
  spectrum(const std::vector <double> &E_, const std::vector <double> &F_);
  spectrum(double Es_, double Ee_, double factor_);
  spectrum(const char *filename);

  int ini();
  int ini(double *E_, double *F_, unsigned num);
  int ini(const std::vector <double> &E_, const std::vector <double> &F_);
  int ini(double Es_, double Ee_, double factor_);

  int comp(const spectrum &another) const;
  int comp(const spectrum &another, const std::string &filename) const;
  //int comp(const spectrum &another, char *filename) const;
  int compare(const spectrum &another) const;
  int compare(const spectrum &another, const std::string &filename) const;
  //int compare(const spectrum &another, char *filename) const;
  int print() const;
  int print(const std::string &filename) const;
  //int print(const char *filename) const;
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
  spectrum operator opers(const double &rhs);\
  spectrum operator opers(const spectrum &rhs);\
  friend spectrum operator opers(const double &lhs,const spectrum &rhs);
  SPECTURM_OPERATOR(+, +=)
  SPECTURM_OPERATOR(-, -=)
  SPECTURM_OPERATOR(*, *=)
  SPECTURM_OPERATOR( /, /=)
};

spectrum operator *(const double &lhs, const spectrum &rhs);
spectrum operator /(const double &lhs, const spectrum &rhs);
spectrum operator +(const double &lhs, const spectrum &rhs);
spectrum operator -(const double &lhs, const spectrum &rhs);
#endif // for #ifndef _SPECTURM_H
