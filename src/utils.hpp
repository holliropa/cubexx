#pragma once

#include "bw/engine/glm.h"

namespace cubexx::utils {
    constexpr glm::vec3 RIGHT = {1.0f, 0.0f, 0.0f};
    constexpr glm::vec3 UP = {0.0f, 1.0f, 0.0f};
    constexpr glm::vec3 FRONT = {0.0f, 0.0f, 1.0f};

    enum class RotationOrder {
        XYZ = 0x0,
        XZY = 0x1,
        YXZ = 0x2,
        YZX = 0x3,
        ZXY = 0x4,
        ZYX = 0x5
    };

    glm::mat4 getMatrix(glm::vec3 position, glm::vec3 rotationEuler, glm::vec3 scale, RotationOrder order);

    glm::vec3 lookAt(glm::vec3 origin, glm::vec3 target, glm::vec3 up);

    glm::vec3 getDirection(const glm::vec3 &radianRotation);

    inline glm::vec3 getDirectionEuler(const glm::vec3 &eulerRotation) {
        return getDirection(glm::radians(eulerRotation));
    }
}