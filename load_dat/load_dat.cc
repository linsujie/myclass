#include"load_dat.h"
#include<iostream>
#include<cstdlib>

using std::vector;
using std::string;
using std::cout;
using std::endl;

const string load_dat::data_name[10] = {"BC", "Be", "Posifrac", "pbarp", "Proton", "Carbon", "Oxygen", "Electron", "TotEP", "Positron"};

// recording the Z and A of all the isotopes to be calculated.
const vector <vector <int> > load_dat::iso_vectors = {{5, 10, 11},
                                                      {6, 12, 13},
                                                      {8, 16, 17, 18},
                                                      {1, 1},
                                                      {4, 10},
                                                      {4, 9},
                                                      {-1, 0}, {-1, 0}, {-1, 0},
                                                      {1, 0}, {1, 0},
                                                      {-1, 1}, {-1, 1}
};
const vector <string> load_dat::iso_names = { "primary_electrons", "secondary_electrons", "primary_DM_electron",
                                    "secondary_positrons", "primary_DM_positron",
                                    "secondary_antiprotons", "primary_DM_antip" };
const int load_dat::common_fluxnum = iso_vectors.size() - iso_names.size(),
      load_dat::fluxnum = iso_vectors.size();

#ifdef DATDIR
  const string path = DATDIR,
#else
  const string path = ".",
#endif
      carbname = path + "/carbon.dat",
      oxyname = path + "/oxygen.dat",
      protname = path + "/proton.dat",
      bcname = path + "/bcratio.dat",
      bename = path + "/beratio.dat",
      pbarpname = path + "/pbarp.dat",
      elename = path + "/electron.dat",
      posiname = path + "/posifrac.dat",
      totepname = path + "/totep.dat",
      positronname = path + "/positron.dat";

const vector <chisq> load_dat::datas = {chisq(bcname),
                                        chisq(bename),
                                        chisq(posiname),
                                        chisq(pbarpname),
                                        chisq(protname, 2.7),
                                        chisq(carbname),
                                        chisq(oxyname),
                                        chisq(elename, 3.0),
                                        chisq(totepname, 3.0),
                                        chisq(positronname, 3.0)};

const vector <vector <load_dat::fluxes> > load_dat::sub_table = {{load_dat::belements},
                                                                 {load_dat::b10elements},
                                                                 {load_dat::secposis, load_dat::dmposis},
                                                                 {load_dat::secantip, load_dat::dmantip},
                                                                 {load_dat::pelements},
                                                                 {load_dat::celements},
                                                                 {load_dat::oelements},
                                                                 {load_dat::prielecs, load_dat::secelecs, load_dat::dmelecs},
                                                                 {load_dat::prielecs, load_dat::secelecs, load_dat::dmelecs, load_dat::secposis, load_dat::dmposis},
                                                                 {load_dat::secposis, load_dat::dmposis}},

                                         load_dat::deno_table = {{load_dat::celements},
                                                                 {load_dat::b9elements},
                                                                 {load_dat::prielecs, load_dat::secelecs, load_dat::dmelecs, load_dat::secposis, load_dat::dmposis},
                                                                 {load_dat::pelements}};

int load_dat::print() const {
  for(unsigned i = 0; i < datas.size(); i++) {
    cout << data_name[i] << ":" << endl;
    datas[i].printsizes();
    cout << endl;
  }

  return 0;
}

const char* enum2str(load_dat::choice chc, int number) {
  switch(chc) {
  case load_dat::bcratio:
    return enum2str(bcexpname(number));

  case load_dat::beratio:
    return enum2str(beexpname(number));

  case load_dat::posifrac:
    return enum2str(posifexpname(number));

  case load_dat::pbarp:
    return enum2str(ppexpname(number));

  case load_dat::proton:
    return enum2str(protexpname(number));

  case load_dat::carbon:
    return enum2str(carbexpname(number));

  case load_dat::electron:
    return enum2str(eleexpname(number));

  case load_dat::totep:
    return enum2str(totexpname(number));

  case load_dat::positron:
    return enum2str(positexpname(number));

  default:
    cout << "Error::load_dat::enum2str::could not find the enum names for this choice: " << chc << endl;
    exit(0);
  }
}

#define NAME(TYPE) ENUMNAME(TYPE)
#define X(a) #a,
NAME(ppexpname) = {
#include "enumdef/ppexpname.def"
};

NAME(carbexpname) = {
#include "enumdef/carbexpname.def"
};

NAME(eleexpname) = {
#include "enumdef/eleexpname.def"
};

NAME(totexpname) = {
#include "enumdef/totexpname.def"
};

NAME(posifexpname) = {
#include "enumdef/posifexpname.def"
};

NAME(positexpname) = {
#include "enumdef/positexpname.def"
};

NAME(protexpname) = {
#include "enumdef/protexpname.def"
};

NAME(bcexpname) = {
#include "enumdef/bcexpname.def"
};

NAME(beexpname) = {
#include "enumdef/beexpname.def"
};
#undef X
#undef NAME

#define X(TYPE) ENUMANDSTR(TYPE, TYPE##_name)
X(ppexpname) X(carbexpname) X(eleexpname) X(totexpname) X(posifexpname)
X(positexpname) X(protexpname) X(bcexpname) X(beexpname)
#undef X
