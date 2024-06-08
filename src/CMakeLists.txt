add_library(
    Imager
    color.cpp
    camera.cpp
    geometric.cpp
    image_renderer.cpp
    material.cpp
    ray.cpp
    solid_object.cpp
    sphere.cpp
    vector3f.cpp
    random.cpp
    viewport.cpp)
target_include_directories(Imager PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(
    owning_collection INTERFACE owning_container.hpp
    owning_container/concepts/stl_container_concept.hpp
    owning_container/concepts/smart_pointer_concept.hpp)

# if(CMAKE_BUILD_TYPE MATCHES "Debug") target_compile_options( Imager BEFORE
# PRIVATE -O1 -fsanitize=undefined PRIVATE -fsanitize=leak) endif()

if(Boost_math_c99_FOUND)
    target_link_libraries(Imager PUBLIC Boost::math_c99)
elseif()
    target_include_directories(Imager PUBLIC ${Boost_INCLUDE_DIRS})
    target_link_libraries(Imager PUBLIC Boost::boost)
endif()

target_compile_options(Imager PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(
    Imager PUBLIC lodepng::lodepng spdlog::spdlog range-v3::range-v3
                  Eigen3::Eigen owning_collection mp-units::mp-units)

# target_compile_definitions(Imager PUBLIC gsl_CONFIG_CONTRACT_VIOLATION_THROWS)
