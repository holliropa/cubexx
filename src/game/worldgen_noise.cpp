#include "worldgen_noise.h"
#include "stb_perlin.h"
#include <cstdint>

namespace cubexx::worldgen {
    static float seed_offset(const std::uint32_t seed, const std::uint32_t salt) {
        std::uint32_t x = seed ^ (salt * 0x9E3779B9u);
        x ^= x >> 16;
        x *= 0x7FEB352Du;
        x ^= x >> 15;
        x *= 0x846CA68Bu;
        x ^= x >> 16;

        const std::uint32_t m = (x & 0x0000FFFFu); // 0..65535
        const float t = static_cast<float>(m) / 65535.0f; // 0..1
        return (t * 8192.0f) - 4096.0f; // -4096..+4096
    }

    static float perlin2(const float x, const float z, const float ox, const float oz) {
        return stb_perlin_noise3(x + ox, z + oz, 0.0f, 0, 0, 0);
    }

    static float perlin3(const float x, const float y, const float z, const float ox, const float oy, const float oz) {
        return stb_perlin_noise3(x + ox, y + oy, z + oz, 0, 0, 0);
    }

    Noise::Noise(const std::uint32_t seed)
        : ocx_(seed_offset(seed, 1)), ocz_(seed_offset(seed, 2))
          , ohx_(seed_offset(seed, 3)), ohz_(seed_offset(seed, 4))
          , odx_(seed_offset(seed, 5)), odz_(seed_offset(seed, 6))
          , ovx_(seed_offset(seed, 7)), ovy_(seed_offset(seed, 8)), ovz_(seed_offset(seed, 9))
          , oex_(seed_offset(seed, 10)), oez_(seed_offset(seed, 11)) {}

    float Noise::continent(const float x, const float z) const { return perlin2(x, z, ocx_, ocz_); }
    float Noise::hills(const float x, const float z) const { return perlin2(x, z, ohx_, ohz_); }
    float Noise::detail(const float x, const float z) const { return perlin2(x, z, odx_, odz_); }
    float Noise::cave(const float x, const float y, const float z) const { return perlin3(x, y, z, ovx_, ovy_, ovz_); }
    float Noise::entrance(const float x, const float z) const { return perlin2(x, z, oex_, oez_); }
}
