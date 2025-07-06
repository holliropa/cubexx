#pragma once

#include "bw/engine/gl.h"
#include "bw/engine/glm.h"

namespace cubexx {
    static constexpr int CHUNK_SIZE = 32;

    struct ChunkData {
        std::array<std::array<std::array<uint32_t, CHUNK_SIZE>, CHUNK_SIZE>, CHUNK_SIZE> cubes;
        bool isEmpty = true;
    };

    struct ChunkMeshData {
        glad::VertexArray vertexArray;
        glad::ArrayBuffer arrayBuffer;
        glad::ElementArrayBuffer elementArrayBuffer;
        GLsizei index_count;
    };

    struct Chunk {
        ChunkData data{};
        std::unique_ptr<ChunkMeshData> meshData;
        glm::ivec3 index{};

        // UP, DOWN, RIGHT, LEFT, FRONT, BACK
        std::array<std::shared_ptr<Chunk>, 6> neighbors{};
    };
}
