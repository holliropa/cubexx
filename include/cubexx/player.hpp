#pragma once

#include "game_object.hpp"

namespace cubexx {
    class Player : public GameObject {
    public:
        explicit Player(Camera* camera);

        void Initialize() override;

        void Update(float delta) override;

    private:
        Camera* pCamera_;
        float speed_ = 10.0f;
    };
}
