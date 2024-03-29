#include <catch2/catch_all.hpp>
#include <catch2/generators/catch_generators_all.hpp>

#include <any>
#include <concepts>
#include <optional>
#include <string>
#include <type_traits>
#include <variant>

TEST_CASE("std::any") {
  std::any some = 233, nothing;
  REQUIRE_THROWS_AS(std::any_cast<double>(some), std::bad_any_cast);
  REQUIRE(std::any_cast<int>(some) == 233);
  REQUIRE(some.type() == typeid(int));
  REQUIRE(some.has_value());
  REQUIRE_FALSE(nothing.has_value());
  some.reset();
  REQUIRE_FALSE(some.has_value());
}

TEST_CASE("std::optional") {
  std::optional<int> some = 233, nothing;
  REQUIRE(some.has_value());
  REQUIRE_FALSE(nothing.has_value());
  REQUIRE(*some == 233);
  REQUIRE(some.value_or(42) == 233);
  REQUIRE(nothing.value_or(42) == 42);
  some.reset();
  REQUIRE_FALSE(some.has_value());
}

struct Cat {
  int id;
};

struct Dog {
  int id;
};

struct Gadget {};

template <typename T>
concept has_id = requires(T p) {
  { p.id } -> std::convertible_to<int>;
};

TEST_CASE("std::variant") {
  SECTION("variant<int, bool>") {
    using V = std::variant<int, bool>;
    V v = 42;
    REQUIRE(std::variant_size_v<decltype(v)> == 2);
    REQUIRE(std::get<int>(v) == 42);
    REQUIRE(*std::get_if<int>(&v) == 42);
    REQUIRE(std::get_if<bool>(&v) == nullptr);
    REQUIRE_THROWS_AS(std::get<bool>(v), std::bad_variant_access);
    REQUIRE(v.index() == 0);
    REQUIRE(std::holds_alternative<int>(v));
    REQUIRE_FALSE(std::holds_alternative<bool>(v));
    REQUIRE(v == V{42});
    REQUIRE_FALSE(v == V{true});
    v = true;
    REQUIRE(std::get<bool>(v));
    REQUIRE(v.index() == 1);
    REQUIRE_FALSE(std::holds_alternative<int>(v));
    REQUIRE(std::holds_alternative<bool>(v));
    REQUIRE(v == V{true});
    REQUIRE_FALSE(v == V{1});
  }

  SECTION("variant<Cat, Dog>") {
    using V = std::variant<Cat, Dog>;
    REQUIRE(std::visit([](auto &&args) -> int { return args.id; },
                       V{Cat{.id = 233}}) == 233);
    auto tf_id = [](V v) -> int {
      return std::visit(
          [](auto &&args) -> int {
            using T = std::decay_t<decltype(args)>;
            if constexpr (std::is_same_v<T, Cat>) {
              return args.id;
            }
            return args.id + 1;
          },
          v);
    };
    REQUIRE(tf_id(V{Cat{.id = 233}}) == 233);
    REQUIRE(tf_id(V{Dog{.id = 233}}) == 234);
  }

  /*
   * https://blog.nickelp.ro/posts/p2162/, Figure 2
   */
  SECTION("variant<Cat, Dog, Gadget>") {
    using V = std::variant<Cat, Dog, Gadget>;
    auto get_id = [](V v) -> int {
      return std::visit(
          [](auto &&args) -> int {
            if constexpr (has_id<decltype(args)>) {
              return args.id;
            }
            return -1;
          },
          v);
    };
    REQUIRE(get_id(V{Cat{.id = 233}}) == 233);
    REQUIRE(get_id(V{Dog{.id = 233}}) == 233);
    REQUIRE(get_id(V{Gadget{}}) == -1);
  }
}
