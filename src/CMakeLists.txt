add_library(
    Imager
    algebra.cpp
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
    sphere.cpp
    spheroid.cpp
    thinring.cpp
    torus.cpp
    triangle.cpp
    vector.cpp)

# if(CMAKE_BUILD_TYPE MATCHES "Debug") target_compile_options( Imager BEFORE
# PRIVATE -O1 -fsanitize=undefined PRIVATE -fsanitize=leak) endif()

if(Boost_FOUND)
	target_include_directories(Imager PUBLIC ${Boost_INCLUDE_DIRS})
	target_link_libraries(Imager PUBLIC Boost::boost)
elseif()
	target_link_libraries(Imager PUBLIC Boost::math)
endif()

target_compile_options(Imager PUBLIC -Wall -Wextra -pedantic)
target_link_libraries(Imager PUBLIC lodepng fmt::fmt-header-only
                                    range-v3::range-v3 Eigen3::Eigen)
# target_compile_definitions(Imager PRIVATE RAYTRACE_DEBUG_POINTS=1)
