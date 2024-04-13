cmake_minimum_required(VERSION 3.15)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=implicit-function-declaration")
file(GLOB_RECURSE SRC_DIR_LIST_dec "./src/*.c")
add_library(
    ldac_dec STATIC
    ${SRC_DIR_LIST_dec}
)