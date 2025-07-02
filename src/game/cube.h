#pragma once

#include "../core/transform.h"
#include "bw/engine/game_object.h"
#include "bw/engine/gl.h"

namespace cubexx {
    class Cube final : public bw::engine::GameObject {
    public:
        Transform transform;

        Cube();

        void update(float deltaTime) override;
        void render(const bw::engine::Camera& camera) override;

    private:
        glad::Program shaderProgram_;
        glad::VertexArray vertexArray_;
        glad::ArrayBuffer arrayBuffer_;
        glad::ElementArrayBuffer elementArrayBuffer_;
    };
}
