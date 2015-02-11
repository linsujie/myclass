#ifndef _LOAD_DAT_H
#define _LOAD_DAT_H
/*********************************************************************
To load the data files
 *********************************************************************/

#include"chisq.h"

class load_dat{
public:
  enum fluxes {belements, celements, oelements, pelements, b10elements, b9elements, prielecs, secelecs, dmelecs, secposis, dmposis, secantip, dmantip};
  //Those determined with name should be located behind others
  static const int fluxnum, common_fluxnum;
  static const std::vector <std::vector <int> > iso_vectors;
  static const std::vector <std::string> iso_names;

  enum choice {bcratio, beratio, posifrac, pbarp, proton, carbon, oxygen, electron, totep, positron};
  //ratio should be putted before fluxes.
  static const std::vector <chisq> datas;
  static const std::string data_name[10];

  static const std::vector <std::vector <fluxes> > sub_table, deno_table;
  int print() const;
};

enum ppexpname { ppBESSPolar, ppBESSTeV, ppBESS00, ppBESS99, ppHEAT, ppPAMELA0806, ppPAMELA0812, ppPAMELA09 };
enum carbexpname {carbACE97,carbACE98,carbACE01,carbACE09,carbATIC0203,carbBalloon91,carbCREAM205,carbTRACER06};
enum eleexpname {elePAMELA, eleAMS02, eleAMS02enlarge, eleAMS025GV, eleAMS0210GV, eleAMS2014, eleAMS20141GV};
enum totexpname {totFH, totAMS02, totAMS025GV, totAMS0210GV, totAMS2014, totAMS20141GV};
enum posifexpname {posiAMS02, posiAMS021GV, posiAMS025GV, posiAMS0210GV, posiAMS2014, posiAMS20141GV};
enum positexpname {positAMS02, positAMS02enlarge, positAMS025GV, positAMS0210GV, positAMS2014, positAMS20141GV};
enum protexpname {protAMS02, protAMS021GV, protPAMELA, protATIC, protCREAM};
enum bcexpname {BCACE97, BCACE98, BCACE01, BCACE09, BCAMS0198, BCATIC0203, BCBALLOON91, BCCREAM04, BCHEAO0379, BCTRACER06, BCUlysses90, BCVoyager86, BCAMS02pre, BCHEAO0379enlarge};
enum beexpname {BeACE97, BeACESIS97, BeBalloon73, BeBalloon7705, BeBalloon7709, BeIMP72, BeIMP74, BeISEE378, BeISOMAX98, BeUlysses90, BeVoyager7791, BeVoyager7798};

#endif // for #ifndef _LOAD_DAT_H
