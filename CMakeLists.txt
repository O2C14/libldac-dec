cmake_minimum_required(VERSION 3.15)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=implicit-function-declaration")

set(lib_enc_dic ../encode/libldac)#from aosp

set(libldac ${lib_enc_dic})
include_directories(
    ${libldac}/inc
)'

set(enc_dic ${libldac}/src)

add_library(
    ldac_enc_lib STATIC

    ${enc_dic}/bitalloc_ldac.c
    ${enc_dic}/bitalloc_sub_ldac.c
    ${enc_dic}/encode_ldac.c
    ${enc_dic}/ldacBT.c
    ${enc_dic}/ldacBT_api.c
    ${enc_dic}/ldacBT_internal.c
    ${enc_dic}/ldaclib.c
    ${enc_dic}/ldaclib_api.c
    ${enc_dic}/memory_ldac.c
    ${enc_dic}/pack_ldac.c
    ${enc_dic}/tables_ldac.c
    
    ${enc_dic}/mdct_ldac.c
    ${enc_dic}/quant_ldac.c
    ${enc_dic}/setpcm_ldac.c
    ${enc_dic}/sigana_ldac.c
    ${enc_dic}/tables_sigproc_ldac.c
)

include_directories(
    ./inc
)
file(GLOB_RECURSE SRC_DIR_LIST_dec "./src/*.c")
add_library(
    ldac_dec_lib STATIC
    ${SRC_DIR_LIST_dec}
)
add_executable(ldac_encoder ldac_enc_test.c)
add_executable(ldac_decoder ldac_dec_test.c)
add_executable(auto_test auto_test.c)


target_link_libraries(ldac_encoder ldac_enc_lib)
target_link_libraries(ldac_decoder ldac_dec_lib)

