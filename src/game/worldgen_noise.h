#pragma once
#include <cstdint>

namespace cubexx::worldgen {
    class Noise {
    public:
        explicit Noise(std::uint32_t seed);

        [[nodiscard]] float continent(float x, float z) const; // expects already scaled coords
        [[nodiscard]] float hills(float x, float z) const;     // expects already scaled coords
        [[nodiscard]] float detail(float x, float z) const;    // expects already scaled coords
        [[nodiscard]] float cave(float x, float y, float z) const; // expects already scaled coords
        [[nodiscard]] float entrance(float x, float z) const;

    private:
        float ocx_, ocz_;
        float ohx_, ohz_;
        float odx_, odz_;
        float ovx_, ovy_, ovz_;
        float oex_, oez_;
    };
}
