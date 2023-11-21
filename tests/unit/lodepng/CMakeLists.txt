add_executable(lodepng_test encode.cpp)

target_include_directories(lodepng_test
                           PRIVATE ${PROJECT_SOURCE_DIR}/third_party)
target_link_libraries(
    lodepng_test
    PRIVATE lodepng gtest gtest_main pthread # pthread is needed on Unix-like
                                             # systems
            fmt::fmt-header-only)
