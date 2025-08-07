#include <cmath>

#include "player_object.h"

#include <iostream>

#include "bw/engine/input.h"

namespace cubexx {
    PlayerObject::PlayerObject(const std::shared_ptr<Camera>& camera, const std::shared_ptr<World>& world)
        : camera_(camera), world_(world) {}

    void PlayerObject::init() {
        camera_->transform.position.y = 75.0f;
        camera_->fov = 45.0f;
        camera_->m_zNear = 0.1f;
        camera_->m_zFar = 10000.0f;
    }

    void PlayerObject::update(const float deltaTime) {
        update_rotation(deltaTime);
        update_movement(deltaTime);
        update_physics(deltaTime);
        update_selected_block(deltaTime);
        update_block_selection(deltaTime);
    }

    PlayerObject::RaycastResult PlayerObject::performRaycast(const glm::vec3& origin, const glm::vec3& direction,
                                                             float maxDistance) const {
        RaycastResult result;
        result.hit = false;

        // DDA (Digital Differential Analyzer) algorithm for voxel traversal
        glm::ivec3 currentBlock = glm::floor(origin);
        glm::ivec3 step;
        glm::vec3 tDelta;
        glm::vec3 tMax;

        // Calculate step direction and initial tMax, tDelta
        for (int i = 0; i < 3; i++) {
            if (direction[i] > 0) {
                step[i] = 1;
                tMax[i] = (std::floor(origin[i]) + 1.0f - origin[i]) / direction[i];
            }
            else if (direction[i] < 0) {
                step[i] = -1;
                tMax[i] = (origin[i] - std::floor(origin[i])) / -direction[i];
            }
            else {
                step[i] = 0;
                tMax[i] = std::numeric_limits<float>::max();
            }
            tDelta[i] = std::abs(1.0f / direction[i]);
        }

        float currentDistance = 0.0f;
        glm::ivec3 faceNormal(0);

        while (currentDistance < maxDistance) {
            // Check if current block is solid
            if (world_->get_block(currentBlock) != CubeType::Air) {
                result.hit = true;
                result.blockPosition = currentBlock;
                result.faceNormal = faceNormal;
                result.distance = currentDistance;
                break;
            }

            // Find which axis to step along (the one with smallest tMax)
            if (tMax.x < tMax.y && tMax.x < tMax.z) {
                currentDistance = tMax.x;
                tMax.x += tDelta.x;
                currentBlock.x += step.x;
                faceNormal = glm::ivec3(-step.x, 0, 0); // Normal points back towards previous block
            }
            else if (tMax.y < tMax.z) {
                currentDistance = tMax.y;
                tMax.y += tDelta.y;
                currentBlock.y += step.y;
                faceNormal = glm::ivec3(0, -step.y, 0);
            }
            else {
                currentDistance = tMax.z;
                tMax.z += tDelta.z;
                currentBlock.z += step.z;
                faceNormal = glm::ivec3(0, 0, -step.z);
            }
        }

        return result;
    }

    bool PlayerObject::isPlayerIntersecting(const glm::ivec3& blockPos) const {
        // Simple collision check - ensure block isn't placed inside player
        glm::vec3 playerPos = camera_->transform.position;
        glm::vec3 blockCenter = glm::vec3(blockPos) + glm::vec3(0.5f);

        // Check if player's position is too close to the block center
        // Assuming player has a collision box of roughly 1.8 units tall and 0.6 units wide
        glm::vec3 distance = glm::abs(playerPos - blockCenter);

        return (distance.x < 0.5f && distance.z < 0.5f &&
            playerPos.y > blockCenter.y - 0.5f && playerPos.y < blockCenter.y + 1.3f);
    }

    void PlayerObject::update_rotation(const float deltaTime) {
        const auto cursorOffset = bw::engine::Input::GetCursorOffset();
        constexpr float sensitivity = 0.1;

        // Adjust Euler angles for yaw (y-axis) and pitch (x-axis) based on cursor offset
        camera_->transform.rotation.y += cursorOffset.x * sensitivity; // Rotate around y-axis (yaw)
        camera_->transform.rotation.x += cursorOffset.y * -sensitivity; // Rotate around x-axis (pitch)

        // Clamp pitch (x rotation) to avoid flipping, in degrees
        if (camera_->transform.rotation.x > 89.0f) camera_->transform.rotation.x = 89.0f;
        if (camera_->transform.rotation.x < -89.0f) camera_->transform.rotation.x = -89.0f;
    }

