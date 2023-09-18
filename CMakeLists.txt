cmake_minimum_required(VERSION 3.16)
project(
    cpp-snippets
    LANGUAGES C
              CXX
)
set(CMAKE_CXX_STANDARD 23)

enable_testing()

find_package(
    WarningsCleanIfGcc
    REQUIRED
)
warnings_if_gcc(WEXTRA)

add_subdirectory("src")
add_subdirectory("tests")
