#pragma once

#include <memory>

#include "bw/engine/base_object.h"
#include "bw/engine/gl.h"

#include "../../core/camera.h"

namespace cubexx {
    class GridObject final : public bw::engine::BaseObject {
    public:
        explicit GridObject(const std::shared_ptr<Camera>& camera);

        void init() override;
        void update(float deltaTime) override;
        void render(const bw::engine::Camera& camera) override;

    private:
        std::shared_ptr<Camera> camera_;
        glad::Program shaderProgram_;
        bool enabled_ = false;
    };
}
