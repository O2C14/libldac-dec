#include "ldacBT_internal.h"


DECLFUNC void ldacBT_param_clear(HANDLE_LDAC_BT hLdacBT) {
  if (hLdacBT) {
    hLdacBT->sfid = -1;
    hLdacBT->pcm.sf = -1;
    hLdacBT->tx.mtu = -1;
    hLdacBT->tx.tx_size = -1;
    hLdacBT->tx.pkt_hdr_sz = -1;
    hLdacBT->frmlen_tx = -1;
    hLdacBT->tx.nfrm_in_pkt = -1;
    hLdacBT->frmlen = -1;
    hLdacBT->tgt_nfrm_in_pkt = -1;
    hLdacBT->tgt_frmlen = -1;
    hLdacBT->tgt_eqmid = -1;
    hLdacBT->cm = -1;
    hLdacBT->cci = -1;
    hLdacBT->eqmid = -1;
    hLdacBT->transport = -1;
    hLdacBT->proc_mode = LDACBT_PROCMODE_UNSET;
    hLdacBT->error_code = 0;
    hLdacBT->error_code_api = 0;
    hLdacBT->frm_samples = 0;
    hLdacBT->pcm.ch = 0;
    hLdacBT->nshift = 0;
    hLdacBT->frm_status = 0;
    hLdacBT->bitrate = 0;
    hLdacBT->stat_alter_op = 0;
    hLdacBT->flg_decode_inited = 0;
    hLdacBT->pcm.fmt = LDACBT_SMPL_FMT_S24;
    hLdacBT->ap_pcm[0] = &hLdacBT->a_pcm[0];
    hLdacBT->ap_pcm[1] = &hLdacBT->a_pcm[LDACBT_MAX_LSU * LDACBT_PCM_WLEN_MAX];
    hLdacBT->pp_pcm = hLdacBT->ap_pcm;
    memset(hLdacBT->a_pcm, 0, sizeof(hLdacBT->a_pcm));
  }
}


DECLFUNC int ldacBT_check_ldaclib_error_code(HANDLE_LDAC_BT hLdacBT) {
  struct _handle_ldac_struct *hLDAC; // r5
  int error_code;                    // [sp+0h] [bp-18h] BYREF
  int internal_error_code;           // [sp+4h] [bp-14h] BYREF

  if (!hLdacBT)
    return -1;
  hLDAC = hLdacBT->hLDAC;
  if (!hLdacBT->hLDAC)
    return -1;
  ldaclib_get_error_code(hLdacBT->hLDAC, &error_code);
  ldaclib_get_internal_error_code(hLDAC, &internal_error_code);
  hLdacBT->error_code = internal_error_code | (error_code << 10);
  return 0;
}


DECLFUNC int ldacBT_assert_cm(int cm)
{
    if ((unsigned int)(cm - 1) <= 1)
        return 0;
    if (cm == 4)
        return 0;
    return LDACBT_ERR_ASSERT_CHANNEL_MODE;
}

DECLFUNC int ldacBT_assert_cci(int cci)
{
    if (!((unsigned int)cci > 2))
        return 0;
    return LDACBT_ERR_ASSERT_CHANNEL_CONFIG;
}

DECLFUNC int ldacBT_assert_sample_format(LDACBT_SMPL_FMT_T fmt)
{
    if (!((unsigned int)(fmt - 2) > 3))
        return 0;
    return LDACBT_ERR_ILL_SMPL_FORMAT;
}



DECLFUNC int ldacBT_assert_pcm_sampling_freq(int sampling_freq)
{
    if (sampling_freq == 44100 || sampling_freq == 48000 ||
        sampling_freq == 88200 || sampling_freq == 96000)
        return 0;
    return LDACBT_ERR_ILL_SAMPLING_FREQ;
}


DECLFUNC int ldacBT_interleave_pcm(unsigned char *p_pcm, const char **pp_pcm,
                          int nsmpl, int nch, LDACBT_SMPL_FMT_T fmt) {
  if (nsmpl <= 0) {
    return 0;
  }
  void *p_lch = pp_pcm[0];
  void *p_rch = pp_pcm[1];
  if (nch == 2) {
    switch (fmt) {
    case LDACBT_SMPL_FMT_S16:
      for (int i = 0; i < nsmpl; i++) {
        ((short *)p_pcm)[(i << 1) + 0] = ((short *)p_lch)[i];
        ((short *)p_pcm)[(i << 1) + 1] = ((short *)p_rch)[i];
      }
      return 2 * 2 * nsmpl;
    case LDACBT_SMPL_FMT_S24:
      for (int i = 0; i < nsmpl * 3; i += 3) {
        p_pcm[(i << 1) + 0] = ((unsigned char *)p_lch)[i + 0];
        p_pcm[(i << 1) + 1] = ((unsigned char *)p_lch)[i + 1];
        p_pcm[(i << 1) + 2] = ((unsigned char *)p_lch)[i + 2];
        p_pcm[(i << 1) + 3] = ((unsigned char *)p_rch)[i + 0];
        p_pcm[(i << 1) + 4] = ((unsigned char *)p_rch)[i + 1];
        p_pcm[(i << 1) + 5] = ((unsigned char *)p_rch)[i + 2];
      }
      return 2 * 3 * nsmpl;
    case LDACBT_SMPL_FMT_S32:
    case LDACBT_SMPL_FMT_F32:
      for (int i = 0; i < nsmpl; i++) {
        ((unsigned int *)p_pcm)[(i << 1) + 0] = ((unsigned int *)p_lch)[i];
        ((unsigned int *)p_pcm)[(i << 1) + 1] = ((unsigned int *)p_rch)[i];
      }
      return 2 * 4 * nsmpl;
    default:
      return 0;
    }
  } else {
    if (nch != 1)
      return 0;
    switch (fmt) {
    case LDACBT_SMPL_FMT_S16:
      memcpy(p_pcm, *pp_pcm, 2 * nsmpl);
      return 2 * nsmpl;
    case LDACBT_SMPL_FMT_S24:
      memcpy(p_pcm, *pp_pcm, 3 * nsmpl);
      return 3 * nsmpl;
    case LDACBT_SMPL_FMT_S32:
    case LDACBT_SMPL_FMT_F32:
      memcpy(p_pcm, *pp_pcm, 4 * nsmpl);
      return 4 * nsmpl;
    default:
      return 0;
    }
  }
}

DECLFUNC int ldacBT_cm_to_cci(int cm) {
  if (cm == 1)
    return 2;
  else
    return cm == 2;
}


DECLFUNC int ldacBT_cci_to_cm(int cci) {
  if (cci == 2)
    return 1;
  if (cci == 1)
    return 2;
  return 4;
}


