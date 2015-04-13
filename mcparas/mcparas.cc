#include"anaspec.h"
#include"enum_utils.h"
#include"mcparas.h"

using std::vector;
using std::string;

const vector <string> mcparas::mode_name = {
  "PSRNP", "MUNP", "TAUNP", "WWNP", "UUNP", "BBNP", "TTNP", "FENP", "FMUNP", "FTAUNP",
  "PSRWP", "MUWP", "TAUWP", "WWWP", "UUWP", "BBWP", "TTWP", "FEWP", "FMUWP", "FTAUWP",
  "PSRNP", "MUNP", "TAUNP", "WWNP", "UUNP", "BBNP", "TTNP", "FENP", "FMUNP", "FTAUNP",
  "PSRNB", "MUNB", "TAUNB", "WWNB", "UUNB", "BBNB", "TTNB", "FENB", "FMUNB", "FTAUNB"
};
const vector <string> mcparas::elec_runNumber = { "mc_ele_PD", "mc_ele_DC", "mc_ele_DR", "mc_ele_DC2", "mc_ele_DR2", "mc_ele_DRC" },
      mcparas::BC_runNumber = { "mc_BC_PD", "mc_BC_DC", "mc_BC_DR", "mc_BC_DC2", "mc_BC_DR2", "mc_BC_DRC" },
      mcparas::prot_runNumber = { "mc_prot_PD", "mc_prot_DC", "mc_prot_DR", "mc_prot_DC2", "mc_prot_DR2", "mc_prot_DRC" },
      enum_name0 = {"pppc", "us"},
      enum_name1 = { "pulsar", "annihilate", "decay" },
      enum_name2 = { "PD", "DC", "DR", "DC2", "DR2", "DRC" },
      enum_name3 = { "nbk", "onebk", "twobk" };

const vector <vector <string> > mcparas::enum_names = {enum_name0, enum_name1, enum_name2, enum_name3};

mcparas::mcparas(Galprop *galprop): galdef(&(galprop->galdef)) {}

GETENUM(mcparas)

int mcparas::propagation_set(double *p, prop_mode prop) {
    galdef->D0_xx = p[0] * 1e28;
    galdef->D_g_1 = p[1];
    galdef->D_g_2 = p[1];
    galdef->D_rigid_br = 4 * 1e3;
    galdef->v_Alfven = p[3];
    galdef->dvdz_conv = p[4];
    galdef->z_max = p[5];
    galdef->z_min = -p[5];
    galdef->dz = p[5] / 20;

    galdef->eta = 1;

    if(DR2 == prop) galdef->eta = -0.4;
    if(DC2 == prop) {
      galdef->D_g_1 = 0;
      galdef->D_rigid_br = p[2] * 1e3;
    }

    const vector <int> convection = {0, 1, 0, 1, 0, 1},
          diff_reacc = {0 ,0, 1, 0, 1, 1};
    galdef->convection = convection[prop];
    galdef->diff_reacc = diff_reacc[prop];

    p_index += 6;
    return 0;
}

int mcparas::proton_set(double *p, inject_mode bks) {
  galdef->nuc_g_1 = p[0];

  switch(bks) {
  case nbk:
    galdef->nuc_g_2 = p[0];
    galdef->nuc_rigid_br = 10 * 1e3;
    galdef->proton_norm_flux = p[1] * 1e-9;
    p_index += 2;
    break;

  case onebk:
  case twobk:
    galdef->nuc_g_2 = p[1];
    galdef->nuc_rigid_br = p[2] * 1e3;
    galdef->proton_norm_flux = p[3] * 1e-9;
    p_index += 4;
    break;
  }
  return 0;
}

int mcparas::proton_set(double *p) {
  galdef->proton_norm_flux = pow(10, p[0]);
  galdef->nuc_g_1 = p[1];
  galdef->nuc_g_2 = p[2];
  galdef->nuc_rigid_br = pow(10, p[3]);
  p_index += 4;
  return 0;
}

