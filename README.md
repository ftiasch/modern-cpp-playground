# C++20 Playground

## Features

- Use `conan` to manage dependencies
- Use `Catch2` for testing
- [An introduction to multithreading in C++20, Anthony Williams](https://meetingcpp.com/mcpp/slides/2022/introduction_to_multithreading_cpp204850.pdf)
  - `std::sort(std::execution::par, ...)` (since C++ 17)
  - `std::jthread` (since C++ 20)
  - `std::latch` (since C++ 20)
  - `std::async`, `std::promise`, `std::future`, `std::packaged_tasks` (since C++ 11)
- [Modern C++ Programming Cookbook](https://github.com/PacktPublishing/Modern-Cpp-Programming-Cookbook)
  - Chapter 1

## Build

```shell
conan install . -if Build -of Build --build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -B Build .
cmake --build Build
ctest --test-dir Build
```
