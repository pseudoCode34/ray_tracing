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

find_package (Boost 1.83.0 COMPONENTS math_c99 CONFIG REQUIRED)
find_package(mp-units CONFIG REQUIRED)
find_package(eigen CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(catch2 CONFIG REQUIRED)
find_package(gtest CONFIG REQUIRED)
find_package(lodepng CONFIG REQUIRED)

set(CMAKE_FIND_USE_SYSTEM_ENVIRONMENT_PATH TRUE)

if(PROJECT_IS_TOP_LEVEL)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    enable_testing()
    # Testing only available if this is the main app.
    add_subdirectory(tests)
endif()

add_subdirectory(src)
add_subdirectory(app)
add_subdirectory(examples)
file(MAKE_DIRECTORY images)
