#include "ldac.h"
#include "tables.h"
static void read_unpack_ldac(int nbits, STREAM* p_block, int* p_loc, int* p_idata) {
  STREAM* p_bufptr;
  register int bpos;
  register unsigned int tmp;
  bpos = *p_loc & LDAC_LOC_MASK;
  p_bufptr = p_block + (*p_loc >> LDAC_LOC_SHIFT);
  // from big-endian
  tmp = ((p_bufptr[2]) | (p_bufptr[1] << 8) | (p_bufptr[0] << 16));
  if (*p_loc < LDAC_MAXBITNUM) {
    *p_idata = ((tmp << bpos) & 0xFFFFFFu) >> (24 - nbits);
  } else {
    *p_idata = 0;
  }
  *p_loc += nbits;
}

static int unpack_scale_factor_0_ldac(AC* p_ac, STREAM* p_stream, int* p_loc) {
  HCDEC* p_hcsf;
  int iqu;
  int nqus;
  const unsigned char* p_tbl;
  int p_idata;

  nqus = p_ac->p_ab->nqus;
  read_unpack_ldac(LDAC_SFCBLENBITS, p_stream, p_loc, &p_ac->sfc_bitlen);
  p_ac->sfc_bitlen += LDAC_MINSFCBLEN_0;
  read_unpack_ldac(LDAC_IDSFBITS, p_stream, p_loc, &p_ac->sfc_offset);
  read_unpack_ldac(LDAC_SFCWTBLBITS, p_stream, p_loc, &p_ac->sfc_weight);
  read_unpack_ldac(p_ac->sfc_bitlen, p_stream, p_loc, p_ac->a_idsf);
  p_hcsf = ga_hcdec_sf0_ldac + (p_ac->sfc_bitlen - LDAC_MINSFCBLEN_0);
  p_tbl = gaa_sfcwgt_ldac[p_ac->sfc_weight];
  if (nqus > 1) {
    for (iqu = 1; iqu < nqus; iqu++) {
      read_unpack_ldac(p_hcsf->maxlen, p_stream, p_loc, &p_idata);
      *p_loc -= p_hcsf->maxlen - p_hcsf->p_tbl[p_hcsf->p_dec[p_idata]].len;
      p_ac->a_idsf[iqu] = (p_hcsf->p_dec[p_idata] + p_ac->a_idsf[iqu - 1]) & p_hcsf->mask;
      p_ac->a_idsf[iqu - 1] += p_ac->sfc_offset - p_tbl[iqu - 1];
    }
  }
  p_ac->a_idsf[nqus - 1] += p_ac->sfc_offset - p_tbl[nqus - 1];
  return 1;
}

DECLFUNC int unpack_frame_header_ldac(
    int* p_smplrate_id,
    int* p_chconfig_id,
    int* p_frame_length,
    int* p_frame_status,
    STREAM* p_stream) {
  int loc = 0;
  if (p_stream[0] != LDAC_SYNCWORD) {
    return 0;
  }
  loc += LDAC_SYNCWORDBITS;
  read_unpack_ldac(LDAC_SMPLRATEBITS, p_stream, &loc, p_smplrate_id);
  read_unpack_ldac(LDAC_CHCONFIG2BITS, p_stream, &loc, p_chconfig_id);
  read_unpack_ldac(LDAC_FRAMELEN2BITS, p_stream, &loc, p_frame_length);
  *p_frame_length += 1;
  read_unpack_ldac(LDAC_FRAMESTATBITS, p_stream, &loc, p_frame_status);

  return 1;
}

