#include "smooth.h"
#include <iostream>
#include <cmath>

using namespace std;

double ln_factor(int n, int m = -1)
{
  if (m > n) return log(0);
  if (m < 0) m = n;

  double lnvalue = 0, temp_value = 1, test_temp_value = temp_value;
  for (int i = n; i > n - m; i--) {
    test_temp_value *= i;
    if (std::isinf(test_temp_value)) {
      lnvalue += log(temp_value);
      temp_value = test_temp_value = i;
    } else temp_value = test_temp_value;
  }

  lnvalue += log(temp_value);

  return lnvalue;
}

double Gram_polynomial(int order, int point_size, int position)
{
  double sum = 0;
  for (int j = 0; j <= order; j++) {
    if (point_size + position < j) continue;

    double term = exp(ln_factor(j + order, 2 * j) + ln_factor(point_size + position, j) - 2 * ln_factor(j) - ln_factor(2 * point_size, j));
    if ((j + order) % 2 != 0) term *= -1;
    sum += term;
  }

  return sum;
}

double h(int order, int point_size, int point_position, int position)
{
  double sum = 0;
  for (int k = 0; k <= order; k++)
    sum += (2 * k + 1) * exp(ln_factor(2 * point_size, k) - ln_factor(2 * point_size + k + 1, k + 1))
      * Gram_polynomial(k, point_size, position) * Gram_polynomial(k, point_size, point_position);

  return sum;
}

enum smooth_spec_even_error { smooth_spec_even_error_value };
enum too_few_data_points { too_few_data_points_value };
bool spec_step_even(const spectrum& spec) {
  if (spec.E.size() < 3) return false;
  double step = spec.E[1] - spec.E[0];

  const double step_tolerance = 1e-6;
  for (unsigned i = 1; i < spec.E.size() - 2; i++) {
    double test_step = spec.E[i +  1] - spec.E[i];
    if (fabs(test_step - step) > step * step_tolerance) return false;
  }

  return true;
}

spectrum smooth(const spectrum& spec, int filter_length, int polynomial_order)
{
  if (!spec_step_even(spec)) throw(smooth_spec_even_error_value);
  if (spec.E.size() < filter_length) throw(too_few_data_points_value);

  spectrum result = spec;

  unsigned point_size = filter_length / 2;
  for (unsigned i = 0; i < result.E.size(); i++) {
    unsigned mid_i = i;
    if (i < point_size) mid_i = point_size;
    else if (i >= spec.F.size() - 1 - point_size) mid_i = spec.F.size() - 1 - point_size;
    int position = i - mid_i;

    result.F[i] = 0;
    for (int ip = - int(point_size); ip <= int(point_size); ip++)
      result.F[i] += h(polynomial_order, point_size, ip, position) * spec.F[mid_i + ip];
  }

  return result;
}

spectrum smooth(const interp& intp, double step_size, int filter_length, int polynomial_order)
{
  double xmin = intp.x_tab[0],
         xmax = intp.x_tab[intp.dim - 1];

  spectrum spec;
  for (double x = xmin; x <= xmax; x += step_size)
    spec.add_p(x, intp.linask(x));

  return smooth(spec, filter_length, polynomial_order);
}
