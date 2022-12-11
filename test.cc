#include <catch2/catch_all.hpp>
#include <catch2/generators/catch_generators_all.hpp>

#include <algorithm>
#include <execution>
#include <iostream>
#include <stop_token>
#include <thread>
#include <vector>

using namespace Catch::Generators;

TEST_CASE("std::sort") {
  const int N = 100000;

  auto v = GENERATE(take(1, chunk(N, random(0, N - 1))));

  BENCHMARK("par") {
    std::sort(std::execution::par, v.begin(), v.end());
    return v;
  };

  BENCHMARK("non-par") {
    std::sort(v.begin(), v.end());
    return v;
  };
}

void busy_wait(std::stop_token st) {
  std::stop_callback cb{st, [] { std::cerr << "stop_cb" << std::endl; }};
  std::cerr << "busy_wait|begin" << std::endl;
  while (!st.stop_requested()) {
    std::this_thread::yield();
  }
  std::cerr << "busy_wait|end" << std::endl;
}

TEST_CASE("std::jthread") {
  std::jthread j{busy_wait};
  std::cerr << "main|begin" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cerr << "main|end" << std::endl;
}
