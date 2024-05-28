add_executable(lodepng_test encode.cpp)

target_include_directories(lodepng_test
                           PRIVATE ${PROJECT_SOURCE_DIR}/third_party)
target_compile_options(
    lodepng_test
    PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/W4
            /WX>
            $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall
            -Wextra
            -pedantic>
            -O1
            -fsanitize=undefined
            -Werror=sign-conversion)

target_link_options(lodepng_test PRIVATE -fsanitize=undefined)
target_link_libraries(
    lodepng_test PRIVATE lodepng GTest::gtest GTest::gtest_main
                         pthread # pthread is needed on Unix-like systems
						 spdlog::spdlog
)
