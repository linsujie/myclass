#include"anaspec.h"
#include"enum_utils.h"
#include"mcparas.h"

using std::vector;
using std::string;

const vector <string> mcparas::elec_runNumber = { "mc_ele_PD", "mc_ele_DC", "mc_ele_DR", "mc_ele_DC2", "mc_ele_DR2", "mc_ele_DRC" },
      mcparas::BC_runNumber = { "mc_BC_PD", "mc_BC_DC", "mc_BC_DR", "mc_BC_DC2", "mc_BC_DR2", "mc_BC_DRC" },
      mcparas::prot_runNumber = { "mc_prot_PD", "mc_prot_DC", "mc_prot_DR", "mc_prot_DC2", "mc_prot_DR2", "mc_prot_DRC" },
      enum_name0 = {"pppc", "us"},
      enum_name1 = { "pulsar", "annihilate", "decay" },
      enum_name2 = { "PD", "DC", "DR", "DC2", "DR2", "DRC" },
      enum_name3 = { "nbk", "onebk", "twobk" },
      enum_name4 = { "emutau", "mutaub", "emutaub", "b3l" };

const vector <vector <string> > mcparas::enum_names = {enum_name0, enum_name1, enum_name2, enum_name3, enum_name4};

mcparas::mcparas(Galprop *galprop): galdef(&(galprop->galdef)) {}

GETENUM(mcparas)

int mcparas::setgalprop(Galprop *galprop) throw() {
  galdef = &(galprop->galdef);
  return 0;
}

int mcparas::setpara(double *p_) throw() {
  p = p_;
  p_index = 0;
  return 0;
}

double mcparas::getp() throw() {
  p_index++;
  return p[p_index - 1];
}

int mcparas::print() const throw() {
  cout << "D0_xx :\t"               << galdef->D0_xx                 << endl
    << "eta :\t"                    << galdef->eta                   << endl
    << "D_rigid_br :\t"             << galdef->D_rigid_br            << endl
    << "D_g_1 :\t"                  << galdef->D_g_1                 << endl
    << "D_g_2 :\t"                  << galdef->D_g_2                 << endl
    << "z_max :\t"                  << galdef->z_max                 << endl
    << "convection :\t"             << galdef->convection            << endl
    << "dvdz_conv :\t"              << galdef->dvdz_conv             << endl
    << "diff_reacc:\t"              << galdef->diff_reacc            << endl
    << "v_Alfven :\t"               << galdef->v_Alfven              << endl
    << endl
    << "nuc_rigid_br :\t"           << galdef->nuc_rigid_br          << endl
    << "nuc_g_1 :\t"                << galdef->nuc_g_1               << endl
    << "nuc_g_2 :\t"                << galdef->nuc_g_2               << endl
    << "proton_norm_flux :\t"       << galdef->proton_norm_flux      << endl
    << endl
    << "electron_norm_flux :\t"     << galdef->electron_norm_flux    << endl
    << "electron_g_0 :\t"           << galdef->electron_g_0          << endl
    << "electron_rigid_br0 :\t"     << galdef->electron_rigid_br0    << endl
    << "electron_g_1 :\t"           << galdef->electron_g_1          << endl
    << "electron_rigid_br :\t"      << galdef->electron_rigid_br     << endl
    << "electron_g_2 :\t"           << galdef->electron_g_2          << endl
    << endl
    << "DM_NFW_rho0  :\t"           << galdef->DM_NFW_rho0           << endl
    << "DM_NFW_rs    :\t"           << galdef->DM_NFW_rs             << endl
    << "DM_NFW_gamma :\t"           << galdef->DM_NFW_gamma          << endl
    << "DM_int0 :\t"                << galdef->DM_int0               << endl
    << "DM_int1 :\t"                << galdef->DM_int1               << endl
    << endl
    << "DM_double0 :\t"             << galdef->DM_double0            << endl
    << "DM_double1 :\t"             << galdef->DM_double1            << endl
    << "DM_double2 :\t"             << galdef->DM_double2            << endl
    << "DM_double3 :\t"             << galdef->DM_double3            << endl
    << "DM_double4 :\t"             << galdef->DM_double4            << endl
    << "DM_double5 :\t"             << galdef->DM_double5            << endl
    << "DM_double6 :\t"             << galdef->DM_double6            << endl
    << "DM_double7 :\t"             << galdef->DM_double7            << endl
    << "DM_double8 :\t"             << galdef->DM_double8            << endl
    << "DM_double9 :\t"             << galdef->DM_double9            << endl
    << "DM_double10 :\t"            << galdef->DM_double10           << endl
    << "DM_double11 :\t"            << galdef->DM_double11           << endl
    << "DM_double12 :\t"            << galdef->DM_double12           << endl
    << "DM_double13 :\t"            << galdef->DM_double13           << endl
    << "DM_double14 :\t"            << galdef->DM_double14           << endl
    << endl
    << "primary_DM_positron :\t"    << galdef->primary_DM_positron   << endl
    << "primary_DM_electron :\t"    << galdef->primary_DM_electron   << endl
    << "primary_DM_antip :\t"       << galdef->primary_DM_antip      << endl
    << "primary_electrons :\t"      << galdef->primary_electrons     << endl
    << "secondary_positrons :\t"    << galdef->secondary_positrons   << endl
    << "secondary_electrons :\t"    << galdef->secondary_electrons   << endl
    << "secondary_protons :\t"      << galdef->secondary_protons     << endl
    << "secondary_antiprotons :\t"  << galdef->secondary_antiprotons << endl
    << "tertiary_antiprotons: \t"   << galdef->tertiary_antiprotons  << endl
    << "max_Z :\t"                  << galdef->max_Z                 << endl;
  return 0;
}

