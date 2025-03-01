#pragma once

#include "camera.hpp"

namespace cubexx {
    class GameObject {
    public:
        virtual ~GameObject() = default;

        virtual void Initialize() {}

        virtual void Update(float deltaTime) {}

        virtual void LateUpdate(float deltaTime) {}

        virtual void Render(const Camera &camera) const {}
    };
}