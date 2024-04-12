#include "ldacBT_internal.h"
#define LDACBT_LIB_VER_MAJOR 2
#define LDACBT_LIB_VER_MINOR 0
#define LDACBT_LIB_VER_BRANCH 33
LDACBT_API int ldacBT_get_version() {
  // return 131105;
  return (((LDACBT_LIB_VER_MAJOR) << 16) | ((LDACBT_LIB_VER_MINOR) << 8) |
          (LDACBT_LIB_VER_BRANCH));
}

LDACBT_API void ldacBT_free_handle(HANDLE_LDAC_BT hLdacBT) {
  struct _handle_ldac_struct *hLDAC; // r3
  HANDLE_LDAC_BT v2;                 // r4

  if (hLdacBT) {
    hLDAC = hLdacBT->hLDAC;
    if (hLdacBT->hLDAC) {
      v2 = hLdacBT;
      if (hLdacBT->proc_mode == LDACBT_PROCMODE_DECODE) {
        ldaclib_free_decode(hLdacBT->hLDAC);
        hLDAC = v2->hLDAC;
      }
      hLdacBT = (HANDLE_LDAC_BT)ldaclib_free_handle(hLDAC);
      v2->hLDAC = 0;
    }
    free(hLdacBT);
  }
}
/* Get LDAC handle */
LDACBT_API HANDLE_LDAC_BT ldacBT_get_handle(void) {

  HANDLE_LDAC_BT hLdacBT;
  hLdacBT = (HANDLE_LDAC_BT)malloc(sizeof(struct _st_ldacbt_handle));
  if (hLdacBT == NULL) {
    return NULL;
  }

  //* Get ldaclib Handler
  if ((hLdacBT->hLDAC = ldaclib_get_handle()) == NULL) {
    ldacBT_free_handle(hLdacBT);
    return NULL;
  }

  ldacBT_param_clear(hLdacBT);
  return hLdacBT;
}
LDACBT_API void ldacBT_close_handle(HANDLE_LDAC_BT hLdacBT) {
  struct _handle_ldac_struct *hLDAC; // r0

  if (hLdacBT) {
    hLDAC = hLdacBT->hLDAC;
    if (hLDAC) {
      if (hLdacBT->proc_mode == LDACBT_PROCMODE_DECODE) {
        ldaclib_free_decode(hLDAC);
        hLDAC = hLdacBT->hLDAC;
      }
      ldaclib_clear_error_code(hLDAC);
      ldaclib_clear_internal_error_code(hLdacBT->hLDAC);
    }
    ldacBT_param_clear(hLdacBT);
  }
}

LDACBT_API int ldacBT_get_error_code(HANDLE_LDAC_BT hLdacBT) {
  int result;

  if (!hLdacBT)
    return 0x80000;
  ldacBT_check_ldaclib_error_code(hLdacBT);
  result = hLdacBT->error_code_api;
  if (result == 9999)
    return hLdacBT->error_code | 0x10000000;
  if (result)
    return hLdacBT->error_code | (result << 20);
  return result;
}

LDACBT_API int ldacBT_get_sampling_freq(HANDLE_LDAC_BT hLdacBT) {
  if (!hLdacBT)
    return -1;
  if (hLdacBT->proc_mode == LDACBT_PROCMODE_DECODE)
    return hLdacBT->pcm.sf;
  hLdacBT->error_code_api = 1000;
  return -1;
}

LDACBT_API int ldacBT_get_bitrate(HANDLE_LDAC_BT hLdacBT) {
  if (!hLdacBT)
    return -1;
  if (hLdacBT->proc_mode == LDACBT_PROCMODE_DECODE)
    return hLdacBT->bitrate;
  hLdacBT->error_code_api = 1000;
  return -1;
}

LDACBT_API int ldacBT_init_handle_decode(HANDLE_LDAC_BT hLdacBT, int cm, int sf,
                                         int nshift, int var0, int var1) {
  int v10;        // r0
  int v11;        // r5
  int v12;        // r8
  int v13;        // r2
  int v14;        // r9
  int v15;        // r3
  int sfid;       // [sp+8h] [bp-28h] BYREF
  int channel[9]; // [sp+Ch] [bp-24h] BYREF

  if (hLdacBT) {
    v10 = ldacBT_assert_cm(cm);
    hLdacBT->error_code_api = v10;
    if (!v10) {
      v11 = ldacBT_assert_pcm_sampling_freq(sf);
      hLdacBT->error_code_api = v11;
      if (!v11) {
        ldacBT_close_handle(hLdacBT);
        hLdacBT->flg_decode_inited = 0;
        hLdacBT->pcm.sf = sf;
        hLdacBT->nshift = nshift;
        hLdacBT->proc_mode = LDACBT_PROCMODE_DECODE;
        if (ldaclib_get_sampling_rate_index(sf, &sfid) >= 0) {
          hLdacBT->sfid = sfid;
          v12 = ldacBT_cm_to_cci(cm);
          if (ldaclib_get_channel(v12, channel) >= 0) {
            v13 = channel[0];
            hLdacBT->pcm.ch = channel[0];
            v14 = 165 * v13 - 3;
            if (ldaclib_set_config_info(hLdacBT->hLDAC, sfid, v12, v14, 0) >=
                0) {
              v15 = sfid;
              hLdacBT->frmlen = v14;
              hLdacBT->cm = cm;
              hLdacBT->cci = v12;
              hLdacBT->frm_status = 0;
              hLdacBT->sfid = v15;
              if (ldaclib_init_decode(hLdacBT->hLDAC, nshift) >= 0) {
                hLdacBT->flg_decode_inited = 1;
                if (ldaclib_get_frame_samples(hLdacBT->sfid,
                                              &hLdacBT->frm_samples) >= 0)
                  return 0;
              }
            }
          }
        }
        hLdacBT->error_code_api = 9999;
      }
    }
  }
  return -1;
}

