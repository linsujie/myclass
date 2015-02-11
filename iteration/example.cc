#include<iostream>
#include<cstdio>
#include<vector>
#include<cmath>
#include"iteration.h"
#include"tridiagmatrix.h"

using namespace std;

class iterfunc : public gfunction {
  int operator ()(vector <double> &u, vector <double> &t, unsigned i) {
    double delta1 = t[i] - t[i - 1],
           delta2 = t[i + 1] - t[i],
           deltat = t[i + 1] - t[i - 1];

    double a2 = 1,
           a1 = -1,
           a0 = 0,
           c = 0,
           omega = 0.5;


    u[i] = (u[i + 1] * (2 * a2 / (delta2 * deltat) + a1 / (2 * delta2))
          + u[i - 1] * (2 * a2 / (delta1 * deltat) - a1 / (2 * delta1)))
          * (2 * delta1 * delta2 / (4 * a2 + (delta1 - delta2) * a1) - a0);
    return 0;
  }
};

double inline V(double r) {// from Potgieter et. al 2014 1302.1284
  return 2.6756e-6 * (1 - exp(- 13.3 * (r - 0.0046))); //r in Au/s  4e5/1.49498e11,  0.0046 is the radius of sun
}
inline double A1(double r) {
  return 1;//- V(r) * r * r / (7.12782e-5 * 7.09) + r; // According to Gleeson 1968, the relation between phi and rmax is phi=\int_{rmin}^{rmax}dr V(r)/(3k0)
}

inline double A2(double r) {
  return 0;// r * r;
}
inline double A0(double r) {
  return - 2 / r;//0.5 * V(r) * r / (7.12782e-5 * 7.09) - 0.25;
}

int calc(vector <double> &u, vector <double> &t, double ulow, double uup) {
  int size = t.size();
  u.resize(size);
  u[0] = ulow, u[size - 1] = uup;

  size -= 2;
  long double d[size], a[size], b[size], c[size];

  for(int i = 1; i <= size; i++) {
    double dt1 = t[i] - t[i - 1],
           dt2 = t[i + 1] - t[i],
           dt =(t[i + 1] - t[i - 1]) / 2;

    a[i - 1] = A2(t[i]) / (dt1 * dt) - A1(t[i]) / (2 * dt1);
    b[i - 1] = (A1(t[i]) * (dt2 - dt1) - 4 * A2(t[i])) / (2 * dt1 * dt2) + A0(t[i]);
    c[i - 1] = A2(t[i]) / (dt2 * dt) + A1(t[i]) / (2 * dt2);
    d[i - 1] = 0;
  cout << d[0] 
    << " a "    << a[i - 1] << "\t"
    << " b "    << b[i - 1] << "\t"
    << " c "    << c[i - 1] << "\t"
    << " d "    << d[i - 1] << "\t"
    << " fij "  << A2(t[i]) << "\t"
    << " gij "  << A1(t[i]) << "\t"
    << " V(r) " << V(t[i])  << "\t"
    << " r "    << t[i] << endl;
  }

  cout << " uup " << uup << " " << c[size - 1] << endl;
  d[0] += - ulow * a[0], d[size - 1] += - uup * c[size - 1];
  cout << d[0] << " and " << d[size - 1] << endl;

  long double abak[size], bbak[size], cbak[size];
  for(unsigned i = 0; i < size; i++) {
    abak[i] = a[i];
    bbak[i] = b[i];
    cbak[i] = c[i];
  }

  tridiagmatrix_solve(a, b, c, d, size);

  for(unsigned i = 1; i < size - 1; i++)
    cout << " rhs is " << abak[i] * d[i - 1] << " " << bbak[i] * d[i] << " " << cbak[i] * d[i + 1] << " " << abak[i] * d[i - 1] + bbak[i] * d[i] + cbak[i] * d[i + 1] << endl;

  for(int i = 0; i < size; i++) u[i + 1] = d[i];
  return 0;
}

int main() {
  vector <double> t, u, dudt, dudt2;
  iterfunc func;
  unsigned size = 300;
  double factor = pow(80000, 1.0 / 299);
  t.resize(size);
  t[0] = 0;
  t[1] = factor * 0.001;
  for (unsigned i = 2; i < size; i++) t[i] = t[i - 1] * factor;
  cout << "t_max is " << t[size - 1] << endl;
  //iteration iter(exp(t[0]), exp(t[size - 1]), t, &func);
  //iter.seterr(1e-8);
  //iter.run();
  calc(u, t, 0, 1 * sqrt(80) * 0.46284);

  dudt.resize(size);
  dudt2.resize(size);
  for(int i = 1; i < size - 1; i++) {
    dudt[i] = ((u[i] - u[i - 1]) / (t[i] - t[i - 1]) + (u[i + 1] - u[i]) / (t[i + 1] - t[i])) / 2;
    dudt2[i] = ((u[i + 1] - u[i]) / (t[i + 1] - t[i]) - (u[i] - u[i - 1]) / (t[i] - t[i - 1])) * 2 / (t[i + 1] - t[i - 1]);
  }

  FILE *tmp;
  tmp = fopen("temp", "w");
  for (unsigned i = 0; i < size; i++) fprintf(tmp, "%f %f %f %f %f %f\n", t[i], t[i] * t[i], u[i], u[i] / sqrt(t[i]), dudt2[i], A2(t[i]) * dudt2[i] + A1(t[i]) * dudt[i] + A0(t[i]) * u[i]);
  /*********************************************************************
    iter.print("tmp");
    for(unsigned i = 1; i < size - 1; i++) {
    vector <double> u = iter.u;
    double df1 = iter.u[i] - iter.u[i-1],
           df2 = iter.u[i+1] - iter.u[i],
           dt1 = t[i] - t[i-1],
           dt2 = t[i+1] - t[i],
           dt = (t[i+1] - t[i-1]) / 2;
    cout << (df2 / dt2 - df1 / dt1) / dt + (df2 / dt2 + df1/ dt1) / 2 << endl;
    }
   *********************************************************************/
  return 0;
}
