#include "chunk_generator_object.h"

namespace cubexx {
    constexpr unsigned CHUNKS_PER_FRAME = 10;

    ChunkGeneratorObject::ChunkGeneratorObject(const std::shared_ptr<World>& world,
                                               const std::shared_ptr<WorldGenerator>& worldGenerator)
        : world_(world), worldGenerator_(worldGenerator) {}

    void ChunkGeneratorObject::update(float deltaTime) {
        for (unsigned i = 0; i < CHUNKS_PER_FRAME; ++i) {
            if (world_->chunksToGenerate.empty())
                return;

            const auto& chunkIndex = world_->chunksToGenerate.front();
            world_->chunksToGenerate.pop_front();
            world_->chunks.emplace(chunkIndex, std::make_shared<Chunk>());
            const auto& chunk = world_->chunks[chunkIndex];
            chunk->index = chunkIndex;
            worldGenerator_->Generate(chunk);

            world_->chunksToGenerateMesh.push_back(chunkIndex);
        }
    }
}
