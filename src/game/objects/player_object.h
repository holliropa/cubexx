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

        struct PlayerBounds {
            float width = 0.6f;
            float height = 1.8f;
            float eyeHeight = 1.6f;
        };

        std::shared_ptr<Camera> camera_;
        std::shared_ptr<World> world_;
        float speed_ = 4.0f;
        glm::vec3 velocity_ = glm::zero<glm::vec3>();
        bool onGround_ = false;
        PlayerBounds bounds_;

        float gravity_ = 20.0f;
        float jumpStrength_ = 8.0f;
        float friction_ = 0.9f;

        // Block placement related
        CubeType currentBlockType_ = CubeType::Stone; // Default block to place

        enum class GameMode {
            Survival,
            Creative
        };
        GameMode gameMode_ = GameMode::Survival;

        [[nodiscard]] RaycastResult performRaycast(const glm::vec3& origin, const glm::vec3& direction,
                                                   float maxDistance) const;
        [[nodiscard]] bool isPlayerIntersecting(const glm::ivec3& blockPos) const;

        void update_rotation(float deltaTime);
        void update_movement(float deltaTime);
        void update_physics(float deltaTime);
        void update_selected_block(float deltaTime);
        void update_block_selection(float deltaTime);

        glm::vec3 resolve_collision(const glm::vec3& oldPos, const glm::vec3& newPos);
        bool is_block_solid(const glm::ivec3& blockPos) const;
        bool check_collision(const glm::vec3& position) const;
    };
}
