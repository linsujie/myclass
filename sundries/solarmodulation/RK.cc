#include <iostream>
#include<cstdlib>
#include<cmath>
#include"RK.h"

//The RK_STMAX is the max step allowed, RK_PARA_DIM is the dimension of the rk-rule arrays
using std::vector;
using std::abs;
using std::cout;
using std::endl;

//The RK4
const double RK:: param_c[RK_PARA_DIM] = {0, 0.5, 0.5, 1};
const double RK:: param_b[RK_PARA_DIM] = {0.1666667, 0.3333333, 0.3333333, 0.1666667};
const double RK:: param_a[RK_PARA_DIM][RK_PARA_DIM] = {{0, 0, 0, 0}, {0.5, 0, 0, 0}, {0, 0.5, 0, 0}, {0, 0, 1, 0}};

/*********************************************************************
  //The 3/8-rule
  const double RK:: param_c[RK_PARA_DIM]={0,0.3333333,0.6666667,1};
  const double RK:: param_b[RK_PARA_DIM]={0.125,0.375,0.375,0.125};
  const double RK:: param_a[RK_PARA_DIM][RK_PARA_DIM]={{0,0,0,0},{0.3333333,0,0,0},{-0.3333333,1,0,0},{1,-1,1,0}};

  //The Dormand-Prince
  const double RK:: param_c[RK_PARA_DIM]={0,0.2,0.3,0.8,0.888889,1,1};
  //  const double RK:: param_b[RK_PARA_DIM]={0.089913194,0,0.45348907,0.6140625,-0.27151238,0.089047619,0.025}; //order 4
  const double RK:: param_b[RK_PARA_DIM]={0.091145833,0,0.4492363,0.65104167,-0.32237618,0.13095238,0}; //order 5
  const double RK:: param_a[RK_PARA_DIM][RK_PARA_DIM]={{0,0,0,0,0,0,0},
  {0.2,0,0,0,0,0,0},
  {0.075,0.225,0,0,0,0,0},
  {0.97777778,-3.7333333,3.5555556,0,0,0,0},
  {2.9525987,-11.595793,9.8228929,-0.29080933,0,0,0},
  {2.8462753,-10.757576,8.9064227,0.27840909,-0.2735313,0,0},
  {0.091145833,0,0.4492363,0.65104167,-0.32237618,0.13095238,0}};

  //The Cash-Karp
  const double RK:: param_c[RK_PARA_DIM]= {0,0.2,0.3,0.6,1,0.875};
  const double RK:: param_b[RK_PARA_DIM]= {0.10217737,0,0.3839079,0.24459274,0.019321987,0.25}; //order 5
  //  const double RK:: param_b[RK_PARA_DIM]= {0.097883598,0,0.40257649,0.21043771,0,0.2891022};//order 4
  const double RK:: param_a[RK_PARA_DIM][RK_PARA_DIM]={{0,0,0,0,0,0},
  {0.2,0,0,0,0,0},
  {0.075,0.225,0,0,0,0},
  {0.3,-0.9,1.2,0,0,0},
  {-0.2037037,2.5,-2.5925926,1.2962963,0,0},
  {0.029495804,0.34179688,0.041594329,0.40034541,0.061767578,0}};
 *********************************************************************/

int RK::setstl(double l, double factor, double err) {
  stlen = l;
  fac = factor;
  errmax = err;
  return 0;
}

int RK::setini(double a_, double b_, double y_) {
  a = a_;
  b = b_;
  yini.clear();
  yini.push_back(y_);
  ytmp.resize(1);

  for (unsigned i = 0; i < RK_PARA_DIM; i++)
    k[i].resize(1);

  return 0;
}
int RK::setini(double a_, double b_, const vector <double> &y_) {
  a = a_;
  b = b_;
  yini = y_;
  ytmp.resize(yini.size());

  for (unsigned i = 0; i < RK_PARA_DIM; i++)
    k[i].resize(yini.size());

  return 0;
}

bool RK::sequence(double x1, double x2, double x3) {
  return (x1 <= x2 && x2 <= x3) || (x1 >= x2 && x2 >= x3);
}

#define RKMAIN(errrt, rt, adsentence)\
  if (fac <= 0) {\
    cout << "The step factor should be positive" << endl;\
    return errrt;\
  }\
  \
  if (fac < 1 && a + stlen / (1 - fac) <= b) {\
    cout << "The range is too large for this factor" << endl;\
    return errrt;\
  }\
  \
  int count = 0;\
  double x = a;\
  double stlentmp = stlen;\
  y = yini;\
  \
  while (sequence(a, x, b) && count <= RK_STMAX) {\
    adsentence;\
    \
    for (int i = 0; i <= RK_PARA_DIM - 1; i++) {\
      ytmp = y;\
      \
      for (int j = 0; j < i; j++) {\
        for (unsigned order = 0; order < y.size(); order++)\
          ytmp[order] += param_a[i][j] * k[j][order];\
      }\
      \
      (*func)(x + param_c[i]*stlentmp, ytmp, k[i]);\
      \
      for (unsigned order = 0; order < y.size(); order++)\
        k[i][order] *= stlentmp;\
    }\
    \
    for (int i = 0; i < RK_PARA_DIM; i++) {\
      for (unsigned order = 0; order < y.size(); order++)\
        y[order] = y[order] + k[i][order] * param_b[i];\
    }\
    \
    x = x + stlentmp;\
    stlentmp = stlentmp * fac;\
    \
    if (abs(k[0][0]) >= abs(errmax) && errmax != 0) stlentmp *= abs(errmax / k[0][0]);\
    \
    if (!sequence(a, x + stlentmp, b) && x != b) stlentmp = b - x; /*To get the value at the point t=b*/\
    \
    count++;\
  }\
  \
  if (count >= RK_STMAX) {\
    cout << "Too many steps to  solve this" << endl;\
    exit(0);\
  }\
  \
  return rt

double RK::dork(gfunction *func, int print_inval) { //R-K main function
  if (print_inval == 0) return dork(func);

  RKMAIN(0, y[0], if (count % print_inval == 0) cout << x << " " << y[0] << endl);
}

double RK::dork(gfunction *func) { //R-K main function
  RKMAIN(0, y[0], );
}

vector <vector <double> > RK::dork_record(gfunction *func) {
  vector <vector <double> > result;
  vector <double> tmp;
  result.resize(yini.size() + 1);
  for (unsigned i = 0; i < result.size(); i++) result[i].reserve(1000);

  RKMAIN(result, result, result[0].push_back(x); for (unsigned y_i = 0; y_i < y.size(); y_i++) result[y_i + 1].push_back(y[y_i]));
}
