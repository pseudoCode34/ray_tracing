add_subdirectory(fmt)
add_subdirectory(range-v3)
add_subdirectory(spdlog)

add_library(lodepng STATIC lodepng/lodepng.cpp)

target_include_directories(lodepng PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
