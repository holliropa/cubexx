#pragma once

#include <memory>
#include "chunk.h"
#include "cubes/cube_type_registry.h"
#include "cubes/texture_manager.h"

namespace cubexx {
    class ChunkMeshGenerator {
    public:
        ChunkMeshGenerator(const std::shared_ptr<CubeTypeRegistry>& cube_type_registry,
                           const std::shared_ptr<TextureManager>& texture_manager);

        void Generate(const std::shared_ptr<Chunk>& chunk);

    private:
        std::shared_ptr<CubeTypeRegistry> cube_type_registry_;
        std::shared_ptr<TextureManager> texture_manager_;
    };
}
