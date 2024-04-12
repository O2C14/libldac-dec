#include "ldaclib.h"
#include "ldac.h"


int ldaclib_assert_sampling_rate_index(int smplrate_id) {
  if ((LDAC_SMPLRATEID_0 <= smplrate_id) && (smplrate_id < LDAC_NSMPLRATEID)) {
    return LDAC_TRUE;
  } else {
    return LDAC_FALSE;
  }
}
int ldaclib_assert_supported_sampling_rate_index(int smplrate_id) {
  if ((LDAC_SMPLRATEID_0 <= smplrate_id) &&
      (smplrate_id < LDAC_NSUPSMPLRATEID)) {
    return LDAC_TRUE;
  } else {
    return LDAC_FALSE;
  }
}
int ldaclib_assert_nlnn_shift(int nlnn_shift) {
  if ((-2 <= nlnn_shift) && (nlnn_shift < LDAC_NSFTSTEP - 2)) {
    return LDAC_TRUE;
  } else {
    return LDAC_FALSE;
  }
}
int ldaclib_get_version() { return 0x10000; }
int ldaclib_get_major_version() { return 1; }
int ldaclib_get_minor_version() { return 0; }
int ldaclib_get_branch_version() { return 0; }
LDAC_RESULT ldaclib_get_sampling_rate_index(int smplrate, int *p_smplrate_id) {
  LDAC_RESULT result; // r0

  switch (smplrate) {
  case 44100:
    result = 0;
    *p_smplrate_id = 0;
    break;
  case 48000:
    *p_smplrate_id = 1;
    return 0;
  case 88200:
    *p_smplrate_id = 2;
    return 0;
  case 96000:
    result = 0;
    *p_smplrate_id = 3;
    break;
  default:
    return -2147467259;
  }
  return result;
}

LDAC_RESULT ldaclib_get_sampling_rate(int smplrate_id, int *p_smplrate) {
  if ((unsigned int)smplrate_id > 3)
    return -2147467259;
  *p_smplrate = ga_smplrate_ldac[smplrate_id];
  return 0;
}

LDAC_RESULT ldaclib_get_frame_samples(int smplrate_id, int *p_framesmpls) {
  if ((unsigned int)smplrate_id > 3)
    return -2147467259;
  *p_framesmpls = ga_framesmpls_ldac[smplrate_id];
  return 0;
}

LDAC_RESULT ldaclib_get_nlnn(int smplrate_id, int *p_nlnn) {
  if ((unsigned int)smplrate_id > 3)
    return -2147467259;
  *p_nlnn = ga_ln_framesmpls_ldac[smplrate_id];
  return 0;
}

LDAC_RESULT ldaclib_get_channel(int chconfig_id, int *p_ch) {
  if ((unsigned int)chconfig_id > 2)
    return -2147467259;
  *p_ch = ga_ch_ldac[chconfig_id];
  return 0;
}

LDAC_RESULT ldaclib_get_channel_config_index(int ch, int *p_chconfig_id) {
  if ((unsigned int)(ch - 1) > 1)
    return -2147467259;
  *p_chconfig_id = ga_chconfig_id_ldac[ch];
  return 0;
}

LDAC_RESULT ldaclib_check_nlnn_shift(int smplrate_id, int nlnn_shift) {
  if (!ldaclib_assert_sampling_rate_index(smplrate_id)) {
    return LDAC_E_FAIL;
  }
  if (!ldaclib_assert_supported_sampling_rate_index(smplrate_id)) {
    return LDAC_E_FAIL;
  }
  if (!ldaclib_assert_nlnn_shift(nlnn_shift)) {
    return LDAC_E_FAIL;
  }

  if (gaa_nlnn_shift_ldac[smplrate_id][nlnn_shift + 2] < 0) {
    return LDAC_E_FAIL;
  }

  return LDAC_S_OK;
}

