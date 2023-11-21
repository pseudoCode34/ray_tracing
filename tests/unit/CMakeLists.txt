add_subdirectory(lodepng)

enable_testing()

# Add a target for unit tests
add_executable(
    unit_test
    # EXCLUDE_FROM_ALL
    # algebra/algebra_test.cpp
    algebra/quadratic_test.cpp
    algebra/cubic_test.cpp
    algebra/complex_test.cpp
    color/color_test.cpp
    optics/optics_test.cpp
    vector/vector_test.cpp
    intersection/intersection_test.cpp
    intersection/pick_closest_intersection_test.cpp)

# Link the unit_test target with GoogleTest
target_link_libraries(
    unit_test gtest gtest_main GTest::gmock pthread # pthread is needed on
                                                    # Unix-like systems
    Imager)

target_include_directories(unit_test PRIVATE ${PROJECT_SOURCE_DIR}/src)
if(BUILD_TESTING)
    # Add a custom target to run the tests using 'make test'
    add_test(NAME unit_test COMMAND unit_test)
endif()
