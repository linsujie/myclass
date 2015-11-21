#ifndef _MCPARAS_H
#define _MCPARAS_H
#include"chi2prop.h"
#include"anaspec.h"
#include<vector>
#include<string>

class mcparas {
private:
  unsigned p_index;
  const double *p;
  Galdef *galdef;

public:

#define X(a) a,
  enum errtype {
#include"enumdef/errtype.def"
  };

  const static std::vector <std::string> elec_runNumber, prot_runNumber, BC_runNumber;

  enum pppc_or_us {
#include"enumdef/pppc_or_us.def"
  };
  enum dm_mode {
#include"enumdef/dm_mode.def"
  };
  enum prop_mode {
#include"enumdef/prop_mode.def"
  };
  enum inject_mode {
#include"enumdef/inject_mode.def"
  };
  enum mix_branch {
#include"enumdef/mix_branch.def"
  };
#undef X

#define X(TYPE) static ENUMNAME(TYPE)
  X(errtype); X(pppc_or_us); X(dm_mode); X(prop_mode); X(inject_mode); X(mix_branch);
#undef X

#ifdef GALP_V55
  mcparas(GalpropWrapper *galprop);
  int setgalprop(GalpropWrapper *galprop) throw();
#else
  mcparas(Galprop *galprop);
  int setgalprop(Galprop *galprop) throw();
#endif
  int setpara(const double *p_) throw();

  static unsigned getenum(const string &enum_name);
  double getp() throw();
  int print() const throw();
  static int err_info(errtype &err) throw();

  int propagation_set_yuan(prop_mode prop) throw();
  int propagation_set_whole(prop_mode prop) throw();
  int propagation_set(prop_mode prop) throw();

  int proton_set(inject_mode bks) throw();
  int proton_set() throw();

#ifdef GALP_V55
  int set_iso_injection(Galdef::specProperties &prop, inject_mode &bks, const double *point) throw();
  int helium_set(inject_mode bks) throw();
#endif

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
#define X(TYPE) ENUMDECLEAR(mcparas::TYPE)
  X(errtype) X(pppc_or_us) X(dm_mode) X(prop_mode) X(inject_mode) X(mix_branch)
#undef X

#endif // for #ifndef _MCPARAS_H
