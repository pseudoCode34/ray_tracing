FetchContent_Declare(
    eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG 3.4.0
    FIND_PACKAGE_ARGS)

FetchContent_MakeAvailable(eigen)