    void PlayerObject::update_movement(float deltaTime) {
        float pitch = glm::radians(camera_->transform.rotation.x);
        float yaw = glm::radians(camera_->transform.rotation.y);

        glm::vec3 forward;
        forward.x = std::cos(yaw) * std::cos(pitch);
        forward.y = std::sin(pitch);
        forward.z = std::sin(yaw) * std::cos(pitch);
        forward = glm::normalize(forward);
        forward.y = 0.0f;

        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));

        glm::vec3 movementDirection(0.0f);

        if (bw::engine::Input::GetKeyPressed(glfw::KeyCode::W)) {
            movementDirection += forward;
        }
        if (bw::engine::Input::GetKeyPressed(glfw::KeyCode::S)) {
            movementDirection -= forward;
        }

        if (bw::engine::Input::GetKeyPressed(glfw::KeyCode::A)) {
            movementDirection -= right;
        }
        if (bw::engine::Input::GetKeyPressed(glfw::KeyCode::D)) {
            movementDirection += right;
        }

        if (onGround_ && bw::engine::Input::GetKeyPressed(glfw::KeyCode::Space)) {
            movementDirection.y = 1.0f;
        }

        if (glm::length(movementDirection) > 0.0f) {
            movementDirection = glm::normalize(movementDirection);
            velocity_.x = movementDirection.x * speed_;
            velocity_.z = movementDirection.z * speed_;
            if (movementDirection.y > 0.0f) {
                velocity_.y = jumpStrength_;
                onGround_ = false;
            }
        }
    }

    void PlayerObject::update_physics(const float deltaTime) {
        // Apply gravity if not on ground
        if (!onGround_) {
            velocity_.y -= gravity_ * deltaTime;
        }

        glm::vec3 newPosition = camera_->transform.position + velocity_ * deltaTime;
        newPosition = resolve_collision(camera_->transform.position, newPosition);
        camera_->transform.position = newPosition;

        // Apply friction to horizontal movement
        velocity_.x *= friction_;
        velocity_.z *= friction_;
    }

    void PlayerObject::update_selected_block(float deltaTime) {
        const auto origin = camera_->transform.position;
        const auto direction = glm::normalize(camera_->transform.getFront());

        RaycastResult raycast = performRaycast(origin, direction, 10.0f);

        if (raycast.hit) {
            world_->selectedBlock = raycast.blockPosition;

            if (bw::engine::Input::GetMouseButtonDown(glfw::MouseButton::Left)) {
                world_->set_block(raycast.blockPosition, CubeType::Air);
            }
            else if (bw::engine::Input::GetMouseButtonDown(glfw::MouseButton::Right)) {
                const glm::ivec3 placePos = raycast.blockPosition + raycast.faceNormal;

                // Check if placement position is valid
                if (world_->get_block(placePos) == CubeType::Air && !isPlayerIntersecting(placePos)) {
                    world_->set_block(placePos, currentBlockType_);
                }
            }
        }
        else {
            world_->selectedBlock = std::nullopt;
        }
    }

    void PlayerObject::update_block_selection(float deltaTime) {
        if (bw::engine::Input::GetScrollOffset().y == 0.0f)
            return;

        const auto scrollOffset = bw::engine::Input::GetScrollOffset().y;
        const int current_index = static_cast<int>(currentBlockType_);
        int new_index = current_index + (scrollOffset > 0.0f ? 1 : -1);

        if (new_index < 1) {
            currentBlockType_ = CubeType::Stone;
        }
        else if (new_index > 3) {
            currentBlockType_ = CubeType::Grass;
        }
        else {
            currentBlockType_ = static_cast<CubeType>(new_index);
        }
    }

    glm::vec3 PlayerObject::resolve_collision(const glm::vec3& oldPos, const glm::vec3& newPos) {
        auto resolvedPos = newPos;
        onGround_ = false; // Reset ground state

        // Test X movement
        auto testPos = glm::vec3(newPos.x, oldPos.y, oldPos.z);
        if (check_collision(testPos)) {
            resolvedPos.x = oldPos.x;
            velocity_.x = 0;
        }

        // Test Z movement
        testPos = glm::vec3(resolvedPos.x, oldPos.y, newPos.z);
        if (check_collision(testPos)) {
            resolvedPos.z = oldPos.z;
            velocity_.z = 0;
        }

        // Test Y movement
        testPos = glm::vec3(resolvedPos.x, newPos.y, resolvedPos.z);
        if (check_collision(testPos)) {
            resolvedPos.y = oldPos.y;
            if (velocity_.y < 0) {
                onGround_ = true; // Player landed on something
            }
            velocity_.y = 0;
        }

        return resolvedPos;
    }

    bool PlayerObject::is_block_solid(const glm::ivec3& blockPos) const {
        CubeType blockType = world_->get_block(blockPos);
        return blockType != CubeType::Air;
    }

    bool PlayerObject::check_collision(const glm::vec3& position) const {
        const glm::vec3 feetPos = position - glm::vec3(0, bounds_.eyeHeight, 0);

        const float halfWidth = bounds_.width * 0.5f;

        // Check multiple points around the player's bounding box
        const std::vector checkPoints = {
            // Bottom corners
            feetPos + glm::vec3(-halfWidth, 0, -halfWidth),
            feetPos + glm::vec3(halfWidth, 0, -halfWidth),
            feetPos + glm::vec3(-halfWidth, 0, halfWidth),
            feetPos + glm::vec3(halfWidth, 0, halfWidth),
            // Top corners
            feetPos + glm::vec3(-halfWidth, bounds_.height, -halfWidth),
            feetPos + glm::vec3(halfWidth, bounds_.height, -halfWidth),
            feetPos + glm::vec3(-halfWidth, bounds_.height, halfWidth),
            feetPos + glm::vec3(halfWidth, bounds_.height, halfWidth),
            // Middle points
            feetPos + glm::vec3(0, bounds_.height * 0.5f, 0),
        };

        for (const auto& point : checkPoints) {
            glm::ivec3 blockPos = glm::floor(point);
            if (is_block_solid(blockPos)) {
                return true;
            }
        }

        return false;
    }
}
