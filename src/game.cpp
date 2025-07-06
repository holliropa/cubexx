#include "game.h"

#include "bw/engine/input.h"

#include "game/objects/chunk_generator_object.h"
#include "game/objects/chunk_mesh_generator_object.h"
#include "game/objects/chunk_unloader_object.h"
#include "game/objects/player_object.h"
#include "game/objects/gizmo_object.h"
#include "game/objects/world_object.h"
#include "game/objects/world_renderer_object.h"

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
        bw::engine::Input::InitCursor(static_cast<float>(w_width) / 2.0f,
                                      static_cast<float>(w_height) / 2.0f);

        glad::Enable(glad::Capability::DepthTest);
        glad::Enable(glad::Capability::CullFace);

        main_camera_ = std::make_shared<Camera>();
        camera_ = main_camera_;
        main_camera_->m_plane = {w_width, w_height};

        config_ = std::make_shared<Config>();
        world_ = std::make_shared<World>();
        world_generator_ = std::make_shared<WorldGenerator>();
        chunk_mesh_generator_ = std::make_shared<ChunkMeshGenerator>();

        const auto player = std::make_shared<PlayerObject>(main_camera_);
        const auto gizmo = std::make_shared<GizmoObject>();
        const auto world = std::make_shared<WorldObject>(config_, main_camera_, world_);
        const auto chunk_generator = std::make_shared<ChunkGeneratorObject>(config_, world_, world_generator_);
        const auto chunk_mesh_generator = std::make_shared<ChunkMeshGeneratorObject>(config_, world_, chunk_mesh_generator_);
        const auto worldRenderer = std::make_shared<WorldRendererObject>(world_);
        const auto chunk_unloader = std::make_shared<ChunkUnloaderObject>(world_);

        gameObjects_.push_back(player);
        gameObjects_.push_back(gizmo);
        gameObjects_.push_back(world);
        gameObjects_.push_back(chunk_generator);
        gameObjects_.push_back(chunk_mesh_generator);
        gameObjects_.push_back(worldRenderer);
        gameObjects_.push_back(chunk_unloader);
    }

    Game::~Game() {
        window_.framebufferSizeEvent.unregister(frame_buffer_event_handle);
    }
}