HANDLE_LDAC ldaclib_get_handle() {
  /*
    HANDLE_LDAC hData;

    hData = (HANDLE_LDAC)malloc(sizeof(HANDLE_LDAC_STRUCT));
    if (hData != (HANDLE_LDAC)NULL) {
        clear_data_ldac(hData, sizeof(HANDLE_LDAC_STRUCT));
        hData->sfinfo.p_mempos = (char *)NULL;
        hData->error_code = LDAC_ERR_NONE;
    }

    return hData;
  */
  HANDLE_LDAC result; // r0
  HANDLE_LDAC hData;  // [sp+4h] [bp-Ch] BYREF

  // syspool_get_buff(&hData, sizeof(HANDLE_LDAC_STRUCT));
  hData = malloc(sizeof(HANDLE_LDAC_STRUCT));
  result = hData;
  if (hData) {
    memset(hData, 0, sizeof(HANDLE_LDAC_STRUCT));
    result = hData;
    hData->sfinfo.p_mempos = 0;
    result->error_code = 0;
  }
  return result;
}
LDAC_RESULT ldaclib_free_handle(HANDLE_LDAC hData) {
  if (!hData)
    return 0;
  if (!hData->sfinfo.p_mempos)
    free(hData);
  // syspool_free_size();
  return 0;
}
LDAC_RESULT ldaclib_set_config_info(HANDLE_LDAC hData, int smplrate_id,
                                    int chconfig_id, int frame_length,
                                    int frame_status) {
  int v6; // r1

  if ((unsigned int)smplrate_id > 5) {
    hData->error_code = 530;
    return -2147467259;
  }
  if ((unsigned int)smplrate_id > 3) {
    hData->error_code = 531;
    return -2147467259;
  }
  if ((unsigned int)chconfig_id > 2) {
    hData->error_code = 533;
    return -2147467259;
  }
  if ((unsigned int)(frame_length - 1) >= 0x400) {
    hData->error_code = 535;
    return -2147467259;
  }
  if (chconfig_id) {
    if ((unsigned int)(chconfig_id - 1) <= 1 &&
        (unsigned int)(frame_length - 22) <= 0x1EA)
      goto LABEL_9;
  LABEL_14:
    hData->error_code = 536;
    return -2147467259;
  }
  if ((unsigned int)(frame_length - 11) > 0xF5)
    goto LABEL_14;
LABEL_9:
  if ((unsigned int)frame_status > 3) {
    hData->error_code = 537;
    return -2147467259;
  } else {
    hData->sfinfo.cfg.smplrate_id = smplrate_id;
    hData->sfinfo.cfg.frame_length = frame_length;
    v6 = ga_ch_ldac[chconfig_id];
    hData->sfinfo.cfg.chconfig_id = chconfig_id;
    hData->sfinfo.cfg.frame_status = frame_status;
    hData->sfinfo.cfg.ch = v6;
    return 0;
  }
}

LDAC_RESULT ldaclib_get_config_info(HANDLE_LDAC hData, int *p_smplrate_id,
                                    int *p_chconfig_id, int *p_frame_length,
                                    int *p_frame_status) {
  *p_smplrate_id = hData->sfinfo.cfg.smplrate_id;
  *p_chconfig_id = hData->sfinfo.cfg.chconfig_id;
  *p_frame_length = hData->sfinfo.cfg.frame_length;
  *p_frame_status = hData->sfinfo.cfg.frame_status;
  return 0;
}

LDAC_RESULT ldaclib_get_frame_header(HANDLE_LDAC hData, uint8_t *p_stream,
                                     int *p_smplrate_id, int *p_chconfig_id,
                                     int *p_frame_length, int *p_frame_status) {
  if (unpack_frame_header_ldac(p_smplrate_id, p_chconfig_id, p_frame_length,
                               p_frame_status, p_stream))
    return 0;
  hData->error_code = 516;
  return -2147467259;
}

