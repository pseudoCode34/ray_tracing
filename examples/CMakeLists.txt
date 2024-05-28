set(EXAMPLE_FILES
	draw_sphere.cpp)

add_library(Example ${EXAMPLE_FILES})

target_compile_options(
    Example
    PRIVATE $<$<CXX_COMPILER_ID:MSVC>:
            /W4
            /WX>
            $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:
            -Wall
            -Wextra
            -Wpedantic>)
# Assuming this is meant to be a public directory
target_include_directories(Example PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(Example lodepng Imager )
