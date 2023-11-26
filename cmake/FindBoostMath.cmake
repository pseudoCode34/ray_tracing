FetchContent_Declare(
    boost_math_tools
    GIT_REPOSITORY https://github.com/boostorg/math.git
    GIT_TAG boost-1.83.0
    FIND_PACKAGE_ARGS)

FetchContent_MakeAvailable(boost_math_tools)
