add_subdirectory(lodepng)
# add_subdirectory(rotation)

add_executable(
    google_unit_test EXCLUDE_FROM_ALL
    algebra/linear_equation_system.cpp
    geometric/gtest/refract.test.cpp owning_collection/construction.test.cpp)

target_link_libraries(
    google_unit_test
    PRIVATE GTest::gtest
            GTest::gtest_main
            GTest::gmock
            pthread # pthread is needed on Unix-like systems
            Imager)

target_include_directories(google_unit_test PRIVATE ${PROJECT_SOURCE_DIR}/src)

# if(CMAKE_BUILD_TYPE MATCHES "Debug") target_compile_options( google_unit_test
# PRIVATE -O1 -fsanitize=undefined # PRIVATE -fsanitize=leak )
# target_link_options(google_unit_test PRIVATE -fsanitize=undefined) endif()

add_executable(
    catch2_unit_test EXCLUDE_FROM_ALL
    camera/catch2/view_matrix.test.cpp
    geometric/catch2/reflect.test.cpp
    geometric/catch2/refract.test.cpp
    geometric/catch2/reflectance.test.cpp
    solid_object/catch2/object_list_hit.test.cpp
    solid_object/catch2/single_object_hit.test.cpp
	camera/catch2/view_matrix.test.cpp
	camera/catch2/viewport.test.cpp
    vector/vector.test.cpp)

target_link_libraries(
    catch2_unit_test
    PRIVATE Catch2::Catch2WithMain pthread # pthread is needed on Unix-like
                                           # systems
            Imager)

target_include_directories(catch2_unit_test PRIVATE ${PROJECT_SOURCE_DIR}/src)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(
        catch2_unit_test PRIVATE -fsanitize=undefined # PRIVATE
        # -fsanitize=leak
    )
    target_link_options(catch2_unit_test PRIVATE -fsanitize=undefined)
endif()

if(BUILD_TESTING)
    # Add a custom target to run the tests using 'make test'
    add_test(NAME google_unit_test COMMAND google_unit_test)
    add_test(NAME catch2_unit_test COMMAND catch2_unit_test PROPERTIES TIMEOUT
                                           10)
endif()
