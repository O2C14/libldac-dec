cmake_minimum_required(VERSION 3.15)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=implicit-function-declaration -fPIC")

include_directories(
    ./inc
)
set(LDAC_SRC_DIC ./src)
add_library(
    ldac_all STATIC
    ${LDAC_SRC_DIC}/ldacBT.c
    ${LDAC_SRC_DIC}/ldaclib.c
)
option(Only_compile_lib "Only compile lib" ON)
option(Used_double "Used double" OFF)


if(Used_double)
add_definitions(-DCONFIG_SCALAR_F64)
endif()

if(NOT Only_compile_lib)

add_executable(ldac_encoder ldac_enc_test.c)
add_executable(ldac_decoder ldac_dec_test.c)
add_executable(auto_test auto_test.c)


target_link_libraries(ldac_encoder ldac_all)
target_link_libraries(ldac_decoder ldac_all)
endif()

