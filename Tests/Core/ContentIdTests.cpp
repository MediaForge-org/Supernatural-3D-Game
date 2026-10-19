#include <doctest.h>
#include <map>
#include <spn/core/ContentId.h>
#include <string>

using spn::core::ContentId;

namespace {
ContentId Make(const char* text) {
    auto id = ContentId::Parse(text);
    REQUIRE(id.has_value());
    return id.value_or(ContentId{});
}
} // namespace

TEST_CASE("ContentId: accepts namespaced dotted paths") {
    for (const char* text : {"base:hair.short_01", "mod_x:a", "a:b.c.d_e", "base:face.jaw_width"}) {
        CAPTURE(text);
        const ContentId id = Make(text);
        CHECK(id.Value() == text);
        CHECK_FALSE(id.IsEmpty());
    }
}

TEST_CASE("ContentId: rejects malformed ids") {
    for (const char* text : {"", "base", ":a", "base:", "Base:a", "base:A", "1base:a", "base:1a",
                             "base:a..b", "base:.a", "base:a.", "base:a-b", "base:a b", "base:a:b",
                             "base:a/b", "base: a", "base:a\n", "base:ä", "-"}) {
        CAPTURE(text);
        CHECK_FALSE(ContentId::Parse(text).has_value());
    }
    const std::string withNul = std::string("base:a") + '\0' + "b";
    CHECK_FALSE(ContentId::Parse(withNul).has_value());
}

TEST_CASE("ContentId: length limits") {
    using namespace spn::core;
    const std::string maxNamespace(kMaxContentNamespaceLength, 'n');
    CHECK(ContentId::Parse(maxNamespace + ":a").has_value());
    CHECK_FALSE(ContentId::Parse(maxNamespace + "n:a").has_value());

    const std::string fits = "n:" + std::string(kMaxContentIdLength - 2, 'a');
    CHECK(fits.size() == kMaxContentIdLength);
    CHECK(ContentId::Parse(fits).has_value());
    CHECK_FALSE(ContentId::Parse(fits + "a").has_value());
}

TEST_CASE("ContentId: default id is empty, namespace and ordering") {
    const ContentId none;
    CHECK(none.IsEmpty());
    CHECK(none.Namespace().empty());
    CHECK(none == ContentId{});

    const ContentId a = Make("base:a.x");
    const ContentId b = Make("base:b");
    const ContentId mod = Make("mod_x:a");
    CHECK(a.Namespace() == "base");
    CHECK(mod.Namespace() == "mod_x");
    CHECK(a < b);
    CHECK(b < mod);
    CHECK(a != b);

    const std::map<ContentId, int> sorted{{mod, 3}, {b, 2}, {a, 1}};
    int expected = 1;
    for (const auto& entry : sorted) {
        CHECK(entry.second == expected++);
    }
}
