#pragma once

#include "camera.h"
#include "bw/engine/application.h"
#include "bw/engine/engine.h"
#include "bw/engine/game.h"

namespace cubexx {
    class Game : public bw::engine::Game {
    public:
        Game(bw::engine::Application& application, bw::engine::Engine& engine);

        ~Game();

    private:
        std::shared_ptr<Camera> main_camera_;
        size_t frame_buffer_event_handle;
    };
}