int mcparas::electron_set(double *p, inject_mode bks) {
  galdef->electron_norm_flux = pow(10, p[0]);

  switch(bks) {
  case nbk:
    galdef->electron_g_1 = p[1];
    p_index += 2;
    break;

  case onebk:
    galdef->electron_g_0 = p[1];
    galdef->electron_g_1 = p[2];
    galdef->electron_rigid_br0 = pow(10, p[3]);
    p_index += 4;
    break;

  case twobk:
    galdef->electron_g_0 = p[1];
    galdef->electron_rigid_br0 = pow(10, p[2]);
    galdef->electron_g_1 = p[3];
    galdef->electron_rigid_br = pow(10, p[4]);
    galdef->electron_g_2 = p[5];
    p_index += 6;
  }
  return 0;
}

int mcparas::setdm(dm_mode dm, pppc_or_us pc) const {
  galdef->DM_int0 = dm;
  galdef->DM_int1 = pc;
  return 0;
}

int mcparas::setexotic(double *p, anaspec::branch_choice branch) {
  if(branch >= (int)anaspec::product_num) {
    cout << "mcparas::setexotic:: The required choice out of range" << endl;
    exit(1);
  }

  galdef->DM_double0 = pow(10, p[0]);

  if(galdef->DM_int0 == pulsar) { // in pulsar cases, input params are log(A), alpha, log(E_c)
    galdef->DM_double1 = p[1];
    galdef->DM_double2 = pow(10, p[2]);
    p_index += 3;
  } else { // in DM cases, input params are log(mc), log(sv)
    galdef->DM_double1 = pow(10, p[1]);
    p_index += 2;

    double bran[anaspec::product_num];
    for (int i = 0; i < (int)anaspec::product_num; i++)
      bran[i] = (i == branch) ? 1 : 0;

    galdef->DM_double2 = bran[0];
    galdef->DM_double3 = bran[1];
    galdef->DM_double4 = bran[2];
    galdef->DM_double5 = bran[3];
    galdef->DM_double6 = bran[4];
    galdef->DM_double7 = bran[5];
    galdef->DM_double8 = bran[6];
    galdef->DM_double9 = bran[7];
    galdef->DM_double10 = bran[8];
    galdef->DM_double11 = bran[9];
    galdef->DM_double12 = bran[10];
  }
  return 0;
}

int mcparas::setexotic(double *p, double *branches) {
  galdef->DM_double0 = pow(10, p[0]);

  if(galdef->DM_int0 == pulsar) { // in pulsar cases, input params are log(A), alpha, log(E_c)
    galdef->DM_double1 = p[1];
    galdef->DM_double2 = pow(10, p[2]);
    p_index += 3;
  } else { // in DM cases, input params are log(mc), log(sv)
    galdef->DM_double1 = pow(10, p[1]);
    p_index += 2;

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
  }
  return 0;
}

