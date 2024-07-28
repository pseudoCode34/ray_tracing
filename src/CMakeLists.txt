add_library(
    ray_tracing
    spatial/camera.cpp
    graphics/image.cpp
    spatial/image_renderer.cpp
    spatial/material.cpp
    spatial/transformable/solid_object.cpp
    spatial/shapes/sphere.cpp
    spatial/viewport.cpp)
target_include_directories(ray_tracing PUBLIC ${PROJECT_SOURCE_DIR}/src)

add_library(
    owning_collection INTERFACE owning_container.hpp
    owning_container/concepts/stl_container_concept.hpp
    owning_container/concepts/smart_pointer_concept.hpp)

# if(CMAKE_BUILD_TYPE MATCHES "Debug") target_compile_options( ray_tracing
# BEFORE PRIVATE -O1 -fsanitize=undefined PRIVATE -fsanitize=leak) endif()

if(Boost_math_c99_FOUND)
    target_link_libraries(ray_tracing PUBLIC Boost::math_c99)
elseif()
    target_include_directories(ray_tracing PUBLIC ${Boost_INCLUDE_DIRS})
    target_link_libraries(ray_tracing PUBLIC Boost::boost)
endif()

target_compile_options(ray_tracing PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(
    ray_tracing PUBLIC lodepng::lodepng spdlog::spdlog range-v3::range-v3
                       Eigen3::Eigen owning_collection mp-units::mp-units)

# target_compile_definitions(ray_tracing PUBLIC
# gsl_CONFIG_CONTRACT_VIOLATION_THROWS)
