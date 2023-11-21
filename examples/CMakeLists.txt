# target_link_libraries(app PRIVATE modern_library fmt::fmt)

# file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/*.hpp")
set(EXAMPLE_FILES
    all_unit_tests.cpp
    bit_donut.cpp
    block.cpp
    chessboard.cpp
    cuboid.cpp
    cylinder.cpp
    dodecahedron_overlap.cpp
    kaleidoscope.cpp
    set_difference.cpp
    set_intersection.cpp
    sphere.cpp
    spheroid.cpp
    torus.cpp
)

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
target_include_directories(Example PUBLIC ${PROJECT_SOURCE_DIR}/src
                                          ${PROJECT_SOURCE_DIR}/third_party)

target_link_libraries(Example Algebra Imager lodepng fmt::fmt-header-only)
# add_dependencies(Example lodepng)
