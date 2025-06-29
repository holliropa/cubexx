#pragma once

#include "bw/engine/gl.h"
#include "bw/engine/glm.h"

namespace cubexx {
    class Chunk {
    public:
        static constexpr int SIZE = 32;

        std::array<std::array<std::array<uint32_t, SIZE>, SIZE>, SIZE> cubes;
        glm::ivec3 position;
        glad::VertexArray vertexArray;
        glad::ArrayBuffer arrayBuffer;
        glad::ElementArrayBuffer elementArrayBuffer;
        GLsizei index_count;
        bool isEmpty_ = true;

        [[nodiscard]] bool isEmpty() const {
            return isEmpty_;
        }
    };
}
