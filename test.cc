#include <catch2/catch_all.hpp>
#include <catch2/generators/catch_generators_all.hpp>

#include <algorithm>
#include <execution>
#include <vector>

#include <iostream>

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
