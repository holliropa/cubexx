#pragma once

#include <memory>

#include "../core/camera.h"
#include "bw/engine/game_object.h"
#include "bw/engine/gl.h"

namespace cubexx {
    class Grid final : public bw::engine::GameObject {
    public:
        explicit Grid(const std::shared_ptr<Camera>& camera);

        void init() override;
        void update(float deltaTime) override;
        void render(const bw::engine::Camera& camera) override;

    private:
        std::shared_ptr<Camera> camera_;
        glad::Program shaderProgram_;
        bool enabled_ = false;
    };
}
