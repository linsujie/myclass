#include <cmath>
#include <vector>
#include <cstdlib>
#include <iostream>

#include "quadrature.h"

using namespace std;

void polint(const vector<double>& xa, const vector<double>& ya, const double x, double &y, double &dy)
{

  int i,m,ns=0;
  double den,dif,dift,ho,hp,w;

  int n=xa.size();
  vector<double> c(n),d(n);
  dif=fabs(x-xa[0]);

  for (i=0;i<n;i++) {

    if ((dift=fabs(x-xa[i])) < dif) {
      ns=i;
      dif=dift;
    }

    c[i]=ya[i];
    d[i]=ya[i];
  }

  y=ya[ns--];
  for (m=1;m<n;m++) {

    for (i=0;i<n-m;i++) {

      ho=xa[i]-x;
      hp=xa[i+m]-x;
      w=c[i+1]-d[i];
      if ((den=ho-hp) == 0.0) {
        cerr << "Error::polint::One point in the xaxis is repeated, please check." << endl;
        exit(0);
      }
      den=w/den;
      d[i]=hp*den;
      c[i]=ho*den;

    }
    y += (dy=(2*(ns+1) < (n-m) ? c[ns+1] : d[ns--]));

  }

}
