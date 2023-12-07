# CMake somehow can't find Boost Math component, but can find filesystem
# component using find_package(Boost 1.53.0 QUIET COMPONENTS Math). So, here
# find the entire Boost lib
find_package(Boost 1.53.0 )

# if(NOT Boost_FOUND) message(STATUS "Fetching Boost Math from git repo.")

# FetchContent_Declare( boost_math_tools GIT_REPOSITORY
# https://github.com/boostorg/math.git GIT_TAG boost-1.83.0)

# FetchContent_MakeAvailable(boost_math_tools) endif()
