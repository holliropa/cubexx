#pragma once

#include "glm_common.hpp"
#include "utils.hpp"

namespace cubexx {
    struct Transform {
        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 rotation = glm::vec3(0.0f);
        glm::vec3 scale = glm::vec3(1.0f);

        Transform() = default;

        Transform(const glm::vec3 position, const glm::vec3 rotation, const glm::vec3 scale)
            : position(position), rotation(rotation), scale(scale) {}


        [[nodiscard]] glm::mat4 getMatrix() const {
            return utils::getMatrix(position, rotation, scale, utils::RotationOrder::ZYX);
        }

        [[nodiscard]] glm::vec3 getFront() const {
            return utils::getDirectionEuler(rotation);
        }

        [[nodiscard]] glm::vec3 getRight() const {
            return glm::cross(getFront(), utils::UP);
        }

        [[nodiscard]] glm::vec3 getUp() const {
            return glm::cross(getRight(), getFront());
        }
    };
}
