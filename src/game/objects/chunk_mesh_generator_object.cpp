#include "chunk_mesh_generator_object.h"

namespace cubexx {
    constexpr unsigned CHUNKS_PER_FRAME = 7;

    ChunkMeshGeneratorObject::ChunkMeshGeneratorObject(const std::shared_ptr<World>& world,
                                                       const std::shared_ptr<ChunkMeshGenerator>& chunk_mesh_generator)
        : world_(world), chunk_mesh_generator_(chunk_mesh_generator) {}

    void ChunkMeshGeneratorObject::update(float deltaTime) {
        for (unsigned i = 0; i < CHUNKS_PER_FRAME; ++i) {
            if (world_->chunksToGenerateMesh.empty())
                return;

            const auto& chunkIndex = world_->chunksToGenerateMesh.front();
            world_->chunksToGenerateMesh.pop_front();

            const auto& chunk = world_->chunks[chunkIndex];

            if (chunk->meshData) {
                continue;
            }

            chunk->meshData = std::make_unique<ChunkMeshData>();
            chunk_mesh_generator_->Generate(chunk);
        }
    }
}
