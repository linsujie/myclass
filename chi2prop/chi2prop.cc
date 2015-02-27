#include<cmath>
#include<iostream>
#include<sstream>
#include<fstream>
#include"mydebug.h"
#include"chi2prop.h"
#include"vec_utils.h"
#include"ffd_solar_mod.h"
#include"sym_solar_mod.h"

using namespace std;

//extern Particle global_particle;
Galprop* chi2prop::galpropmc = new Galprop;

const vector <const char *> chi2prop::contriname = {"electrons", "secondary_electrons", "primary_DM_electron", "primary_electrons", "positrons", "secondary_positrons", "primary_DM_positron"};

#define SIZING(vec)\
  vec.resize(load_dat::fluxnum);\
  for(int i = 0; i < load_dat::fluxnum; i++) vec[i].resize(load_dat::iso_vectors[i].size() - 1);

chi2prop::chi2prop(): outdate(load_dat::fluxnum, true), outdate_solar(load_dat::fluxnum, false), keep(0), stype(force_field) {
  SIZING(Fluxes);
  SIZING(Fluxes_as);
}

int chi2prop::setphi(const vector <double> &phi_) {
  phi = phi_;
  std::fill(outdate_solar.begin(), outdate_solar.end(), true);
  return 0;
}

int chi2prop::setkeep(vector <load_dat::fluxes> &keep_) {
  keep = keep_;
  return 0;
}

inline spectrum chi2prop::get_spec(load_dat::fluxes element, unsigned i_iso) const {
  printDebugMsg("Check cposi", "rescale with cposi: %g", cposi);
  if((element == load_dat::secelecs || element == load_dat::secposis) && i_iso == 0) return cposi * Fluxes[element][0];
  else return Fluxes[element][i_iso];
}

int chi2prop::get_by_iso(double *Etmp, double *Ftmp, int ndat, load_dat::fluxes element) {
  printDebugMsg("Routine", ">>get_flux: %d, %d", load_dat::iso_vectors[element][0], load_dat::iso_vectors[element][1]);
  vector <int> isovec = load_dat::iso_vectors[element];

  for(unsigned j = 0; j < isovec.size() - 1; j++) {
    galpropmc->get_result(Etmp, Ftmp, isovec[j + 1], isovec[0]);
    Fluxes[element][j] = spectrum(Etmp, Ftmp, ndat);
  }

  printDebugMsg("Routine", "<<get_flux");
  return 1;
}

int chi2prop::get_by_name(double *Etmp, double *Ftmp, int ndat, load_dat:: fluxes element) {
  printDebugMsg("Routine", ">>get_flux: %s", load_dat::iso_names[element - load_dat::common_fluxnum].c_str());

  galpropmc->get_result(Etmp, Ftmp, load_dat::iso_names[element - load_dat::common_fluxnum].c_str());
  Fluxes[element][0] = spectrum(Etmp, Ftmp, ndat);

  printDebugMsg("Routine", "<<get_flux");
  return 1;
}

int chi2prop::get_flux(load_dat::fluxes element) {
  if(!outdate[element]) return 0;
  else {
    outdate[element] = false; 
    outdate_solar[element] = true; 
  }

  unsigned ndat=galpropmc->gcr[galpropmc->n_species-1].n_pgrid;
  double *Etmp = new double[ndat],
         *Ftmp = new double[ndat];

  if(element < load_dat::common_fluxnum) get_by_iso(Etmp, Ftmp, ndat, element);
  else get_by_name(Etmp, Ftmp, ndat, element);
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
  return 0;
}

int chi2prop::solar_modulas(load_dat::fluxes element) {
  if(!outdate_solar[element]) return 0;
  outdate_solar[element] = false;

  for(unsigned i = 0; i < Fluxes_as[element].size(); i++){
    solar_modulas(element, i);    
  }
  return 1;
}

