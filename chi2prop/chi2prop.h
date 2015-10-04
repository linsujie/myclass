#ifndef _CHI2PROP_H
#define _CHI2PROP_H
#include<sstream>

#ifdef NEWGALPROP

#include "GalpropWrapper.h"

#else

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

#endif

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
  vector <vector <vector <spectrum> > > Fluxes_as; // Fluxes_as[i_element][i_iso][i_phi]
  vector <bool> outdate, outdate_solar;
  vector <load_dat::fluxes> keep;
  load_dat load;
  static const vector <const char *> contriname;

  spectrum get_spec(load_dat::fluxes element, int i_iso = -1, int i_phi = -1) const;
  int get_flux(load_dat::choice chc);
  int get_flux(load_dat::fluxes element);
  unsigned ele_size(load_dat::fluxes element) const;
  int get_by_iso(double *E, double *F, int ndat, load_dat::fluxes element);
  int get_by_name(double *E, double *F, int ndat, load_dat::fluxes element);
  int solar_modulas(load_dat::choice chc);
  int solar_modulas(load_dat::fluxes element);
  int solar_modulas(load_dat::fluxes element, unsigned iso);
  spectrum sum_elements(const vector <load_dat::fluxes> &elevectors, int iphi = -1) const;
  double calc_chi2(load_dat::choice chc, const spectrum &spec, const vector <int> &exp_subgrp, double phi, const string &outpath) const;

#ifndef NEWGALPROP
  int run(double *p, int iter, model_kind mod, bool pflag, const char *defname);
#endif

public:
  enum errtype { no_exist_phi, no_exist_iso };
  enum solar_type {spherical, force_field};
  solar_type stype;
  double cposi, cpbar;
#ifdef NEWGALPROP
  GalpropWrapper *galpropmc;

  chi2prop(const string &galdefPath, const string &fitsPath, const string &outputPath = "../../FITS", const string &outputPrefix = "galp");
#else
  Galprop *galpropmc;

  chi2prop();
  int setpara(double *p, model_kind mod);
#endif
  int setphi(const vector <double> &phi_) throw();
  int setkeep(const vector <load_dat::fluxes> &keep_) throw();

  double chi2(const vector <vector <int> > &exn, load_dat::choice choice, bool pflag = false, const string &outpath = "null", ios_base::openmode outmode = ios_base::out);

  int print_flux(load_dat::choice chc, const string &fluxname, load_dat::fluxes flux, int iso = -1, int i_phi = -1, const string &outpath = "null");

  int print_flux(load_dat::choice chc, const string &fluxname, const vector <load_dat::fluxes> &sub, const vector <load_dat::fluxes> &denom, int iso = -1, int i_phi = -1, const string &outpath = "null");

  static int err_info(errtype &err) throw();

  int start(int iter) throw();

#ifndef NEWGALPROP
  int run();
  int run(const char *defname);
  int run(double *p, int iter, model_kind mod);
  int run(double *p, int iter, model_kind mod, bool pflag);
#endif
};
#endif // for #ifndef _CHI2PROP_H
