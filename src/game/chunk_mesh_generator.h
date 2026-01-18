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
        glm::vec4 tile_bounds;
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

        void add_quad(std::vector<CubeVertex>& vertices, std::vector<GLuint>& indices, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2,
                      const glm::vec3& p3, float width, float height,
                      const glm::vec3& normal, CubeType type, bool backFace) const;

        void generate_axis_mesh(const std::shared_ptr<Chunk>& chunk, MeshData& mesh_data, int axis);

        static CubeType get_cube_type(const std::shared_ptr<Chunk>& chunk, int x, int y, int z);
    };
}
