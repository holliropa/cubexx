#pragma once

#include "../core/mesh.h"
#include "bw/engine/gl.h"
#include "bw/engine/glm.h"
#include "cubes/cube_type.h"

namespace cubexx {
    static constexpr int CHUNK_SIZE = 32;

    struct ChunkData {
        std::array<std::array<std::array<CubeType, CHUNK_SIZE>, CHUNK_SIZE>, CHUNK_SIZE> cubes;
        bool isEmpty = true;
    };

    struct Chunk {
        ChunkData data{};
        std::unique_ptr<Mesh> mesh;
        glm::ivec3 index{};
        bool isDirty = false;

        // UP, DOWN, RIGHT, LEFT, FRONT, BACK
        std::array<std::shared_ptr<Chunk>, 6> neighbors{};
    };
}
