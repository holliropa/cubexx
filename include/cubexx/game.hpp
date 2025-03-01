#pragma once

#include <memory>
#include "gl_common.hpp"
#include "application.hpp"
#include "engine.hpp"
#include "game_object.hpp"
#include "camera.hpp"

namespace cubexx {
    class Game {
    public:
        Game(Application& application, Engine& engine);

        ~Game();

        void run();

    private:
        Application& application_;
        Engine& engine_;
        glfw::Window* pWindow_;
        std::vector<std::unique_ptr<GameObject>> pObjects_;
        std::unique_ptr<Camera> pCamera_;
    };
}
