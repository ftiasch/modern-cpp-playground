#include <catch2/catch_all.hpp>

namespace CRTP {

template <typename T>
concept BaseLike = requires(T t) {
                     { t.gao() } -> std::convertible_to<int>;
                   };

struct Derived {
  int gao() { return 233; }
};

int invoke(BaseLike auto &t) { return t.gao(); }

} // namespace CRTP

TEST_CASE("CRTP") {
  using namespace CRTP;
  Derived d;
  REQUIRE(invoke(d) == 233);
}
