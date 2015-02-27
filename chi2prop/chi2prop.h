#ifndef _CHI2PROP_H
#define _CHI2PROP_H
#include"ErrorLogger.h"
#include "Configure.h"
#include "Galdef.h"
#include "Spectrum.h"
#include "Distribution.h"
#include "Particle.h"  
#include "Galaxy.h"
#include "Galprop.h" // AWS20050816
#include"galprop_internal.h"

#include"config.h"

#include"spectrum.h"
#include"load_dat.h"
/*********************************************************************
To provide a chi2 to mcmc by runing galprop and calculation the
specified chi2
 *********************************************************************/
using std::vector;

class chi2prop {
private:
  bool pflag;
  vector <double> phi;
  vector <vector <spectrum> > Fluxes; // Fluxes[i_element][i_iso]
  vector <vector <vector <spectrum> > > Fluxes_as; // Fluxes[i_element][i_iso][i_phi]
  vector <bool> outdate, outdate_solar;
  vector <load_dat::fluxes> keep;
  static const vector <const char *> contriname;

  inline spectrum get_spec(load_dat::fluxes element, unsigned i_iso) const;
  int get_flux(load_dat::choice chc);
  int get_flux(load_dat::fluxes element);
  int get_by_iso(double *E, double *F, int ndat, load_dat::fluxes element);
  int get_by_name(double *E, double *F, int ndat, load_dat::fluxes element);
  int solar_modulas(load_dat::choice chc);
  int solar_modulas(load_dat::fluxes element);
  int solar_modulas(load_dat::fluxes element, unsigned iso);
  spectrum sum_elements(const vector <load_dat::fluxes> &elevectors) const;
  spectrum sum_elements(const vector <load_dat::fluxes> &elevectors, unsigned iphi) const;
  int print_lines(load_dat::choice chc, double phi, const spectrum &spec, const spectrum &specns) const;
  double calc_chi2(load_dat::choice chc, const spectrum &spec, const vector <int> &exp_subgrp, bool main_title) const;
  spectrum flux_for_print(load_dat::fluxes flux, int iso, int i_phi);

  int run(double *p, int iter, model_kind mod, bool pflag, const char *defname);

public:
  static Galprop *galpropmc;
  enum solar_type {spherical, force_field};
  solar_type stype;
  double cposi;
  chi2prop();
  int setphi(const vector <double> &phi_);
  int setkeep(vector <load_dat::fluxes> &keep_);
  int setpara(double *p, model_kind mod);

  double chi2(const vector <vector <int> > &exn, load_dat::choice choice);
  double chi2(const vector <double> &phi_, const vector <vector <int> > &exn, load_dat::choice choice);
  double chi2(const vector <vector <int> > &exn, load_dat::choice choice, bool pflag_);
  double chi2(const vector <double> &phi_, const vector <vector <int> > &exn, load_dat::choice choice, bool pflag_);

  int print_flux(load_dat::choice chc, const string &fluxname, load_dat::fluxes flux, int iso, int i_phi);
  int print_flux(load_dat::choice chc, const string &fluxname, load_dat::fluxes flux, int iso);
  int print_flux(load_dat::choice chc, const string &fluxname, load_dat::fluxes flux);

  int print_flux(load_dat::choice chc, const string &fluxname, const vector <load_dat::fluxes> &sub, const vector <load_dat::fluxes> &denom, int iso, int i_phi);
  int print_flux(load_dat::choice chc, const string &fluxname, const vector <load_dat::fluxes> &sub, const vector <load_dat::fluxes> &denom, int iso);
  int print_flux(load_dat::choice chc, const string &fluxname, const vector <load_dat::fluxes> &sub, const vector <load_dat::fluxes> &denom);

  int start(int iter);
  int run();
  int run(const char *defname);
  int run(double *p, int iter, model_kind mod);
  int run(double *p, int iter, model_kind mod, bool pflag);
};
#endif // for #ifndef _CHI2PROP_H
