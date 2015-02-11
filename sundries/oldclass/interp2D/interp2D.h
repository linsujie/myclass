#ifndef _INTERP2D_H
#define _INTERP2D_H
/*********************************************************************
require table2D, interpolating 2D data, you can import a function,
but that function should not be divergent
*********************************************************************/
#include"table2D.h"
#include"gfunction.h"

class interp2D {
private:
  static const int point[4][2], arrow[4];
  table2D lntab;
  std::vector <double> chlist;
  int lntab_mapping();
  int tab_mapping();
  gfunction *func;
  bool ln_or_not;
  double func_interface(double x, double y);
  inline double tri_intp(double [3][3], double x, double y) const;
  int addline(table2D &table, double axis);
  int addcol(table2D &table, double axis);
  bool tranversx(table2D &table, double err);
  bool tranversy(table2D &table, double err);
public:
  table2D tab;
  interp2D(const table2D &tab_);
  interp2D();
  /*********************************************************************
  The range is in the order {x_start,y_start,x_end,y_end}, err is the
  maximal alloweded d^2f/dx^2*\Delta x^2 (or the same term for y)
  in the grids.
  *********************************************************************/
  interp2D(gfunction *func_, double range[4], double err);
  int create_tab(gfunction *func_, double range[4], double err);
  int lncreate_tab(gfunction *func_, double range[4], double err);

  double linask(double x, double y) const;
  double lnask(double x, double y) const;
};
#endif // for #ifndef _INTERP2D_H