int mcparas::err_info(errtype &err) throw() {
  switch(err) {
  case branch_out_of_range:
    cout << "mcparas:: The required branch choice out of range" << endl;
    break;

  case set_branch_for_pulsar:
    cout << "mcparas:: You are trying to set branch infomations for pulsar case" << endl;
    break;

  case mix_branch_out_of_range:
    cout << "mcparas:: The parameter for mix branches out of range" << endl;
    break;
  }
  return 0;
}

int mcparas::propagation_set_yuan(prop_mode prop) throw() {
  double *point = p + p_index;

  galdef->D0_xx = point[0] * 1e28;
  galdef->D_g_1 = point[1];
  galdef->D_g_2 = point[1];
  galdef->z_max = point[2];
  galdef->z_min = -point[2];
  galdef->dz = point[2] / 20;
  galdef->D_rigid_br = 4 * 1e3;
  galdef->v_Alfven = point[3];
  galdef->dvdz_conv = point[4];

  galdef->eta = 1;

  if(DR2 == prop) galdef->eta = -0.4;
  if(DC2 == prop) {
    galdef->D_g_1 = 0;
    galdef->D_rigid_br = point[3] * 1e3;
  }

  const vector <int> convection = {0, 1, 0, 1, 0, 1},
        diff_reacc = {0 ,0, 1, 0, 1, 1};
  galdef->convection = convection[prop];
  galdef->diff_reacc = diff_reacc[prop];

  p_index += 5;
  return 0;
}

int mcparas::propagation_set_whole(prop_mode prop) throw() {
  double *point = p + p_index;

  galdef->D0_xx = point[0] * 1e28;
  galdef->D_g_1 = point[1];
  galdef->D_g_2 = point[1];
  galdef->D_rigid_br = 4 * 1e3;
  galdef->v_Alfven = point[3];
  galdef->dvdz_conv = point[4];
  galdef->z_max = point[5];
  galdef->z_min = -point[5];
  galdef->dz = point[5] / 20;

  galdef->eta = 1;

  if(DR2 == prop) galdef->eta = -0.4;
  if(DC2 == prop) {
    galdef->D_g_1 = 0;
    galdef->D_rigid_br = point[2] * 1e3;
  }

  const vector <int> convection = {0, 1, 0, 1, 0, 1},
        diff_reacc = {0 ,0, 1, 0, 1, 1};
  galdef->convection = convection[prop];
  galdef->diff_reacc = diff_reacc[prop];

  p_index += 6;
  return 0;
}

