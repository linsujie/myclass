#include<iostream>
#include<cmath>
#include"Bin.h"

using std::vector;
using std::cout;
using std::endl;

Bin::Bin(const vector <double> &bin_): bin(bin_) {
  if (bin.size() < 2) cout << "Warning:Bin::bin size wrong" << endl;
};
Bin::Bin(double start, double end, int div, bool ln_or_not): bin(binOri) {
  const double apro_num = 1.0001;

  if (ln_or_not)
    for (double point = start; point <= end * apro_num; point *= pow(end / start, 1.0 / div))
      binOri.push_back(point);

  else
    for (double point = start; point <= end * apro_num; point += (end - start) / div)
      binOri.push_back(point);

  if (binOri.size() < 2) cout << "Warning:Bin::bin size wrong" << endl;
}

int Bin::count1d(const vector <double> &data) {
  extern int get_index(const vector <double> &vec, double x);
  result1d.E.resize(bin.size() - 1, 0);
  result1d.F.resize(bin.size() - 1, 0);
  int ind;

  for (unsigned i = 0; i < data.size(); i++) {
    ind = get_index(bin, data[i]);

    if (ind >= 0 && ind <= int(bin.size() - 2)) {
      result1d.F[ind] += 1;
      result1d.E[ind] += data[i];
    }
  }

  int sum = 0;

  for (unsigned j = 0; j < bin.size() - 1; j++)  sum += result1d.F[j];

  for (unsigned j = 0; j < bin.size() - 1; j++) {
    if (result1d.F[j] != 0)
      result1d.E[j] /= result1d.F[j];

    result1d.F[j] /= ((bin[j + 1] - bin[j]) * sum);
  }

  return 0;
}
