#pragma once

#include <memory>

#include "camera.h"
#include "bw/engine/game_object.h"


namespace cubexx {
    class Player final : public bw::engine::GameObject {
    public:
        explicit Player(const std::shared_ptr<Camera>& camera);

        void init() override;

        void update(float deltaTime) override;

    private:
        std::shared_ptr<Camera> camera_;
        float speed_ = 10.0f;
    };
}
