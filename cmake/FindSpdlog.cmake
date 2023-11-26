FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.12.0
    FIND_PACKAGE_ARGS)

FetchContent_MakeAvailable(spdlog)
