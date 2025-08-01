#pragma once

#include <memory>

#include "../../core/camera.h"
#include "bw/engine/base_object.h"
#include "../world.h"

namespace cubexx {
    class PlayerObject final : public bw::engine::BaseObject {
    public:
        PlayerObject(const std::shared_ptr<Camera>& camera, const std::shared_ptr<World>& world);

        void init() override;
        void update(float deltaTime) override;

    private:
        struct RaycastResult {
            bool hit = false;
            glm::ivec3 blockPosition;
            glm::ivec3 faceNormal;
            float distance;
        };

        std::shared_ptr<Camera> camera_;
        std::shared_ptr<World> world_;
        float speed_ = 10.0f;

        // Block placement related
        CubeType currentBlockType_ = CubeType::Stone; // Default block to place

        RaycastResult performRaycast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance) const;
        bool isPlayerIntersecting(const glm::ivec3& blockPos) const;
    };

}
