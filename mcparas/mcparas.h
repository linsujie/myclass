#ifndef _MCPARAS_H
#define _MCPARAS_H
#include"chi2prop.h"
#include"anaspec.h"
#include<vector>
#include<string>

class mcparas {
private:
  unsigned p_index;
  Galdef *galdef;

public:

  const static std::vector <std::string> mode_name;
  const static std::vector < std::vector <std::string> > enum_names;
  const static std::vector <std::string> elec_runNumber, prot_runNumber, BC_runNumber;

  enum pppc_or_us { pppc, us };
  enum dm_mode { pulsar, annihilate, decay };
  enum prop_mode { PD, DC, DR, DC2, DR2, DRC};
  enum inject_mode { nbk, onebk, twobk };

  enum modes {
    PSR, DMmu, DMtau, DMww, DMuu, DMbb, DMtt, DMfe, DMfmu, DMftau,
    PSRwp, DMmuwp, DMtauwp, DMwwwp, DMuuwp, DMbbwp, DMttwp, DMfewp, DMfmuwp, DMftauwp,
    PSRwb, DMmuwb, DMtauwb, DMwwwb, DMuuwb, DMbbwb, DMttwb, DMfewb, DMfmuwb, DMftauwb,
    PSRnb, DMmunb, DMtaunb, DMwwnb, DMuunb, DMbbnb, DMttnb, DMfenb, DMfmunb, DMftaunb
  };

  mcparas(Galprop *galprop);

  int elec_setpara(double *p, inject_mode bks, double *branches, bool with_proton, dm_mode dmm, pppc_or_us pc);

  int elec_setpara(double *p, inject_mode bks, anaspec::branch_choice bran, bool with_proton, dm_mode dmm, pppc_or_us pc, Galprop *galprop);
  int elec_setpara(double *p, inject_mode bks, anaspec::branch_choice bran, bool with_proton, dm_mode dmm, pppc_or_us pc);
  int elec_setpara(double *p, inject_mode bks, anaspec::branch_choice bran, bool with_proton);
  int elec_setpara(double *p, inject_mode bks, anaspec::branch_choice bran);

  int elec_setpara(double *p, anaspec::branch_choice bran, dm_mode dmm, pppc_or_us pc);
  int elec_setpara(double *p, double *branches, dm_mode dmm, pppc_or_us pc);

  static unsigned getenum(const string &enum_name);
  int print() const;
  int propagation_set(double *p, prop_mode prop);
  int proton_set(double *p, inject_mode bks);
  int proton_set(double *p);
  int electron_set(double *p, inject_mode bks);
  int setexotic(double *p, anaspec::branch_choice branch);
  int setexotic(double *p, double *branches);
  int setdm(dm_mode dm, pppc_or_us pc) const;
  int close_Z() const;
};
#endif // for #ifndef _MCPARAS_H
