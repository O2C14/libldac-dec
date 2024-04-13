#include "ldac.h"

DECLFUNC void dequant_spectrum_ldac(AC *p_ac) {
  int nqus;
  int iqu;
  int nsps;
  int isp;
  SCALAR tmp;
  SCALAR *p_nspec;
  int *p_qspec;
  int i;

  nqus = p_ac->p_ab->nqus;
  if (nqus > 0) {
    for (iqu = 0; iqu < nqus; iqu++) {
      isp = ga_isp_ldac[iqu];
      nsps = ga_nsps_ldac[iqu];
      tmp = ga_iqf_ldac[p_ac->a_idwl1[iqu]] * ga_sf_ldac[p_ac->a_idsf[iqu]];
      if (nsps) {
        p_nspec = p_ac->p_acsub->a_spec + isp;
        p_qspec = p_ac->a_qspec + isp;
        for (i = 0; i < nsps; i++) {
          p_nspec[i] = (SCALAR)p_qspec[i] * tmp;
        }
      }
    }
  }
}

DECLFUNC void dequant_residual_ldac(AC *p_ac) {
  int nqus;
  SCALAR v2;
  int iqu;
  int nsps;
  int isp;
  SCALAR v9;
  SCALAR *p_nspec;
  int i;
  int *p_rspec;

  nqus = p_ac->p_ab->nqus;
  if (nqus > 0) {
    v2 = ga_rsf_ldac[LDAC_MAXIDWL1];
    for (iqu = 0; iqu < nqus; iqu++) {
      if (p_ac->a_idwl2[iqu] > 0) {
        nsps = ga_nsps_ldac[iqu];
        isp = ga_isp_ldac[iqu];
        v9 = (SCALAR)(v2 * ga_iqf_ldac[p_ac->a_idwl2[0]]) *
             ga_sf_ldac[p_ac->a_idsf[0]];
        if (nsps) {
          p_nspec = p_ac->p_acsub->a_spec + isp;
          p_rspec = p_ac->a_rspec + isp;
          for (i = 0; i < nsps; i++) {
            p_nspec[i] += (SCALAR)(v9 * (SCALAR)p_rspec[i]);
          }
        }
      }
    }
  }
}

DECLFUNC void clear_spectrum_ldac(AC *p_ac, int nsps) {
  clear_data_ldac(p_ac->p_acsub->a_spec, sizeof(SCALAR) * nsps);
}
