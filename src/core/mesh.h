#pragma once

#include "bw/engine/gl.h"

namespace cubexx {
    struct Mesh {
        glad::VertexArray vao;
        glad::ArrayBuffer vbo;
        glad::ElementArrayBuffer ebo;
        GLsizei index_count{};

        Mesh() = default;
        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;

        Mesh(Mesh&&) = delete;
        Mesh& operator=(Mesh&&) = delete;
    };
}
