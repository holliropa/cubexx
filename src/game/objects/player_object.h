#pragma once

#include <memory>

#include "../../core/camera.h"
#include "bw/engine/base_object.h"

namespace cubexx {
    class PlayerObject final : public bw::engine::BaseObject {
    public:
        explicit PlayerObject(const std::shared_ptr<Camera>& camera);

        void init() override;

        void update(float deltaTime) override;

    private:
        std::shared_ptr<Camera> camera_;
        float speed_ = 10.0f;
    };
}
