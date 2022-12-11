#include <catch2/catch_all.hpp>
#include <catch2/generators/catch_generators_all.hpp>

#include <algorithm>
#include <execution>
#include <iostream>
#include <latch>
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

TEST_CASE("std::jthread") {
  std::jthread j{[](std::stop_token st) {
    std::stop_callback cb{st, [] { std::cerr << "stop_cb" << std::endl; }};
    std::cerr << "busy_wait|begin" << std::endl;
    while (!st.stop_requested()) {
      std::this_thread::yield();
    }
    std::cerr << "busy_wait|end" << std::endl;
  }};
  std::cerr << "main|begin" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cerr << "main|end" << std::endl;
}

TEST_CASE("std::latch") {
  const int THREADS = 2;
  std::latch done(THREADS);
  std::cerr << "main|begin" << std::endl;
  std::vector<std::jthread> threads;
  for (int i = 0; i < THREADS; ++i) {
    threads.emplace_back(std::jthread([i, &done]() {
      std::cerr << "thread" << i << "|begin" << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(i + 1));
      std::cerr << "thread" << i << "|wake" << std::endl;
      done.count_down();
      std::cerr << "thread" << i << "|end" << std::endl;
    }));
  }
  std::cerr << "main|wait" << std::endl;
  done.wait();
  std::cerr << "main|end" << std::endl;
}
