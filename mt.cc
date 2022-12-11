#include <catch2/catch_all.hpp>
#include <catch2/generators/catch_generators_all.hpp>

#include <algorithm>
#include <execution>
#include <future>
#include <iostream>
#include <latch>
#include <stop_token>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

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
  std::this_thread::sleep_for(1s);
  std::cerr << "main|end" << std::endl;
}

TEST_CASE("std::latch") {
  const int THREADS = 2;
  std::latch done(THREADS);
  std::vector<std::jthread> threads;
  for (int i = 0; i < THREADS; ++i) {
    threads.emplace_back(std::jthread([i, &done]() {
      std::this_thread::sleep_for(std::chrono::seconds(i + 1));
      done.count_down();
    }));
  }
  REQUIRE(!done.try_wait());
  done.wait();
}

TEST_CASE("std::async") {
  auto f = std::async([]() -> int {
    std::this_thread::sleep_for(1s);
    return 233;
  });
  REQUIRE(f.wait_for(0s) != std::future_status::ready);
  auto got = f.get();
  REQUIRE(got == 233);
}

TEST_CASE("std::promise") {
  std::promise<int> prom;
  auto f = prom.get_future();
  std::cerr << "main|begin" << std::endl;
  /*
   * We should give std::jthread a name
   * to prevent it from deconstruction
   */
  std::jthread j([&prom](std::stop_token) -> void {
    std::this_thread::sleep_for(1s);
    prom.set_value(233);
  });
  REQUIRE(f.wait_for(0s) != std::future_status::ready);
  auto got = f.get();
  REQUIRE(got == 233);
}

TEST_CASE("std::packaged_task") {
  std::packaged_task<int(int)> work([](int i) {
    std::this_thread::sleep_for(1s);
    return i;
  });
  auto f = work.get_future();
  /*
   * std::packaged_task should be involed in another thread
   * Remember to std::move(work)
   */
  std::jthread j{std::bind(std::move(work), 233)};
  REQUIRE(f.wait_for(0s) != std::future_status::ready);
  auto got = f.get();
  REQUIRE(got == 233);
}
