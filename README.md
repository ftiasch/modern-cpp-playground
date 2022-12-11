# C++20 Playground

## Features

- Use `conan` to manage dependencies
- Use `Catch2` for testing
- Modern C++ multithreading: `execution::par` (17), `jthread` (20), `latch` (20), `async` (11), `promise` (11), `packaged_tasks` (11)

## Build

```shell
conan install . -if Build -of Build --build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -B Build .
cmake --build Build
ctest --test-dir Build
```

## Reference

[CMake Cookbook - Catch2 CMake example](https://github.com/dev-cafe/cmake-cookbook/blob/master/chapter-04/recipe-02/cxx-example/CMakeLists.txt)

[An introduction to multithreading in C++20, Anthony Williams](https://meetingcpp.com/mcpp/slides/2022/introduction_to_multithreading_cpp204850.pdf)
