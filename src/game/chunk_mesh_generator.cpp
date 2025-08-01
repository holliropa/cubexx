#include "chunk_mesh_generator.h"

namespace cubexx {
    struct CubeVertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };

    void addFace(
        std::vector<CubeVertex>& vertices,
        std::vector<unsigned int>& indices,
        const glm::vec3 position,
        const glm::vec3 normal,
        const glm::vec3* faceVertices,
        const glm::vec2* faceUV
    ) {
        const auto index = vertices.size(); // Track the current vertex index

        // Add the 4 vertices of the face
        for (int i = 0; i < 4; i++) {
            glm::vec3 vertex = position + faceVertices[i];
            vertex = glm::round(vertex);
            vertices.emplace_back(CubeVertex{
                .position{vertex},
                .normal{normal},
                .uv{faceUV[i]},
            });
        }

        // Define two triangles using indices (0, 1, 2) and (2, 3, 0)
        indices.push_back(index + 0);
        indices.push_back(index + 1);
        indices.push_back(index + 2);

        indices.push_back(index + 2);
        indices.push_back(index + 3);
        indices.push_back(index + 0);
    }

    constexpr glm::vec3 rightFace[4] = {
        {1, 0, 1}, {1, 0, 0}, {1, 1, 0}, {1, 1, 1},
    };

    constexpr glm::vec3 leftFace[4] = {
        {0, 0, 0}, {0, 0, 1}, {0, 1, 1}, {0, 1, 0}
    };

    constexpr glm::vec3 topFace[4] = {
        {0, 1, 0}, {0, 1, 1}, {1, 1, 1}, {1, 1, 0}
    };

    constexpr glm::vec3 bottomFace[4] = {
        {0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1}
    };

    constexpr glm::vec3 frontFace[4] = {
        {0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1}
    };

    constexpr glm::vec3 backFace[4] = {
        {1, 0, 0}, {0, 0, 0}, {0, 1, 0}, {1, 1, 0},
    };

    struct FaceInfo {
        glm::vec3 normal;
        const glm::vec3* vertices;
        glm::ivec3 neighbour_offset;
        size_t tile_index;
        int neighbour_index;
    };

    constexpr FaceInfo face_infos[6] = {
        {{0, 1, 0}, topFace, {0, 1, 0}, 0, 0},
        {{0, -1, 0}, bottomFace, {0, -1, 0}, 1, 1},
        {{1, 0, 0}, rightFace, {1, 0, 0}, 2, 2},
        {{-1, 0, 0}, leftFace, {-1, 0, 0}, 3, 3},
        {{0, 0, 1}, frontFace, {0, 0, 1}, 4, 4},
        {{0, 0, -1}, backFace, {0, 0, -1}, 5, 5},
    };


    ChunkMeshGenerator::ChunkMeshGenerator(const std::shared_ptr<CubeTypeRegistry>& cube_type_registry,
                                           const std::shared_ptr<TextureManager>& texture_manager)
        : cube_type_registry_(cube_type_registry),
          texture_manager_(texture_manager) {}

    void ChunkMeshGenerator::Generate(const std::shared_ptr<Chunk>& chunk) {
        std::vector<CubeVertex> vertices;
        std::vector<GLuint> indices;
        vertices.reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 4 / 2);
        indices.reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 6 / 2);

        for (int x = 0; x < CHUNK_SIZE; ++x) {
            for (int y = 0; y < CHUNK_SIZE; ++y) {
                for (int z = 0; z < CHUNK_SIZE; ++z) {
                    const auto type = chunk->data.cubes[x][y][z];
                    if (type == CubeType::Air) continue;

                    const auto& cube_definition = cube_type_registry_->get_definition(chunk->data.cubes[x][y][z]);

                    for (const auto& [normal,
                             face_vertices,
                             neighbour_offset,
                             tile_index,
                             neighbour_index] :
                         face_infos) {
                        glm::ivec3 offset = glm::ivec3(x, y, z) + neighbour_offset;

                        auto neighbour_cube = CubeType::Air;
                        if (offset.x < 0 || offset.x >= CHUNK_SIZE ||
                            offset.y < 0 || offset.y >= CHUNK_SIZE ||
                            offset.z < 0 || offset.z >= CHUNK_SIZE) {
                            const auto& neighbour_chunk = chunk->neighbors[neighbour_index];
                            const auto neighbour_cube_index = (offset + CHUNK_SIZE) % CHUNK_SIZE;
                            neighbour_cube = neighbour_chunk->data.cubes[neighbour_cube_index.x][neighbour_cube_index.y]
                                [neighbour_cube_index.z];
                        }
                        else {
                            neighbour_cube = chunk->data.cubes[offset.x][offset.y][offset.z];
                        }

                        if (neighbour_cube != CubeType::Air)
                            continue;

                        const auto uv = texture_manager_->get_tile_uv(cube_definition.faceTiles[tile_index]);
                        addFace(vertices, indices, {x, y, z}, normal, face_vertices, uv.begin());
                    }
                }
            }
        }

        chunk->mesh->index_count = indices.size();

        glad::Bind(chunk->mesh->vao);
        chunk->mesh->vbo.data(sizeof(CubeVertex) * vertices.size(), vertices.data());
        chunk->mesh->ebo.data(sizeof(GLuint) * indices.size(), indices.data());

        glad::VertexAttribute(0)
            .pointer(3,
                     glad::DataType::Float,
                     false,
                     sizeof(CubeVertex))
            .enable();
        glad::VertexAttribute(1)
            .pointer(3,
                     glad::DataType::Float,
                     false,
                     sizeof(CubeVertex),
                     reinterpret_cast<void*>(offsetof(CubeVertex, normal)))
            .enable();
        glad::VertexAttribute(2)
            .pointer(2,
                     glad::DataType::Float,
                     false,
                     sizeof(CubeVertex),
                     reinterpret_cast<void*>(offsetof(CubeVertex, uv)))
            .enable();
    }
}
