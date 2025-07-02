#include "game.h"

#include "game/player.h"
#include "game/world.h"
#include "game/gizmo.h"

namespace cubexx {
    Game::Game(bw::engine::Application& application, bw::engine::Engine& engine)
        : bw::engine::Game(application, engine) {
        frame_buffer_event_handle = window_.framebufferSizeEvent.set(
            [this](const int width, const int height) {
                main_camera_->m_plane = {width, height};
            });

        window_.setCursorMode(glfw::CursorMode::Disabled);

        int w_width, w_height;
        window_.getWindowSize(&w_width, &w_height);
        window_.setCursorPosition(static_cast<double>(w_width) / 2.0,
                                  static_cast<double>(w_height) / 2.0);

        main_camera_ = std::make_shared<Camera>();
        camera_ = main_camera_;

        main_camera_->m_plane = {w_width, w_height};

        glad::Enable(glad::Capability::DepthTest);
        glad::Enable(glad::Capability::CullFace);
        gameObjects_.emplace_back(std::make_unique<Player>(main_camera_));
        gameObjects_.emplace_back(std::make_unique<World>(main_camera_));
        gameObjects_.emplace_back(std::make_unique<Gizmo>());
    }

    Game::~Game() {
        window_.framebufferSizeEvent.unregister(frame_buffer_event_handle);
    }
}
