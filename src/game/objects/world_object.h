#pragma once

#include "bw/engine/base_object.h"

#include "../../core/camera.h"
#include "../world_generator.h"
#include "../config.h"

namespace cubexx {
    class World;

    class WorldObject final : public bw::engine::BaseObject {
    public:
        explicit WorldObject(const std::shared_ptr<Config>& config,
                             const std::shared_ptr<Camera>& camera,
                             const std::shared_ptr<World>& world);

        void init() override;

        void update(float deltaTime) override;

    private:
        std::shared_ptr<Config> config_;
        std::shared_ptr<Camera> camera_;
        std::shared_ptr<World> world_;

        bool first_ = true;

        glm::ivec3 userChunkIndex_ = glm::zero<glm::ivec3>();
        bool isChunkIndexChanged_ = true;
    };
}
