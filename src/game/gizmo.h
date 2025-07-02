#pragma once

#include "bw/engine/game_object.h"
#include "bw/engine/gl.h"

namespace cubexx {
    class Gizmo final : public bw::engine::GameObject {
    public:
        void init() override;
        void update(float deltaTime) override;
        void render(const bw::engine::Camera& camera) override;

    private:
        bool enabled_ = false;
        glad::Program shaderProgram_;
        glad::VertexArray vertexArray_;
        glad::ArrayBuffer arrayBuffer_;
    };
}
