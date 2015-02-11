#ifndef _GFUNCTION_H
#define _GFUNCTION_H
/*********************************************************************
To provide the interface for a generic function
 *********************************************************************/
#include<vector>
class gfunction {
public:
  virtual double operator()(double x, double y);
  virtual double operator()(double x, double y) const;
  virtual int operator()(double x, std::vector <double> &y1, std::vector <double> &y2);
  virtual int operator()(double x, std::vector <double> &y1, std::vector <double> &y2) const;
};
#endif // for #ifndef _GFUNCTION_H
