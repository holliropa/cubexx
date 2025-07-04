#pragma once

#include "bw/engine/base_object.h"

#include "../chunk_mesh_generator.h"
#include "../world.h"

namespace cubexx {
    class ChunkMeshGeneratorObject final : public bw::engine::BaseObject {
    public:
        ChunkMeshGeneratorObject(const std::shared_ptr<World>& world,
                                 const std::shared_ptr<ChunkMeshGenerator>& chunk_mesh_generator);

        void update(float deltaTime) override;

    private:
        std::shared_ptr<World> world_;
        std::shared_ptr<ChunkMeshGenerator> chunk_mesh_generator_;
    };
}
