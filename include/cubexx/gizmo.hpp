#pragma once

#include "game_object.hpp"
#include "gl_common.hpp"

namespace cubexx {
    class Gizmo : public GameObject {
    public:
        void Initialize() override;

        void Update(float delta) override;

        void Render(const Camera &camera) const override;

    private:
        bool enabled_ = false;
        glad::Program shaderProgram_;
        glad::VertexArray vertexArray_;
        glad::ArrayBuffer arrayBuffer_;
    };
}