LDACBT_API int ldacBT_decode(HANDLE_LDAC_BT hLdacBT, unsigned char *p_bs,
                             unsigned char *p_pcm, LDACBT_SMPL_FMT_T fmt,
                             int bs_bytes, int *used_bytes, int *wrote_bytes) {
  int v7;                // r3
  LDAC_SMPL_FMT_T v10;   // r8
  int v13;               // r0
  int v14;               // r0
  int v15;               // r9
  int v16;               // r5
  int v17;               // r2
  int flg_decode_inited; // r3
  int v19;               // r1
  int v20;               // r2
  LDAC_RESULT v21;       // r0
  int frm_samples;       // r2
  int v23;               // r0
  int sfid;              // [sp+8h] [bp-30h] BYREF
  int cci;               // [sp+Ch] [bp-2Ch] BYREF
  int frmlen;            // [sp+10h] [bp-28h] BYREF
  int frm_status[9];     // [sp+14h] [bp-24h] BYREF

  if (!hLdacBT)
    return 1;
  if (hLdacBT->proc_mode != LDACBT_PROCMODE_DECODE) {
    hLdacBT->error_code_api = 1000;
    return 2;
  }
  if (p_bs && p_pcm && used_bytes && wrote_bytes) {
    if (bs_bytes > 4) {
      v10 = fmt;
      v13 = ldacBT_assert_sample_format(fmt);
      hLdacBT->error_code_api = v13;
      if (v13) {
        return 5;
      } else {
        *used_bytes = 0;
        *wrote_bytes = 0;
        if (ldaclib_get_frame_header(hLdacBT->hLDAC, p_bs, &sfid, &cci, &frmlen,
                                     frm_status) >= 0) {
          v14 = ldacBT_assert_cci(cci);
          hLdacBT->error_code_api = v14;
          if (!v14) {
            v15 = ldacBT_cci_to_cm(cci);
            if (hLdacBT->flg_decode_inited &&
                ldaclib_check_frame_header(hLdacBT->hLDAC, sfid, cci) < 0) {
              ldaclib_get_sampling_rate(sfid, &hLdacBT->pcm.sf);
              if (ldacBT_init_handle_decode(hLdacBT, v15, hLdacBT->pcm.sf,
                                            hLdacBT->nshift, 0, 0))
                return 9;
              hLdacBT->error_code_api = 40;
              v16 = 8;
            } else {
              v16 = 0;
            }
            if (ldaclib_set_config_info(hLdacBT->hLDAC, sfid, cci, frmlen,
                                        frm_status[0]) < 0) {
              hLdacBT->error_code_api = 9999;
              return 10;
            }
            v17 = frmlen;
            flg_decode_inited = hLdacBT->flg_decode_inited;
            hLdacBT->sfid = sfid;
            hLdacBT->frmlen = v17;
            v19 = cci;
            v20 = frm_status[0];
            hLdacBT->cm = v15;
            hLdacBT->cci = v19;
            hLdacBT->frm_status = v20;
            if (!flg_decode_inited) {
              if (ldaclib_init_decode(hLdacBT->hLDAC, hLdacBT->nshift) < 0) {
                hLdacBT->error_code_api = 9999;
                return 11;
              }
              hLdacBT->flg_decode_inited = 1;
              if (ldaclib_get_frame_samples(hLdacBT->sfid,
                                            &hLdacBT->frm_samples) < 0) {
                hLdacBT->error_code_api = 1025;
                return 12;
              }
              if (!hLdacBT->flg_decode_inited)
                return 13;
            }
            v21 = ldaclib_decode(hLdacBT->hLDAC, p_bs + 3, hLdacBT->pp_pcm,
                                 bs_bytes - 3, used_bytes, v10);
            if (*used_bytes)
              *used_bytes += 3;
            if (v21 >= 0) {
              frm_samples = hLdacBT->frm_samples;
              hLdacBT->bitrate =
                  hLdacBT->pcm.sf * *used_bytes / frm_samples / 125;
              v23 = ldacBT_interleave_pcm(p_pcm, (const char **)hLdacBT->pp_pcm,
                                          frm_samples, hLdacBT->pcm.ch,
                                          (LDACBT_SMPL_FMT_T)v10);
              v7 = v16;
              *wrote_bytes = v23;
            } else {
              hLdacBT->error_code_api = 9999;
              return 14;
            }
            return v7;
          }
          return 7;
        } else {
          hLdacBT->error_code_api = 9999;
          return 6;
        }
      }
    } else {
      hLdacBT->error_code_api = 571;
      return 4;
    }
  }
  hLdacBT->error_code_api = 518;
  return 3;
}