int chi2prop::solar_modulas(load_dat::choice chc) {
  printDebugMsg("Routine", ">>solar_modulas")

  vector <load_dat::fluxes> elements = con_vectors(chc);

  for(unsigned i = 0; i < elements.size(); i++) solar_modulas(elements[i]);

  printDebugMsg("Routine", "<<solar_modulas")
  return 0;
}

spectrum chi2prop::sum_elements(const vector <load_dat::fluxes> &elevectors) const {
  spectrum res;

  for(unsigned i = 0; i < elevectors.size(); i++) {
    for(unsigned i_iso = 0; i_iso < Fluxes[elevectors[i]].size(); i_iso++)
      if(0 == i && 0 == i_iso) res = get_spec(elevectors[i], i_iso);
      else res += get_spec(elevectors[i], i_iso);
  }
  return res;
}

spectrum chi2prop::sum_elements(const vector <load_dat::fluxes> &elevectors, unsigned i_phi) const {
  spectrum res;

  for(unsigned i = 0; i < elevectors.size(); i++) {
    for(unsigned i_iso = 0; i_iso < Fluxes_as[elevectors[i]].size(); i_iso++)
      if(0 == i && 0 == i_iso) res = Fluxes_as[elevectors[i]][i_iso][i_phi];
      else res += Fluxes_as[elevectors[i]][i_iso][i_phi];
  }
  return res;
}

//output the lines of calculation
int chi2prop::print_lines(load_dat::choice chc, double phi, const spectrum &spec, const spectrum &ns_spec) const {
  cout << "#Result " << load_dat::data_name[chc] << "-phi-" << phi << endl;
  ns_spec.compare(spec);
  return 0;
}

#define IPHI [i_phi]
#define NOIPHI
#define CHI2PROP_GET_FLUX_FOR_P(vec, ind) \
      if(-1 == iso) { \
        for(unsigned i = 0; i < vec[flux].size(); i++)\
          if(0 == i) spectmp = vec[flux][i]ind;\
          else spectmp += vec[flux][i]ind;\
      } else\
        spectmp = vec[flux][iso]ind

spectrum chi2prop::flux_for_print(load_dat::fluxes flux, int iso, int i_phi) {
  spectrum spectmp;

  if(i_phi >= 0) {
    if(int(Fluxes_as[flux].size()) <= iso || int(phi.size()) <= i_phi) {
      cout << "Error::print_flux::You are trying to print unexist isotope" << endl;
      exit(1);
    }

      if (-1 != iso && outdate_solar[flux]) solar_modulas(flux, unsigned(iso));
      else solar_modulas(flux);
      CHI2PROP_GET_FLUX_FOR_P(Fluxes_as, IPHI);
  } else {
    CHI2PROP_GET_FLUX_FOR_P(Fluxes, NOIPHI);
  }

  return spectmp;
}

int chi2prop::print_flux(load_dat::choice chc, const string &fluxname, load_dat::fluxes flux, int iso, int i_phi) {
  vector <load_dat::fluxes> sub = {flux}, deno;
  return print_flux(chc, fluxname, sub, deno, iso, i_phi);
}
int chi2prop::print_flux(load_dat::choice chc, const string &fluxname, load_dat::fluxes flux, int iso) {
  return print_flux(chc, fluxname, flux, iso, -1);
}
int chi2prop::print_flux(load_dat::choice chc, const string &fluxname, load_dat::fluxes flux) {
  return print_flux(chc, fluxname, flux, -1);
}

