#include "chunk_generator_object.h"

namespace cubexx {
    ChunkGeneratorObject::ChunkGeneratorObject(const std::shared_ptr<Config>& config,
                                               const std::shared_ptr<World>& world,
                                               const std::shared_ptr<WorldGenerator>& worldGenerator)
        : config_(config), world_(world), worldGenerator_(worldGenerator) {}

    void ChunkGeneratorObject::update(float deltaTime) {
        for (unsigned i = 0; i < config_->ChunksPerFrame; ++i) {
            if (world_->chunksToLoad.empty())
                return;

            const auto& chunkIndex = world_->chunksToLoad.front();
            world_->chunksToLoad.pop_front();

            std::shared_ptr<Chunk> chunk;
            if (world_->chunks.find(chunkIndex) != world_->chunks.end())
                chunk = world_->chunks[chunkIndex];
            else {
                world_->chunks.emplace(chunkIndex, std::make_shared<Chunk>());
                chunk = world_->chunks[chunkIndex];
                chunk->index = chunkIndex;
                worldGenerator_->Generate(chunk);
            }

            if (!chunk->neighbors[0]) {
                const auto chunkUpIndex = chunk->index + glm::ivec3(0, 1, 0);

                world_->chunks.emplace(chunkUpIndex, std::make_shared<Chunk>());
                auto chunkUp = world_->chunks[chunkUpIndex];
                chunkUp->index = chunkUpIndex;
                worldGenerator_->Generate(chunkUp);

                chunkUp->neighbors[1] = chunk;
                chunk->neighbors[0] = chunkUp;
            }

            if (!chunk->neighbors[1]) {
                const auto chunkDownIndex = chunk->index + glm::ivec3(0, -1, 0);

                world_->chunks.emplace(chunkDownIndex, std::make_shared<Chunk>());
                auto chunkDown = world_->chunks[chunkDownIndex];
                chunkDown->index = chunkDownIndex;
                worldGenerator_->Generate(chunkDown);

                chunkDown->neighbors[0] = chunk;
                chunk->neighbors[1] = chunkDown;
            }


            if (!chunk->neighbors[2]) {
                const auto chunkRightIndex = chunk->index + glm::ivec3(1, 0, 0);

                world_->chunks.emplace(chunkRightIndex, std::make_shared<Chunk>());
                auto chunkRight = world_->chunks[chunkRightIndex];
                chunkRight->index = chunkRightIndex;
                worldGenerator_->Generate(chunkRight);

                chunkRight->neighbors[3] = chunk;
                chunk->neighbors[2] = chunkRight;
            }

            if (!chunk->neighbors[3]) {
                const auto chunkLeftIndex = chunk->index + glm::ivec3(-1, 0, 0);

                world_->chunks.emplace(chunkLeftIndex, std::make_shared<Chunk>());
                auto chunkLeft = world_->chunks[chunkLeftIndex];
                chunkLeft->index = chunkLeftIndex;
                worldGenerator_->Generate(chunkLeft);

                chunkLeft->neighbors[2] = chunk;
                chunk->neighbors[3] = chunkLeft;
            }

            if (!chunk->neighbors[4]) {
                const auto chunkFrontIndex = chunk->index + glm::ivec3(0, 0, 1);

                world_->chunks.emplace(chunkFrontIndex, std::make_shared<Chunk>());
                auto chunkFront = world_->chunks[chunkFrontIndex];
                chunkFront->index = chunkFrontIndex;
                worldGenerator_->Generate(chunkFront);

                chunkFront->neighbors[5] = chunk;
                chunk->neighbors[4] = chunkFront;
            }

            if (!chunk->neighbors[5]) {
                const auto chunkBackIndex = chunk->index + glm::ivec3(0, 0, -1);

                world_->chunks.emplace(chunkBackIndex, std::make_shared<Chunk>());
                auto chunkBack = world_->chunks[chunkBackIndex];
                chunkBack->index = chunkBackIndex;
                worldGenerator_->Generate(chunkBack);

                chunkBack->neighbors[4] = chunk;
                chunk->neighbors[5] = chunkBack;
            }


            if (!chunk->meshData)
                world_->chunksToGenerateMesh.push_back(chunkIndex);
        }
    }
}
