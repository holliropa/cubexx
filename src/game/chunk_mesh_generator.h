#pragma once

#include <memory>
#include "chunk.h"
#include "cubes/cube_type_registry.h"
#include "cubes/texture_manager.h"

namespace cubexx {
    struct CubeVertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };

    struct MeshData {
        std::vector<CubeVertex> solidVertices;
        std::vector<GLuint> solidIndices;
        std::vector<CubeVertex> transparentVertices;
        std::vector<GLuint> transparentIndices;
        bool ready = false;
    };

    class ChunkMeshGenerator {
    public:
        ChunkMeshGenerator(const std::shared_ptr<CubeTypeRegistry>& cube_type_registry,
                           const std::shared_ptr<TextureManager>& texture_manager);

        MeshData Generate(const std::shared_ptr<Chunk>& chunk);

    private:
        std::shared_ptr<CubeTypeRegistry> cube_type_registry_;
        std::shared_ptr<TextureManager> texture_manager_;
    };
}
