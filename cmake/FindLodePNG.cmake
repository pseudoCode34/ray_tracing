FetchContent_Declare(
    lodepng
    GIT_REPOSITORY https://github.com/lvandeve/lodepng.git
    GIT_TAG c18b949b71f45e78b1f9a28c5d458bce0da505d6
    FIND_PACKAGE_ARGS)

FetchContent_MakeAvailable(lodepng)
include_directories(${lodepng_SOURCE_DIR})
add_library(lodepng STATIC ${lodepng_SOURCE_DIR}/lodepng.cpp)
