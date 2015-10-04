#include<cmath>
#include<iostream>
#include<fstream>
#include<sys/stat.h>
#include"mydebug.h"
#include"chi2prop.h"
#include"vec_utils.h"
#include"ffd_solar_mod.h"
#include"sym_solar_mod.h"

using namespace std;

const vector <const char *> chi2prop::contriname = {"electrons", "secondary_electrons", "primary_DM_electron", "primary_electrons", "positrons", "secondary_positrons", "primary_DM_positron"};

#define SIZING(vec)\
  vec.resize(load_dat::fluxnum);\
  for(int i = 0; i < load_dat::fluxnum; i++) vec[i].resize(load_dat::iso_vectors[i].size() - 1);

#ifdef NEWGALPROP
chi2prop::chi2prop(const string &galdefPath, const string &fitsPath, const string &outputPath, const string &outputPrefix):
#else
chi2prop::chi2prop():
#endif
  outdate(load_dat::fluxnum, true), outdate_solar(load_dat::fluxnum, false), keep(0), stype(force_field),
#ifdef NEWGALPROP
  galpropmc(new GalpropWrapper)
#else
  galpropmc(new Galprop)
#endif
{

#ifdef NEWGALPROP
    galpropmc->configure.init(galdefPath, fitsPath, outputPath, outputPrefix);
#endif

    SIZING(Fluxes);
    SIZING(Fluxes_as);
}

int chi2prop::setphi(const vector <double> &phi_) throw() {
  phi = phi_;
  std::fill(outdate_solar.begin(), outdate_solar.end(), true);
  return 0;
}

int chi2prop::setkeep(const vector <load_dat::fluxes> &keep_) throw() {
  keep = keep_;
  return 0;
}

unsigned chi2prop::ele_size(load_dat::fluxes element) const {
  return Fluxes[element].size();
}

spectrum chi2prop::get_spec(load_dat::fluxes element, int i_iso, int i_phi) const {
  printDebugMsg("Routine", ">> get_spec: element %d, iso %d, iphi %d", element, i_iso, i_phi);
  printDebugMsg("Check cposi", "rescale with cposi: %g", cposi);
  if (i_phi >= int(phi.size())) throw(no_exist_phi);
  else if (i_iso >= int(ele_size(element))) throw(no_exist_iso);

  if (i_iso < 0) {
    spectrum spectmp = get_spec(element, 0, i_phi);
    for(unsigned i = 1; i < ele_size(element); i++)
      spectmp += get_spec(element, i, i_phi);
    return spectmp;
  }

  if (i_phi >= 0) return Fluxes_as[element][i_iso][i_phi];
  if ((element == load_dat::secelecs || element == load_dat::secposis) && i_iso == 0) return cposi * Fluxes[element][0];
  if ((element == load_dat::secantip) && i_iso == 0) return cpbar * Fluxes[element][0];
  return Fluxes[element][i_iso];
}

int chi2prop::get_by_iso(double *Etmp, double *Ftmp, int ndat, load_dat::fluxes element) {
  printDebugMsg("Routine", ">>get_by_iso: %d, %d", load_dat::iso_vectors[element][0], load_dat::iso_vectors[element][1]);
  vector <int> isovec = load_dat::iso_vectors[element];

  for(unsigned j = 0; j < isovec.size() - 1; j++) {
    galpropmc->get_result(Etmp, Ftmp, isovec[j + 1], isovec[0]);
    Fluxes[element][j] = spectrum(Etmp, Ftmp, ndat);
  }

  printDebugMsg("Routine", "<<get_by_iso");
  return 1;
}

int chi2prop::get_by_name(double *Etmp, double *Ftmp, int ndat, load_dat:: fluxes element) {
  printDebugMsg("Routine", ">>get_by_name: %s", load_dat::iso_names[element - load_dat::common_fluxnum].c_str());

  galpropmc->get_result(Etmp, Ftmp, load_dat::iso_names[element - load_dat::common_fluxnum].c_str());
  Fluxes[element][0] = spectrum(Etmp, Ftmp, ndat);

  printDebugMsg("Routine", "<<get_by_name");
  return 1;
}

int chi2prop::get_flux(load_dat::fluxes element) {
  printDebugMsg("Routine", ">>get_flux: by flux; element %d, to get %s", element, outdate[element] ? "true" : "false");
  if(!outdate[element]) return 0;

  outdate[element] = false;
  outdate_solar[element] = true;

#ifdef NEWGALPROP
  unsigned ndat=galpropmc->get_npgrid();
#else
  unsigned ndat=galpropmc->gcr[galpropmc->n_species-1].n_pgrid;
#endif

  double *Etmp = new double[ndat],
         *Ftmp = new double[ndat];

  if(element < load_dat::common_fluxnum) get_by_iso(Etmp, Ftmp, ndat, element);
  else get_by_name(Etmp, Ftmp, ndat, element);
  printDebugMsg("Routine", "<<get_flux: by flux");
  return 1;
}

