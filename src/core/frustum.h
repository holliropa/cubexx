#pragma once
#include "bw/engine/glm.h"

namespace cubexx {
    struct Plane {
        glm::vec3 normal;
        float distance;

        Plane() = default;

        explicit Plane(const glm::vec4& p) : normal(p), distance(p.w) {
            const float length = glm::length(normal);
            normal /= length;
            distance /= length;
        }

        bool isOnForwardPlane(const glm::vec3& center, const glm::vec3& size) const {
            const float r = glm::dot(size, glm::abs(normal));
            return glm::dot(normal, center) + distance >= -r;
        }
    };

    struct Frustum {
        std::array<Plane, 6> planes;

        static Frustum fromViewProjection(const glm::mat4& viewProjection) {
            Frustum f;
            auto& p = f.planes;
            const glm::mat4 m = glm::transpose(viewProjection);

            p[0] = Plane(glm::vec4(m[3] + m[0])); // Left
            p[1] = Plane(glm::vec4(m[3] - m[0])); // Right
            p[2] = Plane(glm::vec4(m[3] + m[1])); // Bottom
            p[3] = Plane(glm::vec4(m[3] - m[1])); // Top
            p[4] = Plane(glm::vec4(m[3] + m[2])); // Near
            p[5] = Plane(glm::vec4(m[3] - m[2])); // Far
            return f;
        }

        bool isBoxVisible(const glm::vec3& min, const glm::vec3& max) const {
            const glm::vec3 center = (max + min) * 0.5f;
            const glm::vec3 size = (max - min) * 0.5f;
            for (const auto& plane : planes) {
                if (!plane.isOnForwardPlane(center, size)) return false;
            }
            return true;
        }
    };
}
