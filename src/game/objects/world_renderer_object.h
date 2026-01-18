#pragma once

#include <memory>

#include "bw/engine/base_object.h"
#include "bw/engine/gl.h"

#include "../world.h"
#include "../../core/camera.h"
#include "../../core/transform.h"
#include "../cubes/texture_manager.h"

namespace cubexx {
    class WorldRendererObject final : public bw::engine::BaseObject {
    public:
        explicit WorldRendererObject(const std::shared_ptr<World>& world, const std::shared_ptr<TextureManager>& texture_manager, const std::shared_ptr<Camera>& camera);

        void init() override;

        void update(float deltaTime) override;

        void render(const bw::engine::Camera& camera) override;

    private:
        void init_shader();

        bool isPolyMode = false;

        Transform transform_;
        glad::Program shaderProgram_;

        std::shared_ptr<World> world_;
        std::shared_ptr<TextureManager> texture_manager_;
        std::shared_ptr<Camera> camera_;
    };
}
