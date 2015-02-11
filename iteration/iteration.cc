#include<iostream>
#include<cstdlib>
#include"iteration.h"

using std::vector;
using std::cout;
using std::endl;

iteration::iteration(): errs(1e-7) {};

iteration::iteration(double u_tmin, double u_tmax, const std::vector <double> x_, gfunction *func_): x(x_), func(func_), errs(1e-7), size(x_.size()) {
  inibound(u_tmin, u_tmax);
}
iteration::iteration(double u_tmin, double u_tmax, const std::vector <double> x_): x(x_), errs(1e-7), size(x_.size()) {
  inibound(u_tmin, u_tmax);
}


int iteration::iniaxis(double u_tmin, double u_tmax, const std::vector <double> x_) {
  x = x_;
  size = x_.size();
  inibound(u_tmin, u_tmax);
  return 0;
}
int iteration::inifunc(gfunction *func_) {
  func = func_;
  return 0;
}

int iteration::inibound(double u_tmin, double u_tmax) {
  if (3 > size) {
    cout << "iteration::iteration: you can't initialize axis with such a short vector" << endl;
    exit(0);
  }
  u.resize(size);
  u[0] = u_tmin, u[size - 1] = u_tmax;
  return 0;
}

int iteration::seterr(double err_) {
  errs = err_;
  return 0;
}

inline double abs(double x) {
  return x < 0 ? -x : x;
}

/*********************************************************************
  NOTICE: if the iterating rule given is not converge, you may sink
  into infinite loop. The judgement to avoid it is not added for the
  efficiency.
*********************************************************************/
int iteration::run() {
  double err = errs + 1,
         errtmp;
  vector <double> ulast;
  for(; err > errs;) {
    ulast = u;
    (*func)(u, x);
    err = 0;
    for(unsigned i = 1; i < size - 1; i++) {
      errtmp = abs(u[i] - ulast[i] / (u[i] + ulast[i]));
      err = err > errtmp ? err : errtmp;
    }
  }
  return 0;
}

int iteration::print(FILE *stream) const {
  for(unsigned i = 0; i < size; i++) fprintf(stream, "%f\t%f\n", x[i], u[i]);

  return 0;
}

int iteration::print() const {
  return print(stdout);
}

int iteration::print(const char *filename) const {
  FILE *out = fopen(filename, "w");
  print(out);
  fclose(out);
  return 0;
}
