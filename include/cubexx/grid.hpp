#pragma once

#include "game_object.hpp"
#include "gl_common.hpp"

namespace cubexx {
    class Grid : public GameObject {
    public:
        void Initialize() override;

        void Update(float delta) override;

        void Render(const Camera& camera) const override;

    private:
        glad::Program shaderProgram_;
        bool enabled_ = false;
    };
}
