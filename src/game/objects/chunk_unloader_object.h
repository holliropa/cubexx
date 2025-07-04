#pragma once
#include "bw/engine/base_object.h"

#include "../world.h"

namespace cubexx {
    class ChunkUnloaderObject final : public bw::engine::BaseObject {
    public:
        explicit ChunkUnloaderObject(const std::shared_ptr<World>& world);

        void update(float deltaTime) override;

    private:
        std::shared_ptr<World> world_;
    };
}
