#ifndef _MCPARAS_H
#define _MCPARAS_H
#include"chi2prop.h"
#include"anaspec.h"
#include<vector>
#include<string>

class mcparas {
private:
  unsigned p_index;
  double *p;
  Galdef *galdef;

public:

  const static std::vector < std::vector <std::string> > enum_names;
  const static std::vector <std::string> elec_runNumber, prot_runNumber, BC_runNumber;

  enum pppc_or_us { pppc, us };
  enum dm_mode { pulsar, annihilate, decay };
  enum prop_mode { PD, DC, DR, DC2, DR2, DRC};
  enum inject_mode { nbk, onebk, twobk };
  enum mix_branch { emutau, mutaub, emutaub, b3l };

  mcparas(Galprop *galprop);
  int setgalprop(Galprop *galprop);
  int setpara(double *p_);

  static unsigned getenum(const string &enum_name);
  int print() const;

  int propagation_set_whole(prop_mode prop);
  int propagation_set(prop_mode prop);

  int proton_set(inject_mode bks);
  int proton_set();

  int electron_set(inject_mode bks);

  int exotic_set();

  int branch_set(anaspec::branch_choice branch) const;
  int branch_set(double *branches) const;
  int branch_set(mix_branch mixbran);

  int setdm(dm_mode dm, pppc_or_us pc) const;

  int close_Z() const;
};
#endif // for #ifndef _MCPARAS_H
