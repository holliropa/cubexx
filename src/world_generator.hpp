#pragma once

#include <memory>
#include "chunk.hpp"

namespace cubexx {
    class WorldGenerator {
    public:
        void Generate(const std::shared_ptr<Chunk>& chunk);
    };
}
