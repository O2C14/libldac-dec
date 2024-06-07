#include "ldac.h"

/* Common Files */
#include "tables_ldac.o.c"
#include "tables_sigproc_ldac.o.c"
#include "setpcm_ldac.o.c"
#include "bitalloc_sub_ldac.o.c"
#include "memory_ldac.o.c"
#include "ldaclib_api.o.c"

/* Encoder/Decoder Files */
#include "mdct_ldac.c"
#include "imdct_ldac.o.c"
#include "sigana_ldac.c"
#include "quant_ldac.c"
#include "dequant_ldac.o.c"
#include "bitalloc_ldac.c"
#include "pack_ldac.c"
#include "unpack_ldac.o.c"
#include "encode_ldac.c"
#include "decode_ldac.o.c"


