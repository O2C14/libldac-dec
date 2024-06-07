#ifndef _PROTO_LDAC_H
#define _PROTO_LDAC_H

/***************************************************************************************************
    Function Declarations
***************************************************************************************************/
/* encode_ldac.c */
DECLFUNC LDAC_RESULT init_encode_ldac(SFINFO *);
DECLFUNC void calc_initial_bits_ldac(SFINFO *);
DECLFUNC void free_encode_ldac(SFINFO *);
DECLFUNC int encode_ldac(SFINFO *, int, int, int, int, int, int, int);

/* decode_ldac.c */
DECLFUNC LDAC_RESULT init_decode_ldac(SFINFO*);
DECLFUNC void free_decode_ldac(SFINFO*);
DECLFUNC void decode_ldac(SFINFO*);

/* setpcm_ldac.c */
DECLFUNC void set_input_pcm_ldac(SFINFO *, char *[], LDAC_SMPL_FMT_T, int);
DECLFUNC void set_output_pcm_ldac(SFINFO*, char*[], LDAC_SMPL_FMT_T, int);

/* mdct_ldac.c */
DECLFUNC void proc_mdct_ldac(SFINFO *, int);

/* imdct_ldac.c */
DECLFUNC void proc_imdct_ldac(SFINFO*, int);

/* sigana_ldac.c */
DECLFUNC int ana_frame_status_ldac(SFINFO *, int);

/* bitalloc_ldac.c */
DECLFUNC int alloc_bits_ldac(AB *);

/* bitalloc_sub_ldac.c */
DECLFUNC int encode_side_info_ldac(AB *);
DECLFUNC void calc_add_word_length_ldac(AC*);
DECLFUNC void reconst_gradient_ldac(AB*, int, int);
DECLFUNC void reconst_word_length_ldac(AC*);

/* quant_ldac.c */
DECLFUNC void norm_spectrum_ldac(AC *);
DECLFUNC void quant_spectrum_ldac(AC *);
DECLFUNC void quant_residual_ldac(AC *);

/* dequant_ldac.c */
DECLFUNC void clear_spectrum_ldac(AC*, int);
DECLFUNC void dequant_spectrum_ldac(AC*);
DECLFUNC void dequant_residual_ldac(AC*);

/* pack_ldac.c */
DECLFUNC void pack_frame_header_ldac(int, int, int, int, STREAM *);
DECLFUNC int pack_raw_data_frame_ldac(SFINFO *, STREAM *, int *, int *);
DECLFUNC int pack_null_data_frame_ldac(SFINFO *, STREAM *, int *, int *);

/* unpack_ldac.c */
DECLFUNC int unpack_frame_header_ldac(int*, int*, int*, int*, STREAM*);
DECLFUNC int unpack_raw_data_frame_ldac(SFINFO*, STREAM*, int*, int*);
DECLFUNC int pack_null_data_frame_ldac(SFINFO*, STREAM*, int*, int*);

/* tables_ldac.c */
DECLFUNC int get_block_nchs_ldac(int);

/* tables_sigproc_ldac.c */
DECLFUNC void set_mdct_table_ldac(int);
DECLFUNC void set_imdct_table_ldac(int);

/* memory_ldac.c */
DECLFUNC size_t align_ldac(size_t);
DECLFUNC void* calloc_ldac(SFINFO*, size_t, size_t);

#endif /* _PROTO_LDAC_H */
