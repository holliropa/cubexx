#pragma once

#include <memory>
#include "chunk.h"
#include "worldgen_noise.h"
#include "worldgen_params.h"

namespace cubexx::worldgen {
    class WorldGenerator {
    public:
        WorldGenerator(std::uint32_t seed, const Params& params);

        void Generate(const std::shared_ptr<Chunk>& chunk) const;

    private:
        std::uint32_t seed_;
        Params params_;
        Noise noise_;
    };
}