DECLFUNC int unpack_raw_data_frame_ldac(
    SFINFO* p_sfinfo, STREAM* p_stream, int* p_loc, int* p_nbytes_used) {
  int val;
  int FILLCODE;
  int nbytes_filled;
  int wl;
  int lsp, hsp;
  int idwl1, idwl2;
  int sfc_bitlen;
  HCDEC* p_hcsf;
  int sfc_mode;
  int nqus, nchs, nsps;
  int ich, ibk, iqu, isp, i, j;
  AB* p_ab;
  AC* p_ac;
  int ext_size;
  int p_idata[11];
  int nbytes_frame = p_sfinfo->cfg.frame_length;
  char nbks = gaa_block_setting_ldac[p_sfinfo->cfg.chconfig_id][1];
  if (nbks > 0) {
    for (ibk = 0; ibk != nbks; ++ibk) {
      p_ab = &p_sfinfo->p_ab[ibk];
      nchs = p_ab->blk_nchs;
      read_unpack_ldac(LDAC_NBANDBITS, p_stream, p_loc, &p_ab->nbands);
      p_ab->nbands += 2;
      if (p_ab->nbands > ga_max_nbands_ldac[*p_ab->p_smplrate_id]) {
        *p_ab->p_error_code = LDAC_ERR_SYNTAX_BAND;
        return LDAC_ERR_UNPACK_BLOCK_FAILED;
      }
      p_ab->nqus = ga_nqus_ldac[p_ab->nbands];
      read_unpack_ldac(LDAC_FLAGBITS, p_stream, p_loc, &p_ab->ext_flag);
      if (p_ab->ext_flag) {
        read_unpack_ldac(2, p_stream, p_loc, &p_ab->ext_mode);
        if (p_ab->ext_mode == 3) {
          for (ich = 0; ich < nchs; ++ich) {
            p_ac = p_ab->ap_ac[ich];
            read_unpack_ldac(7, p_stream, p_loc, &p_ac->ext_size);
            if (p_ac->ext_size > 0) {
              for (ext_size = p_ac->ext_size; ext_size > 0; ext_size -= 16)
                read_unpack_ldac(16, p_stream, p_loc, p_idata);
            }
          }
        }
      }
      read_unpack_ldac(LDAC_GRADMODEBITS, p_stream, p_loc, &p_ab->grad_mode);
      if (p_ab->grad_mode == LDAC_MODE_0) {
        read_unpack_ldac(LDAC_GRADQU0BITS, p_stream, p_loc, &p_ab->grad_qu_l);
        if (p_ab->grad_qu_l >= LDAC_MAXGRADQU) {
          *p_ab->p_error_code = LDAC_ERR_SYNTAX_GRAD_A;
          return LDAC_ERR_UNPACK_BLOCK_FAILED;
        }
        read_unpack_ldac(LDAC_GRADQU0BITS, p_stream, p_loc, &p_ab->grad_qu_h);
        p_ab->grad_qu_h++;
        if (p_ab->grad_qu_h >= LDAC_MAXGRADQU) {
          *p_ab->p_error_code = LDAC_ERR_SYNTAX_GRAD_B;
          return LDAC_ERR_UNPACK_BLOCK_FAILED;
        }
        if (p_ab->grad_qu_h < p_ab->grad_qu_l) {
          *p_ab->p_error_code = LDAC_ERR_SYNTAX_GRAD_C;
          return LDAC_ERR_UNPACK_BLOCK_FAILED;
        }
        read_unpack_ldac(LDAC_GRADOSBITS, p_stream, p_loc, &p_ab->grad_os_l);
        read_unpack_ldac(LDAC_GRADOSBITS, p_stream, p_loc, &p_ab->grad_os_h);
      } else {
        read_unpack_ldac(LDAC_GRADQU1BITS, p_stream, p_loc, &p_ab->grad_qu_l);
        if (p_ab->grad_qu_l > LDAC_DEFGRADQUH) {
          *p_ab->p_error_code = LDAC_ERR_SYNTAX_GRAD_D;
          return LDAC_ERR_UNPACK_BLOCK_FAILED;
        }
        read_unpack_ldac(LDAC_GRADOSBITS, p_stream, p_loc, &p_ab->grad_os_l);
        p_ab->grad_qu_h = LDAC_DEFGRADQUH;
        p_ab->grad_os_h = LDAC_DEFGRADOSH;
      }
      read_unpack_ldac(LDAC_NADJQUBITS, p_stream, p_loc, &p_ab->nadjqus);
      nqus = p_ab->nqus;
      if (nqus < p_ab->nadjqus) {
        *p_ab->p_error_code = LDAC_ERR_SYNTAX_GRAD_E;
        return LDAC_ERR_UNPACK_BLOCK_FAILED;
      }
      reconst_gradient_ldac(p_ab, 0, nqus);
      if (nchs > 0) {
        for (ich = 0; ich < nchs; ++ich) {
          p_ac = p_ab->ap_ac[ich];
          memset(p_ac->a_idsf, 0, sizeof(p_ac->a_idsf));
          read_unpack_ldac(LDAC_SFCMODEBITS, p_stream, p_loc, &p_ac->sfc_mode);
          sfc_mode = p_ac->sfc_mode;
          if (p_ac->ich) {
            if (sfc_mode) {
              read_unpack_ldac(LDAC_SFCBLENBITS, p_stream, p_loc, &p_ac->sfc_bitlen);
              p_hcsf = &ga_hcdec_sf1_ldac[p_ac->sfc_bitlen];
              p_ac->sfc_bitlen += 2;

              if (p_ac->p_ab->nqus > 0) {
                for (j = 0; j != p_ac->p_ab->nqus; ++j) {
                  read_unpack_ldac(p_hcsf->maxlen, p_stream, p_loc, p_idata);
                  int v88 = p_hcsf->p_dec[p_idata[0]];
                  char v89 = v88 & 0x1F;
                  *p_loc -= p_hcsf->maxlen - p_hcsf->p_tbl[v88].len;
                  sfc_bitlen = p_ac->sfc_bitlen;
                  int v91 = v88 >> (sfc_bitlen - 1);
                  v88 = v88 | (-1 << sfc_bitlen);
                  if ((v91 & 1) != 0) v89 = v88 & 0x1F;
                  p_ac->a_idsf[j] = ((unsigned char)p_ac->p_ab->ap_ac[0]->a_idsf[j] + v89) & 0x1F;
                }
              }
            } else if (!unpack_scale_factor_0_ldac(p_ac, p_stream, p_loc)) {
              return LDAC_ERR_UNPACK_BLOCK_FAILED;
            }
          } else if (sfc_mode) {
            nqus = p_ac->p_ab->nqus;
            read_unpack_ldac(LDAC_SFCBLENBITS, p_stream, p_loc, &p_ac->sfc_bitlen);
            p_ac->sfc_bitlen += 2;
            if (p_ac->sfc_bitlen > 4) {
              if (nqus > 0) {
                for (iqu = 0; iqu < nqus; ++iqu)
                  read_unpack_ldac(p_ac->sfc_bitlen, p_stream, p_loc, &p_ac->a_idsf[iqu]);
              }
            } else {
              read_unpack_ldac(LDAC_IDSFBITS, p_stream, p_loc, &p_ac->sfc_offset);
              read_unpack_ldac(LDAC_SFCWTBLBITS, p_stream, p_loc, &p_ac->sfc_weight);
              if (nqus > 0) {
                for (iqu = 0; iqu < nqus; ++iqu) {
                  read_unpack_ldac(p_ac->sfc_bitlen, p_stream, p_loc, &p_ac->a_idsf[iqu]);
                  p_ac->a_idsf[iqu] +=
                      p_ac->sfc_offset - (unsigned char)gaa_sfcwgt_ldac[p_ac->sfc_weight][iqu];
                }
              }
            }
          } else if (!unpack_scale_factor_0_ldac(p_ac, p_stream, p_loc)) {
            return LDAC_ERR_UNPACK_BLOCK_FAILED;
          }
          for (i = 0; i < LDAC_MAXNQUS; ++i) {
            if (p_ac->a_idsf[i] > 31) {
              *p_ac->p_ab->p_error_code = LDAC_ERR_SYNTAX_IDSF;
              return LDAC_ERR_UNPACK_BLOCK_FAILED;
            }
          }
          calc_add_word_length_ldac(p_ac);
          reconst_word_length_ldac(p_ac);
          nqus = p_ac->p_ab->nqus;
          memset(p_ac->a_tmp, 0, sizeof(p_ac->a_tmp));
          memset(p_ac->a_qspec, 0, sizeof(p_ac->a_qspec));
          if (nqus > 0) {
            for (iqu = 0; iqu < nqus; ++iqu) {
              idwl1 = p_ac->a_idwl1[iqu];
              hsp = ga_isp_ldac[iqu + 1];
              lsp = ga_isp_ldac[iqu];
              if (idwl1 == 1) {
                nsps = ga_nsps_ldac[iqu];
                if (nsps == 2) {
                  read_unpack_ldac(LDAC_2DIMSPECBITS, p_stream, p_loc, &val);
                  p_idata[0] = val;
                  p_ac->a_qspec[lsp] = gaa_2dimdec_spec_ldac[val][0];
                  p_ac->a_qspec[lsp + 1] = gaa_2dimdec_spec_ldac[val][1];
                } else if (nsps >> 2) {
                  for (isp = 0; isp < 4 * (nsps >> 2); isp += 4) {
                    read_unpack_ldac(LDAC_4DIMSPECBITS, p_stream, p_loc, &val);
                    if (val > 80) {
                      *p_ac->p_ab->p_error_code = LDAC_ERR_SYNTAX_SPEC;
                      return LDAC_ERR_UNPACK_BLOCK_FAILED;
                    }
                    p_ac->a_qspec[isp + 0 + lsp] = gaa_4dimdec_spec_ldac[val][0];
                    p_ac->a_qspec[isp + 1 + lsp] = gaa_4dimdec_spec_ldac[val][1];
                    p_ac->a_qspec[isp + 2 + lsp] = gaa_4dimdec_spec_ldac[val][2];
                    p_ac->a_qspec[isp + 3 + lsp] = gaa_4dimdec_spec_ldac[val][3];
                  }
                  p_idata[0] = val;
                }
              } else {
                wl = ga_wl_ldac[idwl1];
                int v38;
                int v41;
                v38 = 1 << (wl - 1);
                for (isp = lsp; hsp != isp; ++isp) {
                  read_unpack_ldac(wl, p_stream, p_loc, p_idata);
                  v41 = p_idata[0];
                  if ((v38 & p_idata[0]) != 0) {
                    v41 = (-1 << wl) | p_idata[0];
                  }
                  p_ac->a_qspec[isp] = v41;
                }
              }
            }
          }
          nqus = p_ac->p_ab->nqus;
          memset(p_ac->a_rspec, 0, sizeof(p_ac->a_rspec));
          if (nqus > 0) {
            for (iqu = 0; iqu < nqus; ++iqu) {
              idwl2 = p_ac->a_idwl2[iqu];
              if (idwl2 > 0) {
                lsp = ga_isp_ldac[iqu];
                hsp = ga_isp_ldac[iqu + 1];
                wl = ga_wl_ldac[idwl2];
                for (isp = lsp; hsp != isp; ++isp) {
                  read_unpack_ldac(wl, p_stream, p_loc, p_idata);
                  int v49 = p_idata[0];
                  if (((p_idata[0] >> (wl - 1)) & 1) != 0) v49 = p_idata[0] | (-1 << wl);
                  p_ac->a_rspec[isp] = v49;
                }
              }
            }
          }
        }
      }
      int nbits_padding;
      if (*p_loc > LDAC_BYTESIZE * nbytes_frame) return LDAC_ERR_FRAME_LENGTH_OVER;
      nbits_padding = ((*p_loc + LDAC_BYTESIZE - 1) / LDAC_BYTESIZE) * LDAC_BYTESIZE - *p_loc;

      if (nbits_padding > 0) {
        read_unpack_ldac(nbits_padding, p_stream, p_loc, p_idata);
        if (p_idata[0]) return LDAC_ERR_UNPACK_BLOCK_ALIGN;
      }
    }
  }
  nbytes_filled = nbytes_frame - *p_loc / LDAC_BYTESIZE;

  for (i = 0; i < nbytes_filled; i++) {
    read_unpack_ldac(LDAC_BYTESIZE, p_stream, p_loc, &FILLCODE);
    if (FILLCODE != LDAC_FILLCODE) return LDAC_ERR_UNPACK_FRAME_ALIGN;
  }
  *p_nbytes_used = *p_loc / LDAC_BYTESIZE;
  return 0;
}