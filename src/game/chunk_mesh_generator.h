#pragma once

#include <memory>
#include "chunk.h"

namespace cubexx {
    class ChunkMeshGenerator {
    public:
        void Generate(const std::shared_ptr<Chunk>& chunk);
    };
}
