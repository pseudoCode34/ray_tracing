cmake_minimum_required(VERSION 3.20)

project(
    Ray_tracing
    LANGUAGES CXX
    VERSION 0.255
	DESCRIPTION "A C++ project to draw graphical images")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

# Let's ensure -std=c++xx instead of -std=g++xx
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_BUILD_TYPE Debug)

# find_program(IWYU_PATH NAMES include-what-you-use iwyu)
# if(NOT IWYU_PATH)
#     message(FATAL_ERROR "Could not find the program include-what-you-use")
# endif()
# set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "include-what-you-use")

# Only do these if this is the main project, and not if it is included through
# add_subdirectory
# if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
#     # Optionally set things like CMAKE_CXX_STANDARD,
#     # CMAKE_POSITION_INDEPENDENT_CODE here

#     # Let's nicely support folders in IDEs
#     set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#     # Docs only available if this is the main app
#     find_package(Doxygen)
#     if(Doxygen_FOUND)
#         add_subdirectory(docs)
#     else()
#         message(STATUS "Doxygen not found, not building docs")
#     endif()

#     find_package(fmt REQUIRED COMPONENTS)
# 	if(NOT fmt_FOUND)
# 		message(STATUS "Libfmt not found, not building docs")
#     endif()

#     find_package(spdlog REQUIRED COMPONENTS)
# 	if(NOT spdlog_FOUND)
# 		message(STATUS "Spdlog not found, not building docs")
#     endif()

#     find_package(range-v3 REQUIRED COMPONENTS)
# 	if(NOT range-v3_FOUND)
# 		message(STATUS "range-v3 not found, not building docs")
#     endif()
# endif()

include(ExternalProject)

add_subdirectory(third_party)
add_subdirectory(src)
add_subdirectory(app)
add_subdirectory(examples)

add_subdirectory(tests)
# # Testing only available if this is the main app.
# # Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
# if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
#    AND BUILD_TESTING)
# 	# enable_testing()

#     add_subdirectory(tests)
# endif()
