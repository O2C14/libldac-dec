#include "ldac.h"

/***************************************************************************************************
    Subfunction: Convert from 16bit Signed Integer PCM
***************************************************************************************************/
__inline static void byte_data_to_scalar_s16_ldac(char* p_in, SCALAR* p_out, int nsmpl) {
  int i;
  short* p_s;

  p_s = (short*)p_in;
  for (i = 0; i < nsmpl; i++) {
    *p_out++ = (SCALAR)*p_s++;
  }

  return;
}

/***************************************************************************************************
    Subfunction: Convert from 24bit Signed Integer PCM
***************************************************************************************************/
__inline static void byte_data_to_scalar_s24_ldac(char* p_in, SCALAR* p_out, int nsmpl) {
  int i, val;
  char* p_c;
  SCALAR scale = _scalar(1.0) / _scalar(65536.0);

  p_c = (char*)p_in;
  for (i = 0; i < nsmpl; i++) {
#ifdef LDAC_HOST_ENDIAN_LITTLE
    val = 0x000000ff & (*p_c++);
    val |= 0x0000ff00 & (*p_c++ << 8);
    val |= 0xffff0000 & (*p_c++ << 16);
#else                                      /* LDAC_HOST_ENDIAN_LITTLE */
    val = 0xffff0000 & (*p_c++ << 16);
    val |= 0x0000ff00 & (*p_c++ << 8);
    val |= 0x000000ff & (*p_c++);
#endif                                     /* LDAC_HOST_ENDIAN_LITTLE */
    *p_out++ = scale * (SCALAR)(val << 8); /* Sign Extension */
  }

  return;
}

/***************************************************************************************************
    Subfunction: Convert from 32bit Signed Integer PCM
***************************************************************************************************/
__inline static void byte_data_to_scalar_s32_ldac(char* p_in, SCALAR* p_out, int nsmpl) {
  int i;
  int* p_l;
  SCALAR scale = _scalar(1.0) / _scalar(65536.0);

  p_l = (int*)p_in;
  for (i = 0; i < nsmpl; i++) {
    *p_out++ = scale * (SCALAR)*p_l++;
  }

  return;
}

/***************************************************************************************************
    Subfunction: Convert from 32bit Float PCM
***************************************************************************************************/
__inline static void byte_data_to_scalar_f32_ldac(char* p_in, SCALAR* p_out, int nsmpl) {
  int i;
  float* p_f;
  SCALAR scale = _scalar(32768.0);

  p_f = (float*)p_in;
  for (i = 0; i < nsmpl; i++) {
    *p_out++ = scale * (SCALAR)*p_f++;
  }

  return;
}

/***************************************************************************************************
    Set Input PCM
***************************************************************************************************/
DECLFUNC void set_input_pcm_ldac(
    SFINFO* p_sfinfo, char* pp_pcm[], LDAC_SMPL_FMT_T format, int nlnn) {
  int ich, isp;
  int nchs = p_sfinfo->cfg.ch;
  int nsmpl = npow2_ldac(nlnn);
  SCALAR* p_time;

  if (format == LDAC_SMPL_FMT_S16) {
    for (ich = 0; ich < nchs; ich++) {
      p_time = p_sfinfo->ap_ac[ich]->p_acsub->a_time;
      for (isp = 0; isp < nsmpl; isp++) {
        p_time[isp] = p_time[nsmpl + isp];
      }
      byte_data_to_scalar_s16_ldac(pp_pcm[ich], p_time + nsmpl, nsmpl);
    }
  } else if (format == LDAC_SMPL_FMT_S24) {
    for (ich = 0; ich < nchs; ich++) {
      p_time = p_sfinfo->ap_ac[ich]->p_acsub->a_time;
      for (isp = 0; isp < nsmpl; isp++) {
        p_time[isp] = p_time[nsmpl + isp];
      }
      byte_data_to_scalar_s24_ldac(pp_pcm[ich], p_time + nsmpl, nsmpl);
    }
  } else if (format == LDAC_SMPL_FMT_S32) {
    for (ich = 0; ich < nchs; ich++) {
      p_time = p_sfinfo->ap_ac[ich]->p_acsub->a_time;
      for (isp = 0; isp < nsmpl; isp++) {
        p_time[isp] = p_time[nsmpl + isp];
      }
      byte_data_to_scalar_s32_ldac(pp_pcm[ich], p_time + nsmpl, nsmpl);
    }
  } else if (format == LDAC_SMPL_FMT_F32) {
    for (ich = 0; ich < nchs; ich++) {
      p_time = p_sfinfo->ap_ac[ich]->p_acsub->a_time;
      for (isp = 0; isp < nsmpl; isp++) {
        p_time[isp] = p_time[nsmpl + isp];
      }
      byte_data_to_scalar_f32_ldac(pp_pcm[ich], p_time + nsmpl, nsmpl);
    }
  }

  return;
}