int mcparas::propagation_set(prop_mode prop) throw() {
  double *point = p + p_index;
  unsigned tmpind = 0;

  galdef->D0_xx = point[0] * 1e28;
  galdef->D_g_1 = DC2 == prop ? 0 : point[1];
  galdef->D_g_2 = point[1];
  galdef->D_rigid_br = 4 * 1e3;
  tmpind += 2;

  if (DC2 == prop) {
    galdef->D_rigid_br = point[tmpind] * 1e3;
    tmpind++;
  }

  if (DR == prop || DR2 == prop || DRC == prop) {
    galdef->v_Alfven = point[tmpind];
    tmpind++;
  }

  if (DC == prop || DC2 == prop || DRC == prop) {
    galdef->dvdz_conv = point[tmpind];
    tmpind++;
  }

  galdef->z_max = point[tmpind];
  galdef->z_min = -point[tmpind];
  galdef->dz = point[tmpind] / 20;
  tmpind++;

  galdef->eta = 1;

  if(DR2 == prop) galdef->eta = -0.4;

  const vector <int> convection = {0, 1, 0, 1, 0, 1},
        diff_reacc = {0 ,0, 1, 0, 1, 1};
  galdef->convection = convection[prop];
  galdef->diff_reacc = diff_reacc[prop];

  p_index += tmpind;
  return 0;
}

int mcparas::proton_set(inject_mode bks) throw() {
  double *point = p + p_index;
  galdef->nuc_g_1 = point[0];

  switch(bks) {
  case nbk:
    galdef->nuc_g_2 = point[0];
    galdef->nuc_rigid_br = 10 * 1e3;
    galdef->proton_norm_flux = point[1] * 1e-9;
    p_index += 2;
    break;

  case onebk:
  case twobk:
    galdef->nuc_g_2 = point[1];
    galdef->nuc_rigid_br = point[2] * 1e3;
    galdef->proton_norm_flux = point[3] * 1e-9;
    p_index += 4;
    break;
  }
  return 0;
}

int mcparas::proton_set() throw() {
  double *point = p + p_index;

  galdef->proton_norm_flux = pow(10, point[0]);
  galdef->nuc_g_1 = point[1];
  galdef->nuc_g_2 = point[2];
  galdef->nuc_rigid_br = pow(10, point[3]);
  p_index += 4;
  return 0;
}

int mcparas::electron_set(inject_mode bks) throw() {
  double *point = p + p_index;

  galdef->electron_norm_flux = pow(10, point[0]);

  switch(bks) {
  case nbk:
    galdef->electron_g_1 = point[1];
    p_index += 2;
    break;

  case onebk:
    galdef->electron_g_0 = point[1];
    galdef->electron_g_1 = point[2];
    galdef->electron_rigid_br0 = pow(10, point[3]);
    p_index += 4;
    break;

  case twobk:
    galdef->electron_g_0 = point[1];
    galdef->electron_rigid_br0 = pow(10, point[2]);
    galdef->electron_g_1 = point[3];
    galdef->electron_rigid_br = pow(10, point[4]);
    galdef->electron_g_2 = point[5];
    p_index += 6;
  }
  return 0;
}

int mcparas::exotic_set() throw() {
  double *point = p + p_index;

  galdef->DM_double0 = pow(10, point[0]);

  if(galdef->DM_int0 == pulsar) { // in pulsar cases, input params are log(A), alpha, log(E_c)
    galdef->DM_double1 = point[1];
    galdef->DM_double2 = pow(10, point[2]);
    p_index += 3;
  } else { // in DM cases, input params are log(mc), log(sv)
    galdef->DM_double1 = pow(10, point[1]);
    p_index += 2;
  }

  return 0;
}

