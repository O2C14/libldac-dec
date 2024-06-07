#include "ldacBT_internal.h"
enum {
    /* 2-DH5 */
                     LDACBT_2DH5_02,  LDACBT_2DH5_03,  LDACBT_2DH5_04,  LDACBT_2DH5_05,
    LDACBT_2DH5_06,  LDACBT_2DH5_07,  LDACBT_2DH5_08,  LDACBT_2DH5_09,  LDACBT_2DH5_10,
    LDACBT_2DH5_11,  LDACBT_2DH5_12,  LDACBT_2DH5_13,  LDACBT_2DH5_14,
};

#define LDACBT_NO_DEF_ -1
DECLFUNC const LDACBT_EQMID_PROPERTY tbl_ldacbt_eqmid_property[] = {
    /* kbps,    ID               , label, ID for 2DH5     */
    /* 990 */ { LDACBT_EQMID_HQ, "HQ" , LDACBT_2DH5_02 },
    /* 660 */ { LDACBT_EQMID_SQ, "SQ" , LDACBT_2DH5_03 },
    /* 492 */ { LDACBT_EQMID_Q0, "Q0" , LDACBT_2DH5_04 },
    /* 396 */ { LDACBT_EQMID_Q1, "Q1" , LDACBT_2DH5_05 },
    /* 330 */ { LDACBT_EQMID_MQ, "MQ" , LDACBT_2DH5_06 },
    /* 282 */ { LDACBT_EQMID_Q2, "Q2" , LDACBT_2DH5_07 },
    /* 246 */ { LDACBT_EQMID_Q3, "Q3" , LDACBT_2DH5_08 },
    /* 216 */ { LDACBT_EQMID_Q4, "Q4" , LDACBT_2DH5_09 },
    /* 198 */ { LDACBT_EQMID_Q5, "Q5" , LDACBT_2DH5_10 },
    /* 180 */ { LDACBT_EQMID_Q6, "Q6" , LDACBT_2DH5_11 },
    /* 162 */ { LDACBT_EQMID_Q7, "Q7" , LDACBT_2DH5_12 },
    /* 150 */ { LDACBT_EQMID_Q8, "Q8" , LDACBT_2DH5_13 },
    /* 138 */ { LDACBT_EQMID_END, "Q9" , LDACBT_2DH5_14 },
};

/* LDAC config table
 *  - NFRM/PCKT must be less than 16.
 */
DECLFUNC const LDACBT_CONFIG tbl_ldacbt_config[] = {
/*
 *   index          , NFRM , LDAC  , FRM   
 *                  , ---- ,  FRM  ,  LEN  
 *                  , PCKT ,   LEN ,    /CH
 *                  ,      , [byte], [byte]
 */
    { LDACBT_2DH5_02,     2,    330,   165},
    { LDACBT_2DH5_03,     3,    220,   110},
    { LDACBT_2DH5_04,     4,    164,    82},
    { LDACBT_2DH5_05,     5,    132,    66},
    { LDACBT_2DH5_06,     6,    110,    55},
    { LDACBT_2DH5_07,     7,     94,    47},
    { LDACBT_2DH5_08,     8,     82,    41},
    { LDACBT_2DH5_09,     9,     72,    36},
    { LDACBT_2DH5_10,    10,     66,    33},
    { LDACBT_2DH5_11,    11,     60,    30},
    { LDACBT_2DH5_12,    12,     54,    27},
    { LDACBT_2DH5_13,    13,     50,    25},
    { LDACBT_2DH5_14,    14,     46,    23},
};