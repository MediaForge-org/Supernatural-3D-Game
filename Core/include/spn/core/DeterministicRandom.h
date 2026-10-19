#pragma once

// Platform-independent deterministic random numbers.
//
// std:: distributions and engines are not specified to give identical results across
// standard libraries, so anything that must reproduce from a seed on Fedora, Windows
// and in saves uses these types. All arithmetic is fixed-width integer arithmetic.

#include <cstdint>
#include <string_view>

namespace spn::core {

/// FNV-1a, 64 bit. Used to derive stream labels and fingerprints (not cryptographic).
[[nodiscard]] constexpr std::uint64_t Fnv1a64(std::string_view bytes) noexcept {
    std::uint64_t hash = 0xcbf29ce484222325ULL;
    for (const char ch : bytes) {
        hash ^= static_cast<unsigned char>(ch);
        hash *= 0x100000001b3ULL;
    }
    return hash;
}

/// SplitMix64 (public-domain algorithm by S. Vigna). Every seed is valid.
class SplitMix64 {
public:
    constexpr explicit SplitMix64(std::uint64_t seed) noexcept : state_(seed) {}

    constexpr std::uint64_t Next() noexcept {
        state_ += 0x9e3779b97f4a7c15ULL;
        std::uint64_t z = state_;
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
        z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
        return z ^ (z >> 31);
    }

    /// Uniform in [0, bound) without modulo bias. Returns 0 for bound 0.
    constexpr std::uint64_t NextBelow(std::uint64_t bound) noexcept {
        if (bound == 0) {
            return 0;
        }
        const std::uint64_t threshold = (0 - bound) % bound; // 2^64 mod bound
        while (true) {
            const std::uint64_t value = Next();
            if (value >= threshold) {
                return value % bound;
            }
        }
    }

    /// Uniform in [min, max]. Returns min if max < min.
    constexpr std::int32_t NextInRange(std::int32_t min, std::int32_t max) noexcept {
        if (max <= min) {
            return min;
        }
        const auto span = static_cast<std::uint64_t>(std::int64_t{max} - std::int64_t{min}) + 1;
        return static_cast<std::int32_t>(std::int64_t{min} +
                                         static_cast<std::int64_t>(NextBelow(span)));
    }

private:
    std::uint64_t state_;
};

/// Independent stream for (seed, label). Different labels give unrelated streams, so
/// adding a new consumer never changes the values other consumers draw.
[[nodiscard]] constexpr SplitMix64 MakeStream(std::uint64_t seed, std::string_view label) noexcept {
    SplitMix64 mixer(seed);
    SplitMix64 labelled(mixer.Next() ^ Fnv1a64(label));
    return SplitMix64(labelled.Next());
}

} // namespace spn::core