LDAC_RESULT ldaclib_check_frame_header(HANDLE_LDAC hData, int smplrate_id,
                                       int chconfig_id) {
  LDAC_RESULT result; // r0

  if ((unsigned int)smplrate_id <= 5) {
    if ((unsigned int)smplrate_id > 3) {
      hData->error_code = 531;
      return -2147467259;
    } else if ((unsigned int)chconfig_id > 2) {
      result = -2147467259;
      hData->error_code = 533;
    } else if (smplrate_id == hData->sfinfo.cfg.smplrate_id) {
      if (chconfig_id == hData->sfinfo.cfg.chconfig_id) {
        return 0;
      } else {
        result = -2147467259;
        hData->error_code = 534;
      }
    } else {
      hData->error_code = 532;
      return -2147467259;
    }
  } else {
    hData->error_code = 530;
    return -2147467259;
  }
  return result;
}
LDAC_RESULT ldaclib_init_decode(HANDLE_LDAC hData, int nlnn_shift) {
  unsigned int smplrate_id; // r3
  LDAC_RESULT result;       // r0
  SFINFO *p_sfinfo;         // r5
  int v6;                   // r0

  smplrate_id = hData->sfinfo.cfg.smplrate_id;
  if (smplrate_id > 3 || (unsigned int)(nlnn_shift + 2) > 4 ||
      gaa_nlnn_shift_ldac[smplrate_id][nlnn_shift + 2] < 0) {
    hData->error_code = 538;
    return 0x80004005;
  } else {
    p_sfinfo = &hData->sfinfo;
    v6 = ga_ln_framesmpls_ldac[smplrate_id] + nlnn_shift;
    hData->nlnn = v6;
    set_imdct_table_ldac(v6);
    result = init_decode_ldac(p_sfinfo);
    if (result) {
      hData->error_code = 570;
      return 0x80004005;
    }
  }
  return result;
}

LDAC_RESULT ldaclib_free_decode(HANDLE_LDAC hData) {
  if (!hData->sfinfo.p_mempos)
    free_decode_ldac(&hData->sfinfo);
  return 0;
}

LDAC_RESULT ldaclib_decode(HANDLE_LDAC hData, uint8_t *p_stream, char **ap_pcm,
                           int frame_length, int *p_nbytes_used,
                           LDAC_SMPL_FMT_T sample_format) {
  int v6;           // r4
  SFINFO *p_sfinfo; // r6
  int v10;          // r0
  int nlnn;         // r3
  //_BOOL4 v12; // r4
  int v12;
  int loc[7]; // [sp+4h] [bp-1Ch] BYREF

  v6 = hData->sfinfo.cfg.frame_length;
  loc[0] = 0;
  if (v6 > frame_length) {
    hData->error_code = 571;
    return 0x80004005;
  }
  if ((unsigned int)(sample_format - 2) > 3) {
    hData->error_code = 517;
    return 0x80004005;
  } else {
    p_sfinfo = &hData->sfinfo;
    v10 = unpack_raw_data_frame_ldac(&hData->sfinfo, p_stream, loc,
                                     p_nbytes_used);
    if (v10 <= 0) {
      decode_ldac(p_sfinfo);
      proc_imdct_ldac(p_sfinfo, hData->nlnn);
      nlnn = hData->nlnn;
      v12 = (unsigned int)(hData->sfinfo.error_code - 1) <= 0xFE;
      set_output_pcm_ldac(p_sfinfo, ap_pcm, sample_format, nlnn);
      return v12;
    }
    hData->error_code = v10;
    return 0x80004005;
  }
}
LDAC_RESULT ldaclib_clear_error_code(HANDLE_LDAC hData) {
  hData->error_code = 0;
  return 0;
}

LDAC_RESULT ldaclib_get_error_code(HANDLE_LDAC hData, int *p_error_code) {
  *p_error_code = hData->error_code;
  return 0;
}

LDAC_RESULT ldaclib_clear_internal_error_code(HANDLE_LDAC hData) {
  hData->sfinfo.error_code = 0;
  return 0;
}

LDAC_RESULT ldaclib_get_internal_error_code(HANDLE_LDAC hData,
                                            int *p_error_code) {
  *p_error_code = hData->sfinfo.error_code;
  return 0;
}
