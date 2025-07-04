#include "chunk_unloader_object.h"

namespace cubexx {
    constexpr unsigned CHUNKS_PER_FRAME = 5;

    ChunkUnloaderObject::ChunkUnloaderObject(const std::shared_ptr<World>& world)
        : world_(world) {}

    void ChunkUnloaderObject::update(float deltaTime) {
        for (unsigned i = 0; i < CHUNKS_PER_FRAME; ++i) {
            if (world_->chunksToUnload.empty())
                return;

            const auto& chunkIndex = world_->chunksToUnload.front();
            world_->chunksToUnload.pop_front();
            world_->chunks.erase(chunkIndex);
        }
    }
}
