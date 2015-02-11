#ifndef _INTERP2D_H
#define _INTERP2D_H
/*********************************************************************
require table2D, interpolating 2D data, you can import a function,
but that function should not be divergent
*********************************************************************/
#include<cmath>
#include<set>
#include"Table2D.h"
#include"gfunction.h"
class IntpFunction: public gfunction {
private:
  const gfunction *func;
  bool ln_or_not;
public:
  double operator()(double x, double y) const;
  IntpFunction();
  IntpFunction(gfunction *func_, bool ln_or_not_);
  int initial(gfunction *func_, bool ln_or_not_);
};

class Interp2D {
private:
  /*********************************************************************
    typedef map <double, double> Line;
    typedef map <double, double>::const_iterator LineConsIter;
  *********************************************************************/
  enum PointsNo {bl, tl, tr, br};
  Table2D lntab;
  vector <double> chlist;
  int lnmapping();
  int mapping();
  IntpFunction function;
  double tri_intp(double(*[3])[3], double x, double y) const;
  bool tranversx(Table2D &table, double err);
  bool tranversy(Table2D &table, double err);
  int checkaxis(const Table2D::Line &axis, double err);
  double linask(const Table2D &table, double x, double y) const;
  int create_table(Table2D &table, double range[4], double err);
public:
  Table2D tab;
  Interp2D(const Table2D &tab_);
  Interp2D();
  /*********************************************************************
  The range is in the order {x_start,y_start,x_end,y_end}, err is the
  maximal alloweded d^2f/dx^2*\Delta x^2 (or the same term for y)
  in the grids.
  *********************************************************************/
  Interp2D(gfunction *func_, double range[4], double err);
  int creating(gfunction *func_, double range[4], double err);
  int lncreating(gfunction *func_, double range[4], double err);

  double linask(double x, double y) const;
  double lnask(double x, double y) const;
};
#endif // for #ifndef _INTERP2D_H
