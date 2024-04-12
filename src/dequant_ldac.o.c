#include "ldac.h"
DECLFUNC void dequant_spectrum_ldac(AC *p_ac) {
  int nqus;        // r3
  int iqu;         // r4
  int nsps;        // r1
  int isp;         // r2
  SCALAR tmp;      // s14
  SCALAR *p_nspec; // r10
  int *p_qspec;    // r2
  int i;           // r3

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
  int nqus;        // lr
  SCALAR v2;       // s12
  int iqu;         // r1
  int nsps;        // r6
  int isp;         // r2
  SCALAR v9;       // s13
  SCALAR *p_nspec; // r3
  int i;           // r5
  int *p_rspec;    // r2

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
            p_nspec[i] = p_nspec[i] + (SCALAR)(v9 * (SCALAR)p_rspec[i]);
          }
        }
      }
    }
  }
}


DECLFUNC void clear_spectrum_ldac(AC *p_ac, int nsps) {
  memset(p_ac->p_acsub->a_spec, 0, sizeof(SCALAR) * nsps);
}