vector <load_dat::fluxes> con_vectors(load_dat::choice chc) {
  vector <load_dat::fluxes> res;
  res.insert(res.end(), load_dat::sub_table[chc].begin(), load_dat::sub_table[chc].end());

  if(unsigned(chc) < load_dat::deno_table.size())
    res.insert(res.end(), load_dat::deno_table[chc].begin(), load_dat::deno_table[chc].end());

  return res;
}

int chi2prop::get_flux(load_dat::choice chc){
  printDebugMsg("Routine", ">>get_flux");

  vector <load_dat::fluxes> elements = con_vectors(chc);
  for(unsigned i = 0; i < elements.size(); i++) get_flux(elements[i]);

  printDebugMsg("Routine", "<<get_flux");
  return 0;
}

int chi2prop::solar_modulas(load_dat::fluxes element, unsigned iso) {
  printDebugMsg("Routine", ">>solar_modulas : flux iso %d %d", element, iso)
  static ffd_solar_mod *ffdsolar = new ffd_solar_mod();
  static sym_solar_mod *symsolar = new sym_solar_mod();
  static solar_mod *solar;
  if(spherical == stype) solar = symsolar;
  else solar = ffdsolar;

  int Z_iso = load_dat::iso_vectors[element][0],
      A_iso = load_dat::iso_vectors[element][iso + 1];

  Fluxes_as[element][iso].resize(phi.size());
  for(unsigned i_phi = 0; i_phi < phi.size(); i_phi++) {
    solar->ini(A_iso, Z_iso, phi[i_phi]);
    Fluxes_as[element][iso][i_phi] = get_spec(element, iso);
    solar->mod(get_spec(element, iso), Fluxes_as[element][iso][i_phi]);
  }// i_phi

  printDebugMsg("Routine", "<<solar_modulas : flux iso");
  return 0;
}

int chi2prop::solar_modulas(load_dat::fluxes element) {
  printDebugMsg("Routine", ">>solar_modulas : flux %d", element)
  if(!outdate_solar[element]) return 0;
  outdate_solar[element] = false;

  for(unsigned i = 0; i < ele_size(element); i++) solar_modulas(element, i);

  printDebugMsg("Routine", "<<solar_modulas : flux")
  return 1;
}

int chi2prop::solar_modulas(load_dat::choice chc) {
  printDebugMsg("Routine", ">>solar_modulas")

  vector <load_dat::fluxes> elements = con_vectors(chc);

  for(unsigned i = 0; i < elements.size(); i++) solar_modulas(elements[i]);

  printDebugMsg("Routine", "<<solar_modulas")
  return 0;
}

spectrum chi2prop::sum_elements(const vector <load_dat::fluxes> &elevectors, int i_phi) const {
  printDebugMsg("Routine", ">>sum_elements")
  spectrum res;

  for(unsigned i = 0; i < elevectors.size(); i++) {
    for(unsigned i_iso = 0; i_iso < ele_size(elevectors[i]); i_iso++)
      if(0 == i && 0 == i_iso) res = get_spec(elevectors[i], i_iso, i_phi);
      else res += get_spec(elevectors[i], i_iso, i_phi);
  }
  printDebugMsg("Routine", "<<sum_elements")
  return res;
}

int chi2prop::print_flux(load_dat::choice chc, const string &fluxname, load_dat::fluxes flux, int iso, int i_phi, const string &outpath) {
  vector <load_dat::fluxes> sub = {flux}, deno;
  return print_flux(chc, fluxname, sub, deno, iso, i_phi, outpath);
}

