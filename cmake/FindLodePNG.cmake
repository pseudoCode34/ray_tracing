FetchContent_Declare(
    lodepng
    GIT_REPOSITORY https://github.com/lvandeve/lodepng.git
    GIT_TAG c18b949b71f45e78b1f9a28c5d458bce0da505d6
    FIND_PACKAGE_ARGS)

FetchContent_MakeAvailable(lodepng)

add_library(lodepng ${lodepng_SOURCE_DIR}/lodepng.cpp)
include_directories(${lodepng_SOURCE_DIR})
