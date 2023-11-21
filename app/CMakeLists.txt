set(SOURCE_FILES main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_compile_options(${PROJECT_NAME} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)
# Assuming this is meant to be a public directory
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/examples
           # ${PROJECT_SOURCE_DIR}/third_party
           ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} PRIVATE Example fmt::fmt-header-only
                                              Imager)
