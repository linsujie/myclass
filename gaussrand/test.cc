#include"gaussrand.h"
#include<iostream>

using namespace std;

double randoms() {
  return gaussrand(2.0, 1.0);
}

int main() {
  testrand(randoms, -2, 6, 0.05, 1000000);
}
