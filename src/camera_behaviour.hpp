#pragma once

#include <memory>
#include <iostream>
#include "camera.hpp"
#include "object.hpp"
#include "input.hpp"

class CameraBehaviour : public Object {
public:
    std::shared_ptr<Camera> Camera;
    float m_speed = 2.5f;

    void Initialize() override {
        Camera->transform.position.x = -3.0f;
        Camera->fov = 45.0f;
        Camera->m_zNear = 0.1f;
        Camera->m_zFar = 100.0f;
    }

    void Update(float deltaTime) override {
        if(Input::GetKeyPressed(glfw::KeyCode::M)){
            m_speed += 0.25f;
        }
        if(Input::GetKeyPressed(glfw::KeyCode::N)){
            m_speed -= 0.25f;
        }

        // Define movement and rotation speed
        const float movementStep = m_speed * deltaTime;  // Speed for translation

        // Convert Euler angles (in degrees) to radians for trigonometric calculations
        float pitch = glm::radians(Camera->transform.rotation.x);  // x-axis rotation
        float yaw = glm::radians(Camera->transform.rotation.y);    // y-axis rotation

        // Calculate forward direction based on Euler angles (pitch and yaw)
        glm::vec3 forward;
        forward.x = cos(yaw) * cos(pitch);
        forward.y = sin(pitch);
        forward.z = sin(yaw) * cos(pitch);
        forward = glm::normalize(forward);

        // Calculate right and up directions based on forward vector
        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
        glm::vec3 up = glm::normalize(glm::cross(right, forward));

        // Initialize movement direction vector
        auto movementDirection = glm::vec3(0.0f);

        // Accumulate forward and backward movement
        if (Input::GetKeyPressed(glfw::KeyCode::W)) {
            movementDirection += forward;
        }
        if (Input::GetKeyPressed(glfw::KeyCode::S)) {
            movementDirection -= forward;
        }

        // Accumulate left and right movement
        if (Input::GetKeyPressed(glfw::KeyCode::A)) {
            movementDirection -= right;
        }
        if (Input::GetKeyPressed(glfw::KeyCode::D)) {
            movementDirection += right;
        }

        // Accumulate up and down movement
        if (Input::GetKeyPressed(glfw::KeyCode::Q)) {
            movementDirection += up;
        }
        if (Input::GetKeyPressed(glfw::KeyCode::E)) {
            movementDirection -= up;
        }

        // Apply combined movement if any direction is pressed
        if (glm::length(movementDirection) > 0.0f) {
            movementDirection = glm::normalize(movementDirection);  // Normalize combined direction
            Camera->transform.position += movementDirection * movementStep;
        }

        auto cursorOffset = Input::GetCursorOffset();
        float sensitivity = 0.1f;

        // Adjust Euler angles for yaw (y-axis) and pitch (x-axis) based on cursor offset
        Camera->transform.rotation.y += cursorOffset.x * sensitivity;  // Rotate around y-axis (yaw)
        Camera->transform.rotation.x += cursorOffset.y * -sensitivity;  // Rotate around x-axis (pitch)

        // Clamp pitch (x rotation) to avoid flipping, in degrees
        if (Camera->transform.rotation.x > 89.0f) Camera->transform.rotation.x = 89.0f;
        if (Camera->transform.rotation.x < -89.0f) Camera->transform.rotation.x = -89.0f;
    }
};