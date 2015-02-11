#include<cstdio>
#include<cstdlib>
#include<iostream>
#include"table2D.h"

using std::vector;
using std::endl;
using std::cout;

int table2D::get_pos(double axi_, int x_or_y) const {
  extern int get_index(const vector <double> &vec, const double x);
  int pos = -1;

  if (x_or_y == 1) {
    pos = get_index(x, axi_);

  } else if (x_or_y == 2) {
    pos = get_index(y, axi_);
  }

  return pos;
}
int table2D::search(double axival_, int x_or_y) const {
  int pos = get_pos(axival_, x_or_y);

  if (x_or_y == 1) {
    if (x[pos] != axival_)
      return -1;

  } else if (x_or_y == 2) {
    if (y[pos] != axival_)
      return -1;
  }

  return pos;
}

inline double table2D::dx(int xpos, int ypos) const {
  return (val[xpos - 1][ypos] - val[xpos][ypos]) / (x[xpos - 1] - x[xpos]);
}

inline double table2D::dy(int xpos, int ypos) const {
  return (val[xpos][ypos - 1] - val[xpos][ypos]) / (y[ypos - 1] - y[ypos]);
}

vector <double> x, y;
vector <vector<double> > val;
int table2D::insx(double x_) {
  int pos = get_pos(x_, 1) + 1;
  x.insert(x.begin() + pos, x_);
  val.insert(val.begin() + pos, y);
  return pos;
}
int table2D::insy(double y_) {
  int pos = get_pos(y_, 2) + 1;
  y.insert(y.begin() + pos, y_);

  for (unsigned i = 0; i < val.size(); i++)
    val[i].insert(val[i].begin() + pos, 0);

  return pos;
}

int table2D::insval(int xpos, int ypos, double val_) {
  val[xpos][ypos] = val_;
  return 0;
}

int table2D::insval(double xin_, double yin_, double val_) {
  int xpos, ypos;
  xpos = search(xin_, 1);
  ypos = search(yin_, 2);

  if (xpos == -1 || ypos == -1) {
    cout << "There is no such a point u want to give the value." << endl;
    exit(1);
  }

  insval(xpos, ypos, val_);
  return 0;
}

int table2D::trans() {
  vector <vector<double> > valtmp;
  valtmp.resize(y.size());

  for (unsigned j = 0; j < y.size(); j++) {
    for (unsigned i = 0; i < x.size(); i++)
      valtmp[j].push_back(val[i][j]);
  }

  val.clear();
  val = valtmp;
  x.swap(y);
  return 0;
}

int table2D::list() {
  printf("          ");

  for (unsigned i = 0; i < y.size(); i++)
    printf("%4.3e ", y[i]);

  printf("\n");

  for (unsigned j = 0; j < x.size(); j++) {
    printf("%4.3e ", x[j]);

    for (unsigned i = 0; i < y.size(); i++)
      printf("%4.3e ", val[j][i]);

    printf("\n");
  }

  return 0;
}

int table2D::clear() {
  x.clear();
  y.clear();
  val.clear();
  return 0;
}

double table2D::dx2(double x_, double y_) const {
  int xpos = search(x_, 1),
      ypos = search(y_, 2);

  if (xpos == -1 || ypos == -1) {
    cout << "There is no such a point u want to calc." << endl;
    exit(1);
  }

  return dx2(xpos, ypos);
}
double table2D::dx2(int xpos, int ypos) const {
  if (x.size() < 3) {
    cout << "The data is not enough to calculate the second order derivatives." << endl;
    exit(2);
  }

  if (xpos == 0) xpos++;

  else if (xpos == int(x.size() - 1)) xpos--;

  return (dx(xpos, ypos) - dx(xpos + 1, ypos)) / ((x[xpos - 1] - x[xpos + 1]) / 2);
}

double table2D::dy2(double x_, double y_) const {
  int xpos = search(x_, 1),
      ypos = search(y_, 2);

  if (xpos == -1 || ypos == -1) {
    cout << "There is no such a point u want to calc." << endl;
    exit(1);
  }

  return dy2(xpos, ypos);
}
double table2D::dy2(int xpos, int ypos) const {
  if (y.size() < 3) {
    cout << "The data is not enough to calculate the second order derivatives." << endl;
    exit(2);
  }

  if (ypos == 0) ypos++;

  else if (ypos == int(y.size() - 1)) ypos--;

  return (dy(xpos, ypos) - dy(xpos, ypos + 1)) / ((y[ypos - 1] - y[ypos + 1]) / 2);
}
