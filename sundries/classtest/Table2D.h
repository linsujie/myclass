#ifndef _TABLE2D_H
#define _TABLE2D_H
#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<cmath>
#include<map>
#include"gfunction.h"
class Table2D {
public:
  typedef std::pair <double, double> Pair;
  typedef std::map <double, double> Line;
  typedef std::map <double, double>::iterator LineIter;
  typedef std::map <double, double>::const_iterator LineConsIter;
  typedef std::map <double, Line> Table;
  typedef std::map <double, Line>::iterator TabIter;
  typedef std::map <double, Line>::const_iterator TabConsIter;

  Line xaxis, yaxis;
  Table value;

  Table2D() {};
  Table2D(gfunction *func_);

  int setfunc(gfunction *func_);
  int insline(double x_);
  int inscolm(double y_);
  int trans();
  int list();
  int clear();
private:
  LineConsIter getlineiter(double axival, const Line &line) const;
  TabConsIter gettabiter(double xval) const;
  void notfoundwarn() const;
  const gfunction *func;

  double dx2(TabConsIter rowmiter, LineConsIter yiter) const;
  double dy2_core(TabConsIter rowmiter, LineConsIter yiter) const;
  double dy2(TabConsIter rowmiter, LineConsIter yiter) const;
  double dx2(TabConsIter rowiter) const;
  double dy2(LineConsIter yiter) const;
};
#endif // for #ifndef _TABLE2D_H
