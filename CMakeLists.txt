cmake_minimum_required(VERSION 3.14)

project(cpp20-playground LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

find_package(Catch2 3 REQUIRED)

add_executable(tests tests.cc)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain tbb)

include(CTest)
include(Catch)
catch_discover_tests(tests)
