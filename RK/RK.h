/*********************************************************************
This class is to integrate a function with RK method
*********************************************************************/

#ifndef _RK_H
#define _RK_H

#include <functional>
#include <vector>

#define RK_STMAX 100000000
class RK {
private:
  std::vector<std::vector <double> > k;
  double step, low, up, factor, maxerr;
  bool sequence(double a, double b, double c);
  std::vector <double> ytmp, yini;
  std::vector<double> param_c, param_b;
  std::vector<std::vector<double> > param_a;
  enum print_level { no_print, to_print, print_to_vector };
  std::vector <std::vector <double> > dork_core(const std::function<void(double, const std::vector<double>&, std::vector <double>&)>& derive, print_level pflag);

public:
  enum rule { rk4, three_to_eight, dormand_prince, cash_karp, rule_size };
  std::vector <double> y;

  RK(rule r = rk4);

  int set_rule(rule r);
  int set_step(double step_, double factor_, double maxerr_); //Set step length; errmax is to set the max step locally with the limit stlen<=errmax/k[0], when errmax is 0, the step would not be limited
  int set_start(double low_, double up_, double y0);
  int set_start(double low_, double up_, const std::vector <double> &y0); //Set integral range (a,b) and initial value y

  double dork(const std::function<void(double, const std::vector<double>&, std::vector <double>&)>& derive, bool pflag = false);
  std::vector <std::vector <double> > dork_record(const std::function<void(double, const std::vector<double>&, std::vector <double>&)>& derive);
};

#endif // for #ifndef _RK_H