int mcparas::setexotic(double *p, mix_branch mixbran) {
  double branches[branch_num] = { 0 };

  switch(mixbran) {
  case emutau:
    if(1 - x - y
  }
}

int mcparas::setgalprop(Galprop *galprop) {
  galdef = &(galprop->galdef);
  return 0;
}

void mcparas::elec_setpara_pre(double *p, inject_mode bks, bool with_proton, dm_mode dmm, pppc_or_us pc) {
  p_index = 0;

  setdm(dmm, pc);
  if(with_proton) proton_set(p + p_index);

  electron_set(p + p_index, bks);
}

int mcparas::elec_setpara(double *p, inject_mode bks, anaspec::branch_choice bran,
                          bool with_proton = false, dm_mode dmm = annihilate, pppc_or_us pc = pppc) {
  elec_setpara_pre(p, bks, with_proton, dmm, pppc)
  setexotic(p + p_index, bran);
  return 0;
}

int mcparas::elec_setpara(double *p, inject_mode bks, double *branches,
                          bool with_proton = false, dm_mode dmm = annihilate, pppc_or_us pc = pppc) {
  elec_setpara_pre(p, bks, with_proton, dmm, pppc)
  setexotic(p + p_index, branches);
  return 0;
}

int mcparas::elec_setpara(double *p, anaspec::branch_choice bran, dm_mode dmm, pppc_or_us pc) {
  p_index = 0;
  setdm(dmm, pc);

  setexotic(p + p_index, bran);
  return 0;
}

int mcparas::elec_setpara(double *p, double *branches, dm_mode dmm, pppc_or_us pc) {
  p_index = 0;
  setdm(dmm, pc);

  setexotic(p + p_index, branches);
  return 0;
}

int mcparas::print() const {
    cout << "D0_xx :\t"            << galdef->D0_xx           << endl
         << "eta :\t"              << galdef->eta             << endl
         << "D_rigid_br :\t"       << galdef->D_rigid_br      << endl
         << "D_g_1 :\t"            << galdef->D_g_1           << endl
         << "D_g_2 :\t"            << galdef->D_g_2           << endl
         << "z_max :\t"            << galdef->z_max           << endl
         << "convection :\t"       << galdef->convection      << endl
         << "dvdz_conv :\t"        << galdef->dvdz_conv       << endl
         << "diff_reacc:\t"        << galdef->diff_reacc      << endl
         << "v_Alfven :\t"         << galdef->v_Alfven        << endl
         << "nuc_rigid_br :\t"     << galdef->nuc_rigid_br    << endl
         << "nuc_g_1 :\t"          << galdef->nuc_g_1         << endl
         << "nuc_g_2 :\t"          << galdef->nuc_g_2         << endl
         << "proton_norm_flux :\t" << galdef->proton_norm_flux << endl
         << "electron_norm_flux :\t" << galdef->electron_norm_flux << endl
         << "electron_g_0 :\t" << galdef->electron_g_0 << endl
         << "electron_rigid_br0 :\t" << galdef->electron_rigid_br0 << endl
         << "electron_g_1 :\t" << galdef->electron_g_1 << endl
         << "electron_rigid_br :\t" << galdef->electron_rigid_br << endl
         << "electron_g_2 :\t" << galdef->electron_g_2 << endl
         << "DM_NFW_rho0  :\t" << galdef->DM_NFW_rho0  << endl
         << "DM_NFW_rs    :\t" << galdef->DM_NFW_rs    << endl
         << "DM_NFW_gamma :\t" << galdef->DM_NFW_gamma << endl
         << "DM_int0 :\t" << galdef->DM_int0 << endl
         << "DM_int1 :\t" << galdef->DM_int1 << endl
         << "DM_double0 :\t" << galdef->DM_double0 << endl
         << "DM_double1 :\t" << galdef->DM_double1 << endl
         << "DM_double2 :\t" << galdef->DM_double2 << endl
         << "DM_double3 :\t" << galdef->DM_double3 << endl
         << "DM_double4 :\t" << galdef->DM_double4 << endl
         << "DM_double5 :\t" << galdef->DM_double5 << endl
         << "DM_double6 :\t" << galdef->DM_double6 << endl
         << "DM_double7 :\t" << galdef->DM_double7 << endl
         << "DM_double8 :\t" << galdef->DM_double8 << endl
         << "DM_double9 :\t" << galdef->DM_double9 << endl
         << "DM_double10 :\t" << galdef->DM_double10 << endl
         << "DM_double11 :\t" << galdef->DM_double11 << endl
         << "DM_double12 :\t" << galdef->DM_double12 << endl
         << "DM_double13 :\t" << galdef->DM_double13 << endl
         << "DM_double14 :\t" << galdef->DM_double14 << endl
         << "primary_DM_positron :\t" << galdef->primary_DM_positron << endl
         << "primary_DM_electron :\t" << galdef->primary_DM_electron << endl
         << "primary_electrons :\t" << galdef->primary_electrons << endl
         << "secondary_positrons :\t" << galdef->secondary_positrons << endl
         << "secondary_electrons :\t" << galdef->secondary_electrons << endl
         << "secondary_protons :\t" << galdef->secondary_protons << endl
         << "max_Z :\t" << galdef->max_Z << endl;
    return 0;
}

int mcparas::close_Z() const {
  galdef->max_Z = 0;
  galdef->secondary_positrons = 0;
  galdef->secondary_electrons = 0;
  return 0;
}