int chi2prop::print_flux(load_dat::choice chc, const string &fluxname, const vector <load_dat::fluxes> &sub, const vector <load_dat::fluxes> &deno, int iso, int i_phi, const string &outpath) {
  cout << "#Extra " << load_dat::data_name[chc] << "-extra-" << fluxname;
  if(i_phi >= 0) cout << "_with_phi_" << phi[i_phi];
  cout << endl;

  string subpath = outpath + "/" + load_dat::data_name[chc];
  if (outpath == "null") subpath = "null";
  else {
    mkdir(outpath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(subpath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  }

  ostringstream fname;
  fname << subpath << "/" << fluxname;
  if (i_phi >= 0) fname << "-" << phi[i_phi];

  for (unsigned i = 0; i < sub.size(); i++) get_flux(sub[i]);
  for (unsigned i = 0; i < deno.size(); i++) get_flux(deno[i]);

  spectrum spec = get_spec(sub[0], iso, i_phi);
  for (unsigned i = 1; i < sub.size(); i++) spec += get_spec(sub[i], iso, i_phi);

  if(deno.size()) {
    spectrum denospec = get_spec(deno[0], iso, i_phi);
    for (unsigned i = 1; i < deno.size(); i++) spec += get_spec(deno[i], iso, i_phi);
    spec /= denospec;
  }

  spec.print(outpath == "null" ? "null" : fname.str());
  return 0;
}

//compare the result with specified data
double chi2prop::calc_chi2(load_dat::choice chc, const spectrum &spec, const vector <int> &exp_subgrp, double phi, const string &outpath) const {
  printDebugMsg("Routine", ">>calc_chi2: expgroupsize = %zu", exp_subgrp.size());
  double subsum = 0;

  for(unsigned j = 0; j < exp_subgrp.size(); j++) {
    ostringstream outfile;
    outfile << outpath << "/data-" << phi << "-" << enum2str(chc, exp_subgrp[j]);

    subsum += load.datas[chc].chi2(exp_subgrp[j], spec, pflag, outpath == "null" ? "null" : outfile.str());
  }
  printDebugMsg("Routine", "<<calc_chi2: %f", subsum);
  return subsum;
}

double chi2prop::chi2(const vector <vector <int> > &exn, load_dat::choice chc, bool pflag_, const string &outpath, ios_base::openmode outmode){
  printDebugMsg("Routine", ">>chi2: %s", load_dat::data_name[chc].c_str());
  pflag = pflag_;

  string subpath = outpath + "/" + load_dat::data_name[chc];
  if (pflag) {
    if (outpath == "null") subpath = "null";
    else {
      mkdir(outpath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
      mkdir(subpath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }
  }
  //ostringstream tmpout;

  get_flux(chc);
  solar_modulas(chc);

  spectrum spec, specns;

  if(pflag) {
    specns = sum_elements(load_dat::sub_table[chc]);
    if(unsigned(chc) < load_dat::deno_table.size()) specns /= sum_elements(load_dat::deno_table[chc]);
  }

  double sum=0;
  for(unsigned i_exp = 0; i_exp < phi.size(); i_exp++) {

    spec = sum_elements(load_dat::sub_table[chc], i_exp);
    if(unsigned(chc) < load_dat::deno_table.size()) spec /= sum_elements(load_dat::deno_table[chc], i_exp);

    if (pflag) {
      ostringstream fluxname;
      fluxname << subpath << "/flux-" << phi[i_exp];
      specns.compare(spec, subpath == "null" ? "null" : fluxname.str());
    }
    sum += calc_chi2(chc, spec, exn[i_exp], phi[i_exp], subpath);
  }

  printDebugMsg("Routine", "<<chi2: %f", sum);
  return sum;
}

#ifndef NEWGALPROP
int chi2prop::run(double *p, int iter, model_kind mod, bool pflag, const char *defname) {
  printDebugMsg("Routine", ">> run:\titer %d\tpflag %d\tGALDEF %s\n", iter, pflag, defname);
  std::fill(outdate.begin(), outdate.end(), true);
  if(0 != iter)
    for(unsigned i = 0; i < keep.size(); i++) outdate[keep[i]] = false;

  return galpropmc->run(p, iter, mod, pflag, defname);
  printDebugMsg("Routine", "<< run");
}

int chi2prop::run(double *p, int iter, model_kind mod, bool pflag) {
  return run(p, iter, mod, pflag, "default");
}

int chi2prop::run(double *p, int iter, model_kind mod) {
  return run(p, iter, mod, false, "default");
}

int chi2prop::run(const char *defname) {
  double *p = 0;
  return run(p, 0, DEFAULT, true, defname);
}

int chi2prop::run() {
  double *p= 0;
  return run(p, 0, DEFAULT, true, "default");
}
#endif

int chi2prop::start(int iter) throw() {
  printDebugMsg("Routine", ">> start:\titer %d", iter);
  std::fill(outdate.begin(), outdate.end(), true);
  if(0 != iter)
    for(unsigned i = 0; i < keep.size(); i++) outdate[keep[i]] = false;

  int res = galpropmc->start(iter);
  printDebugMsg("Routine", "<< start");
  return res;
}

#ifndef NEWGALPROP
int chi2prop::setpara(double *p, model_kind mod) {
  return galpropmc->set_params(p, mod);
}
#endif

int chi2prop::err_info(errtype &err) throw() {
  switch(err) {
  case no_exist_phi:
    cout << "Error::chi2prop::You are asking for spectrum with unexisted phi" << endl;
    break;

  case no_exist_iso:
    cout << "Error::chi2prop::You are asking for spectrum with unexisted iso number" << endl;
    break;
  }
  return 0;
}
