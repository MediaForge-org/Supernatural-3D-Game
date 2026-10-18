#include <doctest.h>
#include <spn/core/Result.h>
#include <string>

namespace {
using IntOrText = spn::core::Result<int, std::string>;
}

TEST_CASE("Result: holds a value") {
    IntOrText result{42};
    REQUIRE(result.HasValue());
    CHECK(static_cast<bool>(result));
    CHECK(result.Value() == 42);
}

TEST_CASE("Result: holds an error") {
    const IntOrText result{std::string("boom")};
    REQUIRE_FALSE(result.HasValue());
    CHECK_FALSE(static_cast<bool>(result));
    CHECK(result.Error() == "boom");
}

TEST_CASE("Result: value can be moved out") {
    spn::core::Result<std::string, int> result{std::string("payload")};
    const std::string moved = std::move(result).Value();
    CHECK(moved == "payload");
}
