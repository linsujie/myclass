#include <iostream>
#include<cstdlib>
#include<cmath>
#include"RK.h"

//The RK_STMAX is the max step allowed, RK_PARA_DIM is the dimension of the rk-rule arrays
using std::function;
using std::vector;
using std::abs;
using std::cout;
using std::endl;

//The RK4
const vector<double> rk4_param_c = {0, 0.5, 0.5, 1};
const vector<double> rk4_param_b = {0.1666667, 0.3333333, 0.3333333, 0.1666667};
const vector<vector<double> > rk4_param_a = {{0, 0, 0, 0}, {0.5, 0, 0, 0}, {0, 0.5, 0, 0}, {0, 0, 1, 0}};

//The 3/8-rule
const vector<double> three_to_eight_param_c={0,0.3333333,0.6666667,1};
const vector<double> three_to_eight_param_b={0.125,0.375,0.375,0.125};
const vector<vector<double> > three_to_eight_param_a={{0,0,0,0},{0.3333333,0,0,0},{-0.3333333,1,0,0},{1,-1,1,0}};

//The Dormand-Prince
const vector<double> dormand_prince_param_c={0,0.2,0.3,0.8,0.888889,1,1};
const vector<double> dormand_prince_param_b={0.091145833,0,0.4492363,0.65104167,-0.32237618,0.13095238,0}; //order 5
const vector<vector<double> > dormand_prince_param_a={{0,0,0,0,0,0,0},
  {0.2,0,0,0,0,0,0},
  {0.075,0.225,0,0,0,0,0},
  {0.97777778,-3.7333333,3.5555556,0,0,0,0},
  {2.9525987,-11.595793,9.8228929,-0.29080933,0,0,0},
  {2.8462753,-10.757576,8.9064227,0.27840909,-0.2735313,0,0},
  {0.091145833,0,0.4492363,0.65104167,-0.32237618,0.13095238,0}};

//The Cash-Karp
const vector<double> cash_karp_param_c= {0,0.2,0.3,0.6,1,0.875};
const vector<double> cash_karp_param_b= {0.10217737,0,0.3839079,0.24459274,0.019321987,0.25}; //order 5
const vector<vector<double> > cash_karp_param_a={{0,0,0,0,0,0},
  {0.2,0,0,0,0,0},
  {0.075,0.225,0,0,0,0},
  {0.3,-0.9,1.2,0,0,0},
  {-0.2037037,2.5,-2.5925926,1.2962963,0,0},
  {0.029495804,0.34179688,0.041594329,0.40034541,0.061767578,0}};

int RK::set_rule(RK::rule r) {
  switch(r) {
  case rk4:
      param_a = rk4_param_a; param_b = rk4_param_b; param_c = rk4_param_c; break;

  case three_to_eight:
      param_a = three_to_eight_param_a; param_b = three_to_eight_param_b; param_c = three_to_eight_param_c; break;

  case dormand_prince:
      param_a = dormand_prince_param_a; param_b = dormand_prince_param_b; param_c = dormand_prince_param_c; break;

  case cash_karp:
      param_a = cash_karp_param_a; param_b = cash_karp_param_b; param_c = cash_karp_param_c; break;

  case rule_size:
      break;
  }

  if (param_a.size() == 0) {
    cout << "Error::set_rule::No suitable rule has been chosen, please check;" << endl;
    exit(0);
  }

  k.resize(param_a.size());

  if (!yini.empty()) {
    for (unsigned i = 0; i < k.size(); i++)
      k[i].resize(yini.size());
  }
  return 0;
}

RK::RK(rule r) { set_rule(r); }

int RK::set_step(double step_, double factor_, double maxerr_) {
  step = step_; factor = factor_; maxerr = maxerr_;
  if (factor <= 0) {
    cout << "RK::set_step::Error::The step factor should be positive" << endl;
    exit(0);
  }

  return 0;
}

int RK::set_start(double low_, double up_, double y_) {
  const vector<double> yvec = { y_ };
  return set_start(low_, up_, yvec);
}

int RK::set_start(double low_, double up_, const vector <double> &y_) {
  low = low_; up = up_;
  yini = y_;
  ytmp.resize(yini.size());

  for (unsigned i = 0; i < k.size(); i++)
    k[i].resize(yini.size());
  return 0;
}

bool RK::sequence(double a, double b, double c) {
  return (a <= b && b <= c) || (a >= b && b >= c);
}

vector<vector<double> > RK::dork_core(const function<void(double, const vector<double>&, vector <double>&)>& derive, RK::print_level pflag) {
  if (yini.empty()) {
    cout << "RK::dork::Error::Please set the initial condition before dork" << endl;
    exit(0);
  }
  if (factor < 1 && low + step / (1 - factor) <= up) {
    cout << "RK::set_step::Error::The range [" << low << " - " << up << "] is too wide for the step " << step << " and factor " << factor << endl;
    exit(0);
  }

  vector <vector <double> > result;
  result.resize(yini.size() + 1);

  if (pflag == print_to_vector)
    for (unsigned i = 0; i < result.size(); i++) result[i].reserve(10000);

  int count = 0;
  double x = low;
  double step_tmp = step;
  y = yini;

  while (sequence(low, x, up) && count <= RK_STMAX) {
    if (pflag == print_to_vector) {
      result[0].push_back(x);
      for (unsigned iy = 0; iy < y.size(); iy++) result[iy + 1].push_back(y[iy]);
    } else if (pflag == to_print) cout << x << " " << y[0] << endl;

    for (int i = 0; i < param_a.size(); i++) {
      ytmp = y;

      for (int j = 0; j < i; j++) {
        for (unsigned order = 0; order < y.size(); order++)
          ytmp[order] += param_a[i][j] * k[j][order];
      }

      derive(x + param_c[i] * step_tmp, ytmp, k[i]);

      for (unsigned order = 0; order < y.size(); order++)
        k[i][order] *= step_tmp;
    }

    for (int i = 0; i < param_b.size(); i++) {
      for (unsigned order = 0; order < y.size(); order++)
        y[order] = y[order] + k[i][order] * param_b[i];
    }

    x = x + step_tmp;
    step_tmp = step_tmp * factor;

    if (abs(k[0][0]) >= abs(maxerr) && maxerr != 0) step_tmp *= abs(maxerr / k[0][0]);

    if (!sequence(low, x + step_tmp, up) && x != up) step_tmp = up - x; /*To get the value at the point t=b*/

    count++;
  }

  if (count >= RK_STMAX) {
    cout << "RK::dork::Error::Too many steps to  solve this" << endl;
    exit(0);
  }

  return result;
}

double RK::dork(const std::function<void(double, const vector<double>&, vector <double>&)>& derive, bool pflag) {
  dork_core(derive, print_level(pflag));
  return y[0];
}

vector <vector <double> > RK::dork_record(const function<void(double, const vector<double>&, vector <double>&)>& derive) {
  return dork_core(derive, print_to_vector);
}
