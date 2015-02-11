#include<fstream>
#include<sstream>
#include<iostream>
#include<cmath>
#include"sym_solar_mod.h"
#include"ffd_solar_mod.h"

using namespace std;

spectrum read_spect(const char *filename, double Eindx) {
  string line;
  double Etmp, Ftmp;
  ifstream datfile(filename);
  spectrum res;

  for (; getline(datfile, line);) {
    istringstream is(line);
    is >> Etmp; res.E.push_back(Etmp);
    is >> Ftmp; res.F.push_back(Ftmp / pow(Etmp, Eindx));
  }
  return res;
}

int main() {
  sym_solar_mod symsolar(0, -1, 1.6, false);
  ffd_solar_mod ffdsolar(0, -1, 1.6, false);

  spectrum ori = read_spect("tmpfile", 3),
           tar;
  ori.print("before");

  cout << "====== sym modulation ======" << endl;
  symsolar.mod(ori, tar);
  tar.print("symmod");

  cout << "====== force field modulation ======" << endl;
  ffdsolar.mod(ori, tar);
  tar.print("force");

  //There is only two important member in a spectrum, E (energy) and F (flux), they are vectors.
  for(unsigned i = 0; i < tar.E.size(); i++) {
    cout << "E and F is " << tar.E[i] << " " << tar.F[i] << endl;
  }
  return 0;
}
