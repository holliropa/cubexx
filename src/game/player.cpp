#include <cmath>

#include "player.h"
#include "bw/engine/input.h"

namespace cubexx {
    Player::Player(const std::shared_ptr<Camera>& camera)
        : camera_(camera) {}

    void Player::init() {
        camera_->transform.position.y = 75.0f;
        camera_->fov = 45.0f;
        camera_->m_zNear = 0.1f;
        camera_->m_zFar = 10000.0f;
    }

    void Player::update(float deltaTime) {
        if (bw::engine::Input::GetKeyPressed(glfw::KeyCode::M)) {
            speed_ += 0.25f;
        }
        if (bw::engine::Input::GetKeyPressed(glfw::KeyCode::N)) {
            speed_ -= 0.25f;
        }

        // Define movement and rotation speed
        const float movementStep = speed_ * deltaTime; // Speed for translation

        // Convert Euler angles (in degrees) to radians for trigonometric calculations
        float pitch = glm::radians(camera_->transform.rotation.x); // x-axis rotation
        float yaw = glm::radians(camera_->transform.rotation.y); // y-axis rotation

        // Calculate forward direction based on Euler angles (pitch and yaw)
        glm::vec3 forward;
        forward.x = std::cos(yaw) * std::cos(pitch);
        forward.y = std::sin(pitch);
        forward.z = std::sin(yaw) * std::cos(pitch);
        forward = glm::normalize(forward);

        // Calculate right and up directions based on forward vector
        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
        glm::vec3 up = glm::normalize(glm::cross(right, forward));

        // Initialize movement direction vector
        auto movementDirection = glm::vec3(0.0f);

        // Accumulate forward and backward movement
        if (bw::engine::Input::GetKeyPressed(glfw::KeyCode::W)) {
            movementDirection += forward;
        }
        if (bw::engine::Input::GetKeyPressed(glfw::KeyCode::S)) {
            movementDirection -= forward;
        }

        // Accumulate left and right movement
        if (bw::engine::Input::GetKeyPressed(glfw::KeyCode::A)) {
            movementDirection -= right;
        }
        if (bw::engine::Input::GetKeyPressed(glfw::KeyCode::D)) {
            movementDirection += right;
        }

        // Accumulate up and down movement
        if (bw::engine::Input::GetKeyPressed(glfw::KeyCode::Q)) {
            movementDirection += up;
        }
        if (bw::engine::Input::GetKeyPressed(glfw::KeyCode::E)) {
            movementDirection -= up;
        }

        // Apply combined movement if any direction is pressed
        if (glm::length(movementDirection) > 0.0f) {
            movementDirection = glm::normalize(movementDirection); // Normalize combined direction
            camera_->transform.position += movementDirection * movementStep;
        }

        auto cursorOffset = bw::engine::Input::GetCursorOffset();
        float sensitivity = 0.1f;

        // Adjust Euler angles for yaw (y-axis) and pitch (x-axis) based on cursor offset
        camera_->transform.rotation.y += cursorOffset.x * sensitivity; // Rotate around y-axis (yaw)
        camera_->transform.rotation.x += cursorOffset.y * -sensitivity; // Rotate around x-axis (pitch)

        // Clamp pitch (x rotation) to avoid flipping, in degrees
        if (camera_->transform.rotation.x > 89.0f) camera_->transform.rotation.x = 89.0f;
        if (camera_->transform.rotation.x < -89.0f) camera_->transform.rotation.x = -89.0f;
    }
}
