#pragma once

#include "transform.h"
#include "bw/engine/camera.h"

namespace cubexx {
    class Camera final : public bw::engine::Camera {
    public:
        Transform transform;
        float fov{};
        glm::vec2 m_plane{};
        float m_zNear{};
        float m_zFar{};

        [[nodiscard]] glm::mat4 get_view() const override {
            return glm::lookAt(transform.position, transform.position + transform.getFront(), utils::UP);
        }

        [[nodiscard]] glm::mat4 get_projection() const override {
            return glm::perspective(glm::radians(fov), m_plane.x / m_plane.y, m_zNear, m_zFar);
        }
    };
}