int mcparas::branch_set(anaspec::branch_choice branch) const throw(errtype) {
  if(branch >= anaspec::branch_num) {
    throw(branch_out_of_range);
  }

    double bran[anaspec::branch_num] = { 0 };
    bran[branch] = 1;

    return branch_set(bran);
}

int mcparas::branch_set(double *branches) const throw(errtype) {
  if (galdef->DM_int0 == pulsar) throw(set_branch_for_pulsar); // Don't initial the branch in the pulsar case;

  galdef->DM_double2 = branches[0];
  galdef->DM_double3 = branches[1];
  galdef->DM_double4 = branches[2];
  galdef->DM_double5 = branches[3];
  galdef->DM_double6 = branches[4];
  galdef->DM_double7 = branches[5];
  galdef->DM_double8 = branches[6];
  galdef->DM_double9 = branches[7];
  galdef->DM_double10 = branches[8];
  galdef->DM_double11 = branches[9];
  galdef->DM_double12 = branches[10];

  return 0;
}

int mcparas::branch_set(mix_branch mixbran) throw(errtype) {
  //setting the branches for mixing branch cases, return 1 if the inputed parameters are inappropriate;
  double *point = p + p_index,
         branches[anaspec::branch_num] = { 0 };
  double x, y, z, sqsum, denom;

  switch(mixbran) {
  case emutau:
    if(1 - point[0] - point[1] < -1e-7) throw(mix_branch_out_of_range);
    branches[anaspec::electron] = point[0];
    branches[anaspec::mu] = point[1];
    branches[anaspec::tau] = 1 - point[0] - point[1];
    p_index += 2;
    break;

  case mutaub:
    if(1 - point[0] - point[1] < -1e-7) throw(mix_branch_out_of_range);
    branches[anaspec::mu] = point[0];
    branches[anaspec::tau] = point[1];
    branches[anaspec::bottom] = 1 - point[0] - point[1];
    p_index += 2;
    break;

  case emutaub:
    if(1 - point[0] - point[1] - point[2] < -1e-7) throw(mix_branch_out_of_range);
    branches[anaspec::electron] = point[0];
    branches[anaspec::mu] = point[1];
    branches[anaspec::tau] = point[2];
    branches[anaspec::bottom] = 1 - point[0] - point[1] - point[2];
    p_index += 3;
    break;

  case b3l:
    x = point[0], y = point[1];
    z = 1 - x * x - y * y + 1e-7;
    if(z < 0) throw(mix_branch_out_of_range);
    z = sqrt(z);
    sqsum = (x + y + z) * (x + y + z);
    denom = 9 * 1.5 + 5.0 / 3 * sqsum;

    branches[anaspec::electron] = 9 * x * x / denom;
    branches[anaspec::mu] = 9 * y * y / denom;
    branches[anaspec::tau] = 9 * z * z / denom;
    branches[anaspec::up] = 3 * sqsum / (3 * denom); //for u, d, s
    branches[anaspec::charm] = sqsum / (3 * denom);
    branches[anaspec::bottom] = sqsum / (3 * denom);
    branches[anaspec::top] = sqsum / (3 * denom);
    p_index += 2;
    break;
  }

  return branch_set(branches);
}

int mcparas::setdm(dm_mode dm, pppc_or_us pc) const throw() {
  galdef->DM_int0 = dm;
  galdef->DM_int1 = pc;
  return 0;
}

int mcparas::close_Z() const throw() {
  galdef->max_Z = 0;
  galdef->secondary_positrons = 0;
  galdef->secondary_electrons = 0;
  return 0;
}

Galdef* mcparas::getpoint() const throw() {
  return galdef;
}

unsigned mcparas::np() const throw() {
  return p_index;
}
