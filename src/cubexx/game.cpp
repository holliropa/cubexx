#include <iostream>
#include "cubexx/game.hpp"
#include "cubexx/input.hpp"
#include "cubexx/player.hpp"
#include "cubexx/world.hpp"
#include "cubexx/gizmo.hpp"

namespace cubexx {
    Game::Game(Application& application, Engine& engine)
        : application_(application),
          engine_(engine),
          pWindow_(application.getWindow()),
          pCamera_(std::make_unique<Camera>()) {
        pWindow_->keyEvent.set(
            [](glfw::KeyCode key, int, glfw::KeyAction action, glfw::ModifierKeyBit) {
                Input::HandleKey(key, action);
            });
        pWindow_->framebufferSizeEvent.set(
            [this](int width, int height) {
                glad::Viewport(width, height);
                pCamera_->m_plane = {width, height};
            });
        pWindow_->cursorPosEvent.set(
            [](double x, double y) {
                Input::HandleCursorPosition(static_cast<float>(x), static_cast<float>(y));
            });

        pWindow_->setCursorMode(glfw::CursorMode::Disabled);
        int w_width, w_height;
        pWindow_->getWindowSize(&w_width, &w_height);
        pWindow_->setCursorPosition(static_cast<double>(w_width) / 2.0,
                                    static_cast<double>(w_height) / 2.0);

        pCamera_->m_plane = {application_.width_, application_.height_};

        glad::Enable(glad::Capability::DepthTest);
        glad::Enable(glad::Capability::CullFace);
        pObjects_.emplace_back(std::make_unique<Player>(pCamera_.get()));
        pObjects_.emplace_back(std::make_unique<World>(pCamera_.get()));
        pObjects_.emplace_back(std::make_unique<Gizmo>());

        for (const auto& object : pObjects_) {
            object->Initialize();
        }
    }

    void Game::run() {
        auto last_time = glfw::getTime();
        auto last_FPS_update_time = glfw::getTime();
        unsigned frameCount = 0;
        while (!pWindow_->shouldClose()) {
            glfwPollEvents();
            Input::Update();

            const auto current_time = glfw::getTime();
            const auto delta = static_cast<float>(current_time - last_time);
            last_time = current_time;

            frameCount++;
            if (const auto time_since_FPS_update = current_time - last_FPS_update_time; time_since_FPS_update > 1.0) {
                const auto fps = static_cast<double>(frameCount) / time_since_FPS_update;
                std::cout << "FPS: " << fps << '\n';
                last_FPS_update_time = current_time;
                frameCount = 0;
            }

            for (const auto& object : pObjects_) {
                object->Update(delta);
            }

            for (const auto& object : pObjects_) {
                object->LateUpdate(delta);
            }

            glad::ClearBuffers().Color().Depth();
            glad::ClearColor(0.1f, 0.4f, 0.7f);

            for (const auto& object : pObjects_) {
                object->Render(*pCamera_);
            }

            if (Input::GetKeyDown(glfw::KeyCode::Escape)) {
                pWindow_->setShouldClose(true);
            }

            pWindow_->swapBuffers();
        }
    }

    Game::~Game() {
        pObjects_.clear();
    }
}
