#pragma once

#include "bw/engine/base_object.h"

#include "../chunk_mesh_generator.h"
#include "../world.h"
#include "../config.h"
#include "../../core/task_system.h"

namespace cubexx {
    class ChunkMeshGeneratorObject final : public bw::engine::BaseObject {
    public:
        ChunkMeshGeneratorObject(const std::shared_ptr<Config>& config,
                                 const std::shared_ptr<World>& world,
                                 const std::shared_ptr<ChunkMeshGenerator>& chunk_mesh_generator);

        void update(float deltaTime) override;

    private:
        std::shared_ptr<Config> config_;
        std::shared_ptr<World> world_;
        std::shared_ptr<ChunkMeshGenerator> chunk_mesh_generator_;
        std::unique_ptr<TaskSystem> task_system_;
        std::mutex results_mutex_;
        std::queue<std::pair<std::shared_ptr<Chunk>, MeshData>> completed_meshes_;
    };
}
