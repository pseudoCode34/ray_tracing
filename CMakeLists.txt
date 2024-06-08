cmake_minimum_required(VERSION 3.24)

project(Ray_tracing)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# guard against in-source builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(
        FATAL_ERROR
            "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. "
    )
endif()

include(FetchContent)

find_package (Boost 1.83.0 COMPONENTS math_c99 CONFIG QUIET)
if(NOT Boost_math_c99_FOUND)
	message(STATUS "Fetching Boost Math from git repo.")

	FetchContent_Declare(
		boost_math_tools
		GIT_REPOSITORY https://github.com/boostorg/math.git
		GIT_TAG boost-1.83.0)

	FetchContent_MakeAvailable(boost_math_tools)
endif()

find_package(Eigen3 3.3 NO_MODULE)

find_package(mp-units CONFIG REQUIRED)
find_package(lodepng CONFIG REQUIRED)

FetchContent_Declare(
    range-v3
    GIT_REPOSITORY https://github.com/ericniebler/range-v3.git
    GIT_TAG 0.12.0
    FIND_PACKAGE_ARGS)
FetchContent_MakeAvailable(range-v3)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.12.0
    FIND_PACKAGE_ARGS)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
    FIND_PACKAGE_ARGS NAMES GTest)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0 # or a later release
    FIND_PACKAGE_ARGS NAMES catch2)
FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/llvm/lib/c++")

set(CMAKE_FIND_USE_SYSTEM_ENVIRONMENT_PATH TRUE)

if(PROJECT_IS_TOP_LEVEL)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_STANDARD 23)

    # Let's ensure -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)
    enable_testing()
    # Testing only available if this is the main app.
    add_subdirectory(tests)
endif()

add_subdirectory(src)
add_subdirectory(app)
add_subdirectory(examples)
file(MAKE_DIRECTORY images)