DECLFUNC void set_output_pcm_ldac(
    SFINFO* p_sfinfo, char* pp_pcm[], LDAC_SMPL_FMT_T format, int nlnn) {
  SCALAR* p_time;
  int nchs = p_sfinfo->cfg.ch;
  int nsmpl = npow2_ldac(nlnn);
  if (nchs <= 0) {
    return;
  }
  switch (format) {
    case LDAC_SMPL_FMT_S16:
      for (int ich = 0; ich < nchs; ich++) {
        p_time = p_sfinfo->ap_ac[ich]->p_acsub->a_time;
        short* p_pcm = pp_pcm[ich];
        int temp;
        for (int isp = 0; isp < nsmpl; isp++) {
          temp = (int)(floor(p_time[isp] + _scalar(0.5)));
          if (temp < -0x8000) temp = -0x8000;
          if (temp >= 0x7FFF) temp = 0x7FFF;
          p_pcm[isp] = *(short*)(&temp);
        }
      }
      break;
    case LDAC_SMPL_FMT_S24:
      for (int ich = 0; ich < nchs; ich++) {
        p_time = p_sfinfo->ap_ac[ich]->p_acsub->a_time;
        char* p_pcm = pp_pcm[ich];
        int temp;
        for (int isp = 0; isp < nsmpl; isp++) {
          temp = (int)(floor(p_time[isp] * _scalar(256.0) + _scalar(0.5)));
          if (temp < -0x800000) temp = -0x800000;
          if (temp >= 0x7FFFFF) temp = 0x7FFFFF;
          p_pcm[(isp * 3) + 0] = ((char*)(&temp))[0];
          p_pcm[(isp * 3) + 1] = ((char*)(&temp))[1];
          p_pcm[(isp * 3) + 2] = ((char*)(&temp))[2];
        }
      }
      break;
    case LDAC_SMPL_FMT_S32:
      for (int ich = 0; ich < nchs; ich++) {
        p_time = p_sfinfo->ap_ac[ich]->p_acsub->a_time;
        int* p_pcm = pp_pcm[ich];
        long long temp;
        for (int isp = 0; isp < nsmpl; isp++) {
          temp = (long long)(floor(p_time[isp] * _scalar(65536.0) + _scalar(0.5)));
          if (temp < -0x80000000LL) temp = -0x80000000LL;
          if (temp >= 0x7FFFFFFFLL) temp = 0x7FFFFFFFLL;
          p_pcm[isp] = *(int*)(&temp);
        }
      }
      break;
    case LDAC_SMPL_FMT_F32:
      for (int ich = 0; ich < nchs; ich++) {
        p_time = p_sfinfo->ap_ac[ich]->p_acsub->a_time;
        float* p_pcm = pp_pcm[ich];
        float temp;
        for (int isp = 0; isp < nsmpl; isp++) {
          temp = p_time[isp] * (_scalar(1.0) / _scalar(32768.0));
          if (temp < -1.0) temp = -1.0;
          if (temp >= 1.0) temp = 1.0;
          p_pcm[isp] = temp;
        }
      }
      break;
    default:
      break;
  }
}
