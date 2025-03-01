#pragma once

#include <memory>
#include "chunk.hpp"

namespace cubexx {
    class ChunkMeshGenerator {
    public:
        void Generate(const std::shared_ptr<Chunk>& chunk);
    };
}
