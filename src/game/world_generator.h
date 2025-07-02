#pragma once

#include <memory>
#include "chunk.h"

namespace cubexx {
    class WorldGenerator {
    public:
        void Generate(const std::shared_ptr<Chunk>& chunk);
    };
}
