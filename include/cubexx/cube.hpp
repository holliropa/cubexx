#pragma once

#include "gl_common.hpp"
#include "transform.hpp"
#include "game_object.hpp"

namespace cubexx {
    class Cube : public GameObject {
    public:
        Transform transform;

        Cube();

        void Update(float delta) override;

        void Render(const Camera& camera) const override;

    private:
        glad::Program shaderProgram_;
        glad::VertexArray vertexArray_;
        glad::ArrayBuffer arrayBuffer_;
        glad::ElementArrayBuffer elementArrayBuffer_;
    };
}
