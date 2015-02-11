#include<iostream>
#include"gfunction.h"

using std::vector;
using std::cout;
using std::endl;

double gfunction::operator()(double x, double y) {
  cout << "gfunction::(double,double)::This is an empty function" << endl;
  return 0;
}

double gfunction::operator()(double x, double y) const {
  cout << "gfunction::(double,double)::This is an empty function" << endl;
  return 0;
}

int gfunction::operator()(double x, vector <double> &y1, vector <double> &y2) {
  cout << "gfunction::(double,vector,vector)::This is an empty function" << endl;
  return 0;
}

int gfunction::operator()(double x, vector <double> &y1, vector <double> &y2) const {
  cout << "gfunction::(double,vector,vector)::This is an empty function" << endl;
  return 0;
}

int gfunction::operator()(vector <double> &x, vector <double> &y) const {
  cout << "gfunction::(vector, vector)::This is an empty function" << endl;
  return 0;
}

int gfunction::operator()(vector <double> &x, vector <double> &y) {
  cout << "gfunction::(vector, vector)::This is an empty function" << endl;
  return 0;
}
