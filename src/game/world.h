#pragma once
#include <memory>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "chunk.h"

namespace cubexx {
    class World {
    public:
        std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> chunks;
        std::unordered_set<glm::ivec3> visibleChunks;
        std::deque<glm::ivec3> chunksToGenerate;
        std::deque<glm::ivec3> chunksToGenerateMesh;
        std::deque<glm::ivec3> chunksToUnload;
    };
}
