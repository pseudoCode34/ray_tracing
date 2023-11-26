FetchContent_Declare(
    eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG dd8c71e62852b2fe429edb6682ac91fd1c578a26
    FIND_PACKAGE_ARGS)

FetchContent_MakeAvailable(eigen)
