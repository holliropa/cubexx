#pragma once

#include <memory>

#include "bw/engine/base_object.h"
#include "bw/engine/gl.h"

#include "../world.h"
#include "../../core/transform.h"

namespace cubexx {
    class WorldRendererObject final : public bw::engine::BaseObject {
    public:
        explicit WorldRendererObject(const std::shared_ptr<World>& world);

        void init() override;

        void render(const bw::engine::Camera& camera) override;

    private:
        void init_shader();
        void init_texture();

        Transform transform_;
        glad::Program shaderProgram_;
        glad::Texture2D texture_;

        std::shared_ptr<World> world_;
    };
}
