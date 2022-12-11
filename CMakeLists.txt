cmake_minimum_required(VERSION 3.14)

project(cpp20-playground LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

find_package(Catch2 3 REQUIRED)

add_executable(any any.cc)
target_link_libraries(any PRIVATE Catch2::Catch2WithMain)

add_executable(mt mt.cc)
target_link_libraries(mt PRIVATE Catch2::Catch2WithMain tbb)

include(CTest)
include(Catch)
catch_discover_tests(any)
catch_discover_tests(mt)
