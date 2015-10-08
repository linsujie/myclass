#ifndef _LOAD_DAT_H
#define _LOAD_DAT_H
/*********************************************************************
To load the data files
 *********************************************************************/

#include"chisq.h"
#include"enum_utils.h"

class load_dat{
public:
  enum fluxes {belements, celements, oelements, pelements, heelements, b10elements, b9elements, prielecs, secelecs, dmelecs, secposis, dmposis, secantip, dmantip};
  //Those determined with name should be located behind others
  static const int fluxnum, common_fluxnum;
  static const std::vector <std::vector <int> > iso_vectors;
  static const std::vector <std::string> iso_names;

  enum choice {bcratio, beratio, posifrac, pbarp, proton, helium, carbon, oxygen, electron, totep, positron};
  //ratio should be putted before fluxes.
  static const std::string data_name[11];

  static const std::vector <std::vector <fluxes> > sub_table, deno_table;

  std::vector <chisq> datas;

  load_dat();
  int print() const;
};
const char* enum2str(load_dat::choice chc, int number);

#define X(a) a,
enum ppexpname {
#include "enumdef/ppexpname.def"
};

enum carbexpname {
#include "enumdef/carbexpname.def"
};

enum eleexpname {
#include "enumdef/eleexpname.def"
};

enum totexpname {
#include "enumdef/totexpname.def"
};

enum posifexpname {
#include "enumdef/posifexpname.def"
};

enum positexpname {
#include "enumdef/positexpname.def"
};

enum protexpname {
#include "enumdef/protexpname.def"
};

enum heliumexpname {
#include "enumdef/heliumexpname.def"
};

enum bcexpname {
#include "enumdef/bcexpname.def"
};

enum beexpname {
#include "enumdef/beexpname.def"
};
#undef X

#define X(TYPE) ENUMDECLEAR(TYPE)
X(ppexpname) X(carbexpname) X(eleexpname) X(totexpname) X(posifexpname)
X(positexpname) X(protexpname) X(heliumexpname) X(bcexpname) X(beexpname)
#undef X

#endif // for #ifndef _LOAD_DAT_H