int chi2prop::print_flux(load_dat::choice chc, const string &fluxname, const vector <load_dat::fluxes> &sub, const vector <load_dat::fluxes> &deno, int iso, int i_phi) {
  cout << "#Extra " << load_dat::data_name[chc] << "-extra-" << fluxname;
  if(i_phi >= 0) cout << "_with_phi_" << phi[i_phi];
  cout << endl;

  spectrum spec = flux_for_print(sub[0], iso, i_phi);
  for (unsigned i = 1; i < sub.size(); i++) spec += flux_for_print(sub[i], iso, i_phi);

  if(deno.size()) {
    spectrum denospec = flux_for_print(deno[0], iso, i_phi);
    for (unsigned i = 1; i < deno.size(); i++) spec += flux_for_print(deno[i], iso, i_phi);
    spec /= denospec;
  }

  spec.print();
  return 0;
}
int chi2prop::print_flux(load_dat::choice chc, const string &fluxname, const vector <load_dat::fluxes> &sub, const vector <load_dat::fluxes> &deno, int iso) {
  return print_flux(chc, fluxname, sub, deno, iso, -1);
}
int chi2prop::print_flux(load_dat::choice chc, const string &fluxname, const vector <load_dat::fluxes> &sub, const vector <load_dat::fluxes> &deno) {
  return print_flux(chc, fluxname, sub, deno, -1);
}

//compare the result with specified data
double chi2prop::calc_chi2(load_dat::choice chc, const spectrum &spec, const vector <int> &exp_subgrp, bool main_title) const {
  printDebugMsg("Routine", ">>calc_chi2: expgroupsize = %zu", exp_subgrp.size());
  double subsum = 0,
         tmpchi2;

  for(unsigned j = 0; j < exp_subgrp.size(); j++){
    bool main_head = (main_title && j == 0);
    tmpchi2 = load_dat::datas[chc].chi2(exp_subgrp[j], spec, pflag, main_head);

    if(pflag) cout<<"#chi2 is: "<<tmpchi2<<endl;
    subsum += tmpchi2;
  }
  printDebugMsg("Routine", "<<calc_chi2: %f", subsum); 
  return subsum;
}

double chi2prop::chi2(const vector <vector <int> > &exn, load_dat::choice chc){
  printDebugMsg("Routine", ">>chi2: %s", load_dat::data_name[chc].c_str());

  get_flux(chc);
  solar_modulas(chc);
  
  spectrum spec, specns;

  if(pflag) {
    specns = sum_elements(load_dat::sub_table[chc]);
    if(unsigned(chc) < load_dat::deno_table.size()) specns /= sum_elements(load_dat::deno_table[chc]);
  }

  double sum=0;
  if(pflag) cout << "#Data " << load_dat::data_name[chc] << endl;
  for(unsigned i_exp = 0; i_exp < phi.size(); i_exp++) {

    spec = sum_elements(load_dat::sub_table[chc], i_exp);
    if(unsigned(chc) < load_dat::deno_table.size()) spec /= sum_elements(load_dat::deno_table[chc], i_exp);

    if(pflag) print_lines(chc, phi[i_exp], spec, specns);
    sum += calc_chi2(chc, spec, exn[i_exp], i_exp == 0);
  }

  printDebugMsg("Routine", "<<chi2: %f", sum);
  return sum;
}
double chi2prop::chi2(const vector <double> &phi_, const vector <vector <int> > &exn, load_dat::choice chc){
  setphi(phi_);
  return chi2(exn, chc);
}
double chi2prop::chi2(const vector <vector <int> > &exn, load_dat::choice chc, bool pflag_){
  pflag = pflag_;
  return chi2(exn, chc);
}
double chi2prop::chi2(const vector <double> &phi_, const vector <vector <int> > &exn, load_dat::choice chc, bool pflag_){
  setphi(phi_);
  pflag = pflag_;
  return chi2(exn, chc);
}

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

int chi2prop::start(int iter) {
  printDebugMsg("Routine", ">> start:\titer %d", iter);
  std::fill(outdate.begin(), outdate.end(), true);
  if(0 != iter)
    for(unsigned i = 0; i < keep.size(); i++) outdate[keep[i]] = false;

  int res = galpropmc->start(iter);
  printDebugMsg("Routine", "<< start");
  return res;
}

int chi2prop::setpara(double *p, model_kind mod) {
  return galpropmc->set_params(p, mod);
}

