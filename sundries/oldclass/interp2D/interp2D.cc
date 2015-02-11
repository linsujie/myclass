#include<cmath>
#include<iostream>
#include<cstdlib>
#include"interp2D.h"

using std::cout;
using std::endl;
using std::abs;

int interp2D::lntab_mapping() {
  for (unsigned i = 0; i < tab.x.size(); i++)
    lntab.insx(log(fmax(tab.x[i], 1e-300)));

  for (unsigned j = 0; j < tab.y.size(); j++)
    lntab.insy(log(fmax(tab.y[j], 1e-300)));

  for (unsigned i = 0; i < tab.x.size(); i++)
    for (unsigned j = 0; j < tab.y.size(); j++)
      lntab.insval(int(i), int(j), log(fmax(tab.val[i][j], 1e-300)));

  return 0;
}

int interp2D::tab_mapping() {
  for (unsigned i = 0; i < lntab.x.size(); i++)
    tab.insx(exp(lntab.x[i]));

  for (unsigned j = 0; j < lntab.y.size(); j++)
    tab.insy(exp(lntab.y[j]));

  for (unsigned i = 0; i < lntab.x.size(); i++)
    for (unsigned j = 0; j < lntab.y.size(); j++)
      tab.insval(int(i), int(j), exp(lntab.val[i][j]));

  return 0;
}

inline double interp2D::tri_intp(double sele_point[3][3], double x, double y) const {
  return (sele_point[0][2] + sele_point[1][2]) / 2 +
         (y - sele_point[0][1]) * ((sele_point[0][2] + sele_point[1][2]) / 2 - sele_point[2][2]) / (sele_point[0][1] - sele_point[2][1]) +
         (x - sele_point[2][0]) * (sele_point[0][2] - sele_point[1][2]) / (sele_point[0][0] - sele_point[1][0]);
}
/*********************************************************************
To interpolate in a rectangle area, we first divide it into  four triangle and interpolating in the corresponding triangle
The area defined like below:
**************************************************
  (0,-1) p1      ->             (0,0) p3
     *                           *
        *         A1          *
           *               *
              *         *
                 *   *
          A0       *       A3
                 *   *
              *         *
           *       A2      *
        *                     *
     *                           *
  (-1,-1) p0        <-          (-1,0) p2
*********************************************************************/
double interp2D::linask(double x, double y) const {
  int xpos = tab.get_pos(x, 1) + 1,
      ypos = tab.get_pos(y, 2) + 1;
  xpos = xpos < 1 ? 1 : (xpos < int(tab.x.size())) ? xpos : xpos - 1;
  ypos = ypos < 1 ? 1 : (ypos < int(tab.x.size())) ? ypos : ypos - 1;
  double midx = (tab.x[xpos] + tab.x[xpos - 1]) / 2.,
         midy = (tab.y[ypos] + tab.y[ypos - 1]) / 2.,
         midval = (tab.val[xpos][ypos] + tab.val[xpos][ypos - 1]
                   + tab.val[xpos - 1][ypos] + tab.val[xpos - 1][ypos - 1]) / 4.;
  int Aind = 0;
  double k = (tab.y[ypos] - tab.y[ypos - 1]) / (tab.x[xpos] - tab.x[xpos - 1]);

  if (y - tab.y[ypos] - (x - tab.x[xpos])*k < 0) Aind += 2;

  if (y - tab.y[ypos] + (x - tab.x[xpos - 1])*k > 0) Aind += 1;

  double sele_point[3][3];

  if (Aind == 0 || Aind == 3) {
    sele_point[0][0] = tab.x[xpos + point[Aind][0]];
    sele_point[0][1] = tab.y[ypos + point[Aind][1]];
    sele_point[0][2] = tab.val[xpos + point[Aind][0]][ypos + point[Aind][1]];
    sele_point[1][0] = tab.x[xpos + point[arrow[Aind]][0]];
    sele_point[1][1] = tab.y[ypos + point[arrow[Aind]][1]];
    sele_point[1][2] =
      tab.val[xpos + point[arrow[Aind]][0]][ypos + point[arrow[Aind]][1]];
    sele_point[2][0] = midx;
    sele_point[2][1] = midy;
    sele_point[2][2] = midval;

  } else {
    sele_point[0][1] = tab.x[xpos + point[Aind][0]];
    sele_point[0][0] = tab.y[ypos + point[Aind][1]];
    sele_point[0][2] = tab.val[xpos + point[Aind][0]][ypos + point[Aind][1]];
    sele_point[1][1] = tab.x[xpos + point[arrow[Aind]][0]];
    sele_point[1][0] = tab.y[ypos + point[arrow[Aind]][1]];
    sele_point[1][2] =
      tab.val[xpos + point[arrow[Aind]][0]][ypos + point[arrow[Aind]][1]];
    sele_point[2][1] = midx;
    sele_point[2][0] = midy;
    sele_point[2][2] = midval;
    double tmp;
    tmp = x;
    x = y;
    y = tmp;
  }

  return tri_intp(sele_point, x, y);
}

