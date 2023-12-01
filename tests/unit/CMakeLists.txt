add_subdirectory(lodepng)

add_executable(
    unit_test
    # EXCLUDE_FROM_ALL
    # algebra/algebra_test.cpp
    algebra/quadratic_test.cpp
    algebra/linear_equation_system.cpp
    algebra/cubic_test.cpp
    algebra/complex_test.cpp
    color/color_test.cpp
    concrete_block/large_cuboid_test.cpp
    concrete_block/small_cuboid_test.cpp
    optics/optics_test.cpp
    vector/vector_test.cpp
    intersection/intersection_test.cpp
    intersection/pick_closest_intersection_test.cpp)

target_link_libraries(
    unit_test
    PRIVATE GTest::gtest
            GTest::gtest_main
            GTest::gmock
            pthread # pthread is needed on Unix-like systems
            Boost::math
            eigen
            Algebra
            Imager)

target_include_directories(unit_test PRIVATE ${PROJECT_SOURCE_DIR}/src)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(
        unit_test
        PRIVATE -O1 -fsanitize=undefined
        # PRIVATE -fsanitize=leak
	)

endif()

if(BUILD_TESTING)
    # Add a custom target to run the tests using 'make test'
    add_test(NAME unit_test COMMAND unit_test)
endif()
