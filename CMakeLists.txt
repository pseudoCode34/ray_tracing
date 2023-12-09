cmake_minimum_required(VERSION 3.24)

project(
    Ray_tracing
    LANGUAGES CXX
    VERSION 0.1
    DESCRIPTION "A C++ project to draw graphical images")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

# Let's ensure -std=c++xx instead of -std=g++xx
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

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
# set(FETCHCONTENT_TRY_FIND_PACKAGE_MODE ALWAYS)

include(cmake/FindFmt.cmake)
include(cmake/FindLodePNG.cmake)
include(cmake/FindSpdlog.cmake)
include(cmake/FindRange-v3.cmake)
include(cmake/FindGoogleTest.cmake)
include(cmake/FindClangFormat.cmake)
include(cmake/FindBoostMath.cmake)
include(cmake/FindEigen.cmake)

if(PROJECT_IS_TOP_LEVEL)
    enable_testing()
    # Testing only available if this is the main app.
    add_subdirectory(tests)
endif()

add_subdirectory(src)
add_subdirectory(app)
add_subdirectory(examples)
file(MAKE_DIRECTORY images)
