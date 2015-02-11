#ifndef _BIN_H
#define _BIN_H
/*********************************************************************
This class is used to count the distribution of a set of number
 *********************************************************************/

#include"spectrum.h"
class Bin {
private:
  std::vector <double> binOri;
  const std::vector <double> &bin;
public:
  spectrum result1d;

  Bin(const std::vector <double> &bin_);
  Bin(double start, double end, int div, bool ln_or_not);
  int count1d(const std::vector <double> &data);
};
#endif // for #ifndef _BIN_H
