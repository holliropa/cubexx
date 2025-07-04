#pragma once

#include "bw/engine/base_object.h"
#include "bw/engine/gl.h"

namespace cubexx {
    class GizmoObject final : public bw::engine::BaseObject {
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
