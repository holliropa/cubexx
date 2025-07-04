#pragma once
#include <memory>

#include "bw/engine/base_object.h"

#include "../world_generator.h"
#include "../world.h"

namespace cubexx {
    class ChunkGeneratorObject final : public bw::engine::BaseObject {
    public:
        ChunkGeneratorObject(const std::shared_ptr<World>& world,
                             const std::shared_ptr<WorldGenerator>& worldGenerator);

        void update(float deltaTime) override;

    private:
        std::shared_ptr<World> world_;
        std::shared_ptr<WorldGenerator> worldGenerator_;
    };
}