double interp2D::lnask(double x, double y) const {
  if (x <= 0 || y <= 0) cout << "Error::interp2D::lnask: x or y out of range" << endl;

  x = log(fmax(x, 1e-300));
  y = log(fmax(y, 1e-300));
  int xpos = lntab.get_pos(x, 1) + 1,
      ypos = lntab.get_pos(y, 2) + 1;
  xpos = xpos < 1 ? 1 : (xpos < int(lntab.x.size())) ? xpos : xpos - 1;
  ypos = ypos < 1 ? 1 : (ypos < int(lntab.x.size())) ? ypos : ypos - 1;
  double midx = (lntab.x[xpos] + lntab.x[xpos - 1]) / 2.,
         midy = (lntab.y[ypos] + lntab.y[ypos - 1]) / 2.,
         midval = (lntab.val[xpos][ypos] + lntab.val[xpos][ypos - 1]
                   + lntab.val[xpos - 1][ypos] + lntab.val[xpos - 1][ypos - 1]) / 4.;
  int Aind = 0;
  double k = (lntab.y[ypos] - lntab.y[ypos - 1]) / (lntab.x[xpos] - lntab.x[xpos - 1]);

  if (y - lntab.y[ypos] - (x - lntab.x[xpos])*k < 0) Aind += 2;

  if (y - lntab.y[ypos] + (x - lntab.x[xpos - 1])*k > 0) Aind += 1;

  double sele_point[3][3];

  if (Aind == 0 || Aind == 3) {
    sele_point[0][0] = lntab.x[xpos + point[Aind][0]];
    sele_point[0][1] = lntab.y[ypos + point[Aind][1]];
    sele_point[0][2] = lntab.val[xpos + point[Aind][0]][ypos + point[Aind][1]];
    sele_point[1][0] = lntab.x[xpos + point[arrow[Aind]][0]];
    sele_point[1][1] = lntab.y[ypos + point[arrow[Aind]][1]];
    sele_point[1][2] =
      lntab.val[xpos + point[arrow[Aind]][0]][ypos + point[arrow[Aind]][1]];
    sele_point[2][0] = midx;
    sele_point[2][1] = midy;
    sele_point[2][2] = midval;

  } else {
    sele_point[0][1] = lntab.x[xpos + point[Aind][0]];
    sele_point[0][0] = lntab.y[ypos + point[Aind][1]];
    sele_point[0][2] = lntab.val[xpos + point[Aind][0]][ypos + point[Aind][1]];
    sele_point[1][1] = lntab.x[xpos + point[arrow[Aind]][0]];
    sele_point[1][0] = lntab.y[ypos + point[arrow[Aind]][1]];
    sele_point[1][2] =
      lntab.val[xpos + point[arrow[Aind]][0]][ypos + point[arrow[Aind]][1]];
    sele_point[2][1] = midx;
    sele_point[2][0] = midy;
    sele_point[2][2] = midval;
    double tmp;
    tmp = x;
    x = y;
    y = tmp;
  }

  return exp(tri_intp(sele_point, x, y));
}

double interp2D::func_interface(double x, double y) {
  if (ln_or_not)
    return log(fmax((*func)(exp(x), exp(y)), 1e-300));

  else
    return (*func)(x, y);
}

int interp2D::addline(table2D &table, double axi) {
  int pos = table.insx(axi);

  for (unsigned i = 0; i < table.y.size(); i++)
    table.insval(pos, i, func_interface(axi, table.y[i]));

  return 0;
}

int interp2D::addcol(table2D &table, double axi) {
  int pos = table.insy(axi);

  for (unsigned i = 0; i < table.x.size(); i++)
    table.insval(i, pos, func_interface(table.x[i], axi));

  return 0;
}

