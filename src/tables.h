#ifndef LDAC_TABLES_H
#define LDAC_TABLES_H
extern const SCALAR *gaa_wcos_ldac[LDAC_NUMLNN];
extern const int *gaa_rev_perm_ldac[2];
extern const SCALAR ga_rsf_ldac[16];
extern HCDEC ga_hcdec_sf0_ldac[4];
extern HCDEC ga_hcdec_sf1_ldac[4];
extern const int gaa_2dimdec_spec_ldac[8][2];
extern const int gaa_4dimdec_spec_ldac[81][4];
extern const SCALAR *gaa_bwin_ldac[2];

extern const unsigned int ga_smplrate_ldac[LDAC_NSUPSMPLRATEID];
extern const unsigned short ga_framesmpls_ldac[LDAC_NSUPSMPLRATEID];
extern const unsigned char ga_ln_framesmpls_ldac[LDAC_NSUPSMPLRATEID];
extern const unsigned char ga_max_nbands_ldac[LDAC_NSUPSMPLRATEID];
extern const char gaa_nlnn_shift_ldac[LDAC_NSUPSMPLRATEID][LDAC_NSFTSTEP];
extern const unsigned char ga_ch_ldac[LDAC_NCHCONFIGID];
extern const unsigned char ga_chconfig_id_ldac[LDAC_MAXNCH + 1];
extern const char gaa_block_setting_ldac[LDAC_NCHCONFIGID][LDAC_MAXNCH + 2];
extern const unsigned char ga_idsp_ldac[LDAC_MAXNQUS];
extern const unsigned char ga_nsps_ldac[LDAC_MAXNQUS];
extern const unsigned short ga_isp_ldac[LDAC_MAXNQUS + 1];
extern const unsigned char ga_nqus_ldac[LDAC_MAXNBANDS + 1];
extern const unsigned char ga_wl_ldac[LDAC_NIDWL];
extern const short gaa_ndim_wls_ldac[4][LDAC_NIDWL];
extern const int ga_2dimenc_spec_ldac[LDAC_N2DIMSPECENCTBL];
extern const int ga_4dimenc_spec_ldac[LDAC_N4DIMSPECENCTBL];
extern const unsigned char gaa_resamp_grad_ldac[LDAC_MAXGRADQU][LDAC_MAXGRADQU];
extern const unsigned char gaa_sfcwgt_ldac[LDAC_NSFCWTBL][LDAC_MAXNQUS];
extern const HC sa_hc_sf0_blen3_ldac[8];
extern const HC sa_hc_sf0_blen4_ldac[16];
extern const HC sa_hc_sf0_blen5_ldac[32];
extern const HC sa_hc_sf0_blen6_ldac[64];
extern const HC sa_hc_sf1_blen2_ldac[4];
extern const HC sa_hc_sf1_blen3_ldac[8];
extern const HC sa_hc_sf1_blen4_ldac[16];
extern const HC sa_hc_sf1_blen5_ldac[32];
extern HCENC ga_hcenc_sf0_ldac[LDAC_MAXSFCBLEN_0 - LDAC_MINSFCBLEN_0 + 1];
extern HCENC ga_hcenc_sf1_ldac[LDAC_MAXSFCBLEN_2 - LDAC_MINSFCBLEN_2 + 1];
extern const SCALAR *gaa_fwin_ldac[LDAC_NUMLNN];

extern const SCALAR *gaa_wsin_ldac[LDAC_NUMLNN];
extern const int *gaa_perm_ldac[LDAC_NUMLNN];
extern const SCALAR sa_fwin_1fs_ldac[LDAC_1FSLSU]; /* Q30 */
extern const SCALAR sa_fwin_2fs_ldac[LDAC_2FSLSU]; /* Q30 */
extern const SCALAR sa_wcos_1fs_ldac[LDAC_1FSLSU]; /* Q31 */
extern const SCALAR sa_wcos_2fs_ldac[LDAC_2FSLSU]; /* Q31 */
extern const SCALAR sa_wsin_1fs_ldac[LDAC_1FSLSU]; /* Q31 */
extern const SCALAR sa_wsin_2fs_ldac[LDAC_2FSLSU]; /* Q31 */
extern const int sa_perm_1fs_ldac[LDAC_1FSLSU];
extern const int sa_perm_2fs_ldac[LDAC_2FSLSU];
extern const SCALAR ga_sf_ldac[LDAC_NIDSF]; /* Q15 diff*/
extern const SCALAR ga_isf_ldac[LDAC_NIDSF];
extern const INT32 ga_qf_ldac[LDAC_NIDWL];   /* Q16 */
extern const SCALAR ga_iqf_ldac[LDAC_NIDWL];  /* Q31 */
extern const INT32 ga_irsf_ldac[LDAC_NIDWL]; /* Q15 */

#endif