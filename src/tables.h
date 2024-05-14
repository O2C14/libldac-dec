#ifndef LDAC_TABLES_H
#define LDAC_TABLES_H

extern const int* gaa_rev_perm_ldac[LDAC_NUMLNN];
extern const SCALAR ga_rsf_ldac[LDAC_NIDWL];
extern HCDEC ga_hcdec_sf0_ldac[LDAC_MAXSFCBLEN_0-LDAC_MINSFCBLEN_0+1];
extern HCDEC ga_hcdec_sf1_ldac[LDAC_MAXSFCBLEN_2-LDAC_MINSFCBLEN_2+1];
extern const int gaa_2dimdec_spec_ldac[LDAC_N2DIMSPECDECTBL][2];
extern const int gaa_4dimdec_spec_ldac[LDAC_N4DIMSPECDECTBL][4];
extern const SCALAR* gaa_bwin_ldac[LDAC_NUMLNN];
extern const SCALAR* gaa_fwin_ldac[LDAC_NUMLNN];
extern const SCALAR* gaa_wsin_ldac[LDAC_NUMLNN];
extern const SCALAR* gaa_wcos_ldac[LDAC_NUMLNN];
extern const unsigned int ga_smplrate_ldac[LDAC_NSUPSMPLRATEID];
extern const unsigned short ga_framesmpls_ldac[LDAC_NSUPSMPLRATEID];
extern const unsigned char ga_ln_framesmpls_ldac[LDAC_NSUPSMPLRATEID];
extern const unsigned char ga_max_nbands_ldac[LDAC_NSUPSMPLRATEID];
extern const char gaa_nlnn_shift_ldac[LDAC_NSUPSMPLRATEID][LDAC_NSFTSTEP];
extern const unsigned char ga_ch_ldac[LDAC_NCHCONFIGID];
extern const unsigned char ga_chconfig_id_ldac[LDAC_MAXNCH + 1];
extern const char gaa_block_setting_ldac[LDAC_NCHCONFIGID][LDAC_MAXNCH + 2];
extern const unsigned char ga_nsps_ldac[LDAC_MAXNQUS];
extern const unsigned short ga_isp_ldac[LDAC_MAXNQUS + 1];
extern const unsigned char ga_nqus_ldac[LDAC_MAXNBANDS + 1];
extern const unsigned char ga_wl_ldac[LDAC_NIDWL];
extern const unsigned char gaa_resamp_grad_ldac[LDAC_MAXGRADQU][LDAC_MAXGRADQU];
extern const unsigned char gaa_sfcwgt_ldac[LDAC_NSFCWTBL][LDAC_MAXNQUS];
extern const SCALAR ga_sf_ldac[LDAC_NIDSF]; /* Q15 diff*/
extern const SCALAR ga_isf_ldac[LDAC_NIDSF];
extern const SCALAR ga_iqf_ldac[LDAC_NIDWL]; /* Q31 */

#endif