//addline to the table until we should not add any more. return false if nothing was done.
bool interp2D::tranversx(table2D &table, double err) {
  bool mark = true;
  int ind;

  for (ind = 0; mark; ind++) {
    chlist.clear();
    int lastpushed = -5;

    for (int i = 1; i < int(table.x.size() - 1); i++) {
      double dx2max = 0,
             dx2;

      for (int j = 0; j < int(table.y.size()); j++) {
        dx2 = abs(table.dx2(i, j));
        dx2max = (dx2max > dx2) ? dx2max : dx2;
      }

      if (dx2max * (table.x[i + 1] - table.x[i - 1]) * (table.x[i + 1] - table.x[i - 1]) > err) {
        if (lastpushed != (i - 1))
          chlist.push_back((table.x[i - 1] + table.x[i]) / 2);

        chlist.push_back((table.x[i + 1] + table.x[i]) / 2);
        lastpushed = i;
      }
    }

    if (chlist.empty()) mark = false;

    for (unsigned i = 0; i < chlist.size(); i++)
      addline(table, chlist[i]);
  }

  //ind==1 means that scanx return false in the first time, so noline was added.
  if (ind == 1)
    return false;

  else
    return true;
}

bool interp2D::tranversy(table2D &table, double err) {
  bool mark = true;
  int ind;

  for (ind = 0; mark; ind++) {
    chlist.clear();
    int lastpushed = -5;

    for (int j = 1; j < int(table.y.size() - 1); j++) {
      double dy2max = 0,
             dy2;

      for (int i = 0; i < int(table.x.size()); i++) {
        dy2 = abs(table.dy2(i, j));
        dy2max = (dy2max > dy2) ? dy2max : dy2;
      }

      if (dy2max * (table.y[j + 1] - table.y[j - 1]) * (table.y[j + 1] - table.y[j - 1]) > err) {
        if (lastpushed != (j - 1))
          chlist.push_back((table.y[j - 1] + table.y[j]) / 2);

        chlist.push_back((table.y[j + 1] + table.y[j]) / 2);
        lastpushed = j;
      }
    }

    if (chlist.empty()) mark = false;

    for (unsigned i = 0; i < chlist.size(); i++)
      addcol(table, chlist[i]);
  }

  if (ind == 1)
    return false;

  else
    return true;
}

int interp2D::create_tab(gfunction *func_, double range[4], double err) {
  tab.clear();
  lntab.clear();
  func = func_;
  ln_or_not = false;
  double midx = (range[0] + range[2]) / 2,
         midy = (range[1] + range[3]) / 2;
  addline(tab, range[0]);
  addline(tab, range[2]);
  addline(tab, midx);
  addcol(tab, range[1]);
  addcol(tab, range[3]);
  addcol(tab, midy);

  bool xchange = true,
       ychange = true;

  while (xchange || ychange) {
    xchange = tranversx(tab, err);
    ychange = tranversy(tab, err);
  }

  cout << "x size is " << tab.x.size() << " y size is " << tab.y.size() << endl;
  lntab_mapping();
  return 0;
}

int interp2D::lncreate_tab(gfunction *func_, double range[4], double err) {
  tab.clear();
  lntab.clear();
  func = func_;
  ln_or_not = true;
  double lnrange[4];

  for (int i = 0; i < 4; i++) {
    if (range[i] <= 0) cout << "Error::interp2D::lncreate_tab:range wrong" << endl;

    lnrange[i] = log(range[i]);
  }

  double midx = (lnrange[0] + lnrange[2]) / 2,
         midy = (lnrange[1] + lnrange[3]) / 2;
  addline(lntab, lnrange[0]);
  addline(lntab, lnrange[2]);
  addline(lntab, midx);
  addcol(lntab, lnrange[1]);
  addcol(lntab, lnrange[3]);
  addcol(lntab, midy);

  bool xchange = true,
       ychange = true;

  while (xchange || ychange) {
    xchange = tranversx(lntab, err);
    ychange = tranversy(lntab, err);
  }

  cout << "x size is " << lntab.x.size() << " y size is " << lntab.y.size() << endl;
  tab_mapping();
  return 0;
}

interp2D::interp2D(gfunction *func_, double range[4], double err) {
  create_tab(func_, range, err);
}

interp2D::interp2D() {}

interp2D::interp2D(const table2D &tab_): tab(tab_) {
  lntab_mapping();
}

const int interp2D::arrow[4] = {1, 3, 0, 2},
interp2D::point[4][2] = { { -1, -1}, {0, -1}, { -1, 0}, {0, 0} };
