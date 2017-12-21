#ifndef _QUADRATURE_H
#define _QUADRATURE_H
#include <vector>
#include <functional>

namespace quadrature
{
  enum too_many_steps { too_many_steps_val };
  enum not_a_number { not_a_number_val };
}

void polint(const std::vector<double>& xa, const std::vector<double>& ya, const double x, double &y, double &dy);
double qsimp(const std::function <double(const double)>& func, double low, double up, double tol);
double qromo(const std::function <double(const double)>& func, const double a, const double b, double err);
double qromb(const std::function <double(const double)>& func, double a, double b, double err);
double qadaptive(const std::function <double(const double)>& func, double low, double up, double err);
double qadaptive_positive(const std::function <double(const double)>& func, double low, double up, double err);
#endif // for #ifndef _QUADRATURE_H
