#include<iostream>
#include<cmath>
#include"RK.h"
#include"gfunction.h"
/*********************************************************************
  This is an example of the useage of the RK class. RK class is used
  to calculate the value of a 1D odered one differential equation f(x,y).
  where dy/dx=f(x,y)
  It is also useful when you only want to do integration numerically.

  All the differential equation odered larger than one can be
  calculated by sperated it to several one odered equation, for example,
          d2y/dx2=f(x,y,dy/dx)
  can be trans to:
          dy/dx=z
          dz/dx=f(x,y,z)
  and be calculated.
  However, I just found that I didn't considered about this before, so
  you can only deal with odered one situation now, I'll add the
  function for the high odered situation soon.
  What's more, be careful when you dealing with the value 0. It will
  easily cause problem.
*********************************************************************/
using namespace std;
/*********************************************************************
  The way to pass the diff equation to the class is changed. The function below is examples:
  function1:
             dy/dx=y_1
             dy_1/dx=4*x^2*y+2*y
  We will return y at last, so this is equal to d^2y/dx^2=4*x^2+2*y.

  What's more, you can still get y_1 as the variable y in class RK is Public, by RKobj.y[1].

  function2:
             dy/dx=y*x*Aa

  function3:
             d^2y/dx^2=-dy/dx - y/x
 *********************************************************************/
class Func1 : public gfunction {
public:
  int operator()(double x, vector <double> &y, vector <double> &dydx) {
    dydx[0] = y[1];
    dydx[1] = 4 * x * x * y[0] - 2 * y[0];
    return 0;
  }
};

class Func2 : public gfunction {
private:
  double paras;
public:
  Func2(double paras_): paras(paras_) {}
  int operator()(double x, vector <double> &y, vector <double> &dydx) {
    dydx[0] = y[0] * x * paras;
    return 0;//function with an parameters array is also accepted
  }
};

class Func3 : public gfunction {
public:
  int operator()(double x, vector <double> &y, vector <double> &dydx) {
    dydx[0] = y[1];
    dydx[1] = - y[1] - y[0] / x;
  }
};

int main() {
  Func1 function1;
  //example 1
  RK Intgr;
  double stlen = 1e-20,
         factor = 1.05,
         err = 1e-5;
  Intgr.setstl(stlen, factor, err); //stlen is the first step lenght, it will get larger then by times factor each step. Err is set to control the step lenth, the function will guarantee that stlen*dy/dx<err in each step.
  double start = 0,
         end = 2.5;
  vector <double> y0;//In this situation, you have to set the initial value of y and y' in the start point, so you have to set y0 a vector.
  y0.push_back(1);//set y_initial=1
  y0.push_back(0);//set y'_initial=0
  Intgr.setini(start, end, y0); //set the start and value of y in the start point. The end is, of course the point where you want to reach and get the y there.
  int print_inval = 0;
  double result = Intgr.dork(&function1, 0); //solving and return the result, the print_inval is about how to printing the x and y pair when solving the result, 0 is never print. n is print them each n step when n>0.

  /*********************************************************************
    result=Intgr.dork(&function1);//This is also ok if you don't want to print anything.
  *********************************************************************/
  cout << "calculated result " << result << " The analitical result " << exp(-end * end) << endl;

  //Another example2: dy/dx=x*y*A, and set A=2
  stlen = 1e-3, factor = 1.05, err = 1;
  start = 0, end = 3;

  double y00 = 1; //If you only deal with order 1 situation, you can just use a double number to set the initial y;
  Intgr.setstl(stlen, factor, err);
  Intgr.setini(start, end, y00);
  Func2 function2(2);
  result = Intgr.dork(&function2, 0); //The params should be an array
  cout << "calculated result " << result << " The analitical result " << exp(end * end) << endl;

  //The last example:
  y0.clear();
  y0.push_back(0.27067);
  y0.push_back(-0.135335);
  stlen = -1e-3, factor = 1.05, err = 0.001;
  start = 2, end = 1.235;

  Intgr.setstl(stlen, factor, err);
  Intgr.setini(start, end, y0);

  Func3 function3;
  result = Intgr.dork(&function3, 1);
  cout << "x * exp(-x) " << end * exp(-end) << " calculated result " << result << endl;

  vector <vector <double> > res;
  res = Intgr.dork_record(&function3);

  for (unsigned i = 0; i < res.size(); i++) {
    cout << "x " << res[i][0] << " y " << res[i][1] << " y' " << res[i][2] << " exp(-x) - x * exp(-x) " << exp(-res[i][0]) - res[i][0] * exp(-res[i][0]) << endl;
  }
  return 0;
}
