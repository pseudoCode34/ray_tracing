add_library(Algebra algebra.cpp)

add_library(
    Imager
    binary.cpp
    block.cpp
    chessboard.cpp
    color.cpp
    cuboid.cpp
    cylinder.cpp
    dodecahedron.cpp
    icosahedron.cpp
    imager.cpp
    imager_buffer.cpp
    intersection.cpp
    optics.cpp
    planet.cpp
    reorient.cpp
    scene.cpp
    setcompl.cpp
    setdiff.cpp
    setisect.cpp
    setunion.cpp
    solid.cpp
    solid_object_reorientable.cpp
    sphere.cpp
    spheroid.cpp
    thinring.cpp
    torus.cpp
    triangle.cpp
    vector.cpp)

# if(CMAKE_BUILD_TYPE MATCHES "Debug")
#     target_compile_options( Imager BEFORE PRIVATE -O1 -fsanitize=undefined
#     PRIVATE -fsanitize=leak)
# endif()

target_link_libraries(Algebra PUBLIC Eigen3::Eigen)
target_link_libraries(Imager PUBLIC Algebra lodepng fmt::fmt-header-only
                                     range-v3::range-v3)
# target_compile_definitions(Imager PRIVATE RAYTRACE_DEBUG_POINTS=1)
