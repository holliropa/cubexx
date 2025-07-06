#pragma once

#include "bw/engine/application.h"
#include "bw/engine/engine.h"
#include "bw/engine/game.h"

#include "core/camera.h"

#include "game/chunk_mesh_generator.h"
#include "game/config.h"
#include "game/world.h"
#include "game/world_generator.h"

namespace cubexx {
    class Game : public bw::engine::Game {
    public:
        Game(bw::engine::Application& application, bw::engine::Engine& engine);

        ~Game();

    private:
        std::shared_ptr<Camera> main_camera_;
        size_t frame_buffer_event_handle;

        std::shared_ptr<World> world_;
        std::shared_ptr<Config> config_;
        std::shared_ptr<WorldGenerator> world_generator_;
        std::shared_ptr<ChunkMeshGenerator> chunk_mesh_generator_;
    };
}
