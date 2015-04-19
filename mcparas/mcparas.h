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

  enum errtype { branch_out_of_range, set_branch_for_pulsar, mix_branch_out_of_range };

  const static std::vector < std::vector <std::string> > enum_names;
  const static std::vector <std::string> elec_runNumber, prot_runNumber, BC_runNumber;

  enum pppc_or_us { pppc, us };
  enum dm_mode { pulsar, annihilate, decay };
  enum prop_mode { PD, DC, DR, DC2, DR2, DRC};
  enum inject_mode { nbk, onebk, twobk };
  enum mix_branch { emutau, mutaub, emutaub, b3l };

  mcparas(Galprop *galprop);
  int setgalprop(Galprop *galprop) throw();
  int setpara(double *p_) throw();

  static unsigned getenum(const string &enum_name);
  double getp() throw();
  int print() const throw();
  static int err_info(errtype &err) throw();

  int propagation_set_yuan(prop_mode prop) throw();
  int propagation_set_whole(prop_mode prop) throw();
  int propagation_set(prop_mode prop) throw();

  int proton_set(inject_mode bks) throw();
  int proton_set() throw();

  int electron_set(inject_mode bks) throw();

  int exotic_set() throw();

  int branch_set(anaspec::branch_choice branch) const throw(errtype);
  int branch_set(double *branches) const throw(errtype);
  int branch_set(mix_branch mixbran) throw(errtype);

  int setdm(dm_mode dm, pppc_or_us pc) const throw();

  int close_Z() const throw();
  Galdef* getpoint() const throw();
  unsigned np() const throw();
};
#endif // for #ifndef _MCPARAS_H
