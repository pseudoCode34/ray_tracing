# #Note that headers are optional, and do not affect add_library, but they will
# #not show up in IDEs unless they are listed in add_library.

# #Optionally glob, but only for CMake 3.12 or later : file(GLOB HEADER_LIST
# CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/*.hpp") file(GLOB SOURCE_LIST
# CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")

# # #Make an automatic library - will be static or dynamic based on user setting
# add_library(${PROJECT_NAME} _lib ${SOURCE_LIST} ${HEADER_LIST})

# #We need this directory, and users of our library will need it too
# target_include_directories(${PROJECT_NAME} _lib PUBLIC
# "${PROJECT_SOURCE_DIR}/include")

# #TODO : NEED fmt ? This depends on(header only) fmt
# target_link_libraries(${PROJECT_NAME} _lib PRIVATE fmt::fmt - header - only)

# target_link_directories(${PROJECT_NAME} _lib PRIVATE
# "${PROJECT_SOURCE_DIR}/lib")

# #All users of this library will need at least C ++ 20
# target_compile_features(${PROJECT_NAME} _lib PUBLIC cxx_std_20)

# #IDEs should put the headers in a nice place source_group(TREE #
# "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST })

add_library(Algebra algebra.cpp)
# state that anybody linking to us needs to include the current source dir to
# find MathFunctions.h, while we don't. target_include_directories(Algebra
# PUBLIC ${PROJECT_SOURCE_DIR } / include)

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
    light_source.cpp
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

target_include_directories(Imager PUBLIC ${PROJECT_SOURCE_DIR}/third_party)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    # target_compile_options(
    #     Imager BEFORE
    #     PUBLIC -O1 -fsanitize=undefined
    #     PUBLIC -fsanitize=address)
endif()

target_link_libraries(Imager PUBLIC lodepng Algebra fmt::fmt-header-only)
# target_compile_definitions(Imager PRIVATE RAYTRACE_DEBUG_POINTS=1)
# add_dependencies(Imager lodepng)
