#include"sym_solar_mod.h"
#include"ffd_solar_mod.h"
#include"solar_mod.h"
#include<fstream>
#include<sstream>
#include<iostream>
#include<cmath>
#include<cstdlib>

using namespace std;

spectrum read_spect(const char *filename, double Eindx) {
  printf("reading %s\n", filename);
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

int main(int argc, char *argv[]) {
  double phi = argc >= 2 ? atof(argv[1]) : 0.4;
  cout << "phi is " << phi << endl;
  int choice = argc >= 3 ? atoi(argv[2]) : 0;
  cout << "type is " << (0 == choice ? "ffd" : "sym") << endl;
  static sym_solar_mod *symsolar = new sym_solar_mod();
  static ffd_solar_mod *ffdsolar = new ffd_solar_mod();
  solar_mod *solar;
  if(0 == choice) solar = ffdsolar;
  else solar = symsolar;
  solar->ini(0, 1, 1.5 * phi, true);
  spectrum ori = read_spect("bestplotDR", 0),
           tar;

  tar = ori;
  solar->mod(ori, tar);
  tar.print("tmp");
  return 0;
}
