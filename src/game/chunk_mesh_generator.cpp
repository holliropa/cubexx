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
        glm::vec3 normal,
        const glm::vec3* faceVertices,
        glm::vec2* faceUV
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


    void ChunkMeshGenerator::Generate(const std::shared_ptr<Chunk>& chunk) {
        std::vector<CubeVertex> vertices;
        std::vector<GLuint> indices;

        for (int x = 0; x < CHUNK_SIZE; ++x) {
            for (int y = 0; y < CHUNK_SIZE; ++y) {
                for (int z = 0; z < CHUNK_SIZE; ++z) {
                    if (chunk->data.cubes[x][y][z] == 0) continue;

                    constexpr glm::vec3 rightFace[4] = {
                        {1, 0, 0},
                        {1, 1, 0},
                        {1, 1, 1},
                        {1, 0, 1}
                    };
                    glm::vec2 rightUV[4] = {
                        {1, 0},
                        {1, 1},
                        {0, 1},
                        {0, 0},
                    };

                    constexpr glm::vec3 leftFace[4] = {
                        {0, 0, 0},
                        {0, 0, 1},
                        {0, 1, 1},
                        {0, 1, 0}
                    };
                    glm::vec2 leftUV[4] = {
                        {0, 0},
                        {1, 0},
                        {1, 1},
                        {0, 1},
                    };

                    constexpr glm::vec3 topFace[4] = {
                        {0, 1, 0},
                        {0, 1, 1},
                        {1, 1, 1},
                        {1, 1, 0}
                    };
                    glm::vec2 topUV[4] = {
                        {0, 1},
                        {0, 0},
                        {1, 0},
                        {1, 1},
                    };

                    constexpr glm::vec3 bottomFace[4] = {
                        {0, 0, 0},
                        {1, 0, 0},
                        {1, 0, 1},
                        {0, 0, 1}
                    };
                    glm::vec2 bottomUV[4] = {
                        {0, 0},
                        {1, 0},
                        {1, 1},
                        {0, 1},
                    };

                    constexpr glm::vec3 frontFace[4] = {
                        {0, 0, 1},
                        {1, 0, 1},
                        {1, 1, 1},
                        {0, 1, 1}
                    };
                    glm::vec2 frontUV[4] = {
                        {0, 0},
                        {1, 0},
                        {1, 1},
                        {0, 1},
                    };

                    constexpr glm::vec3 backFace[4] = {
                        {0, 0, 0},
                        {0, 1, 0},
                        {1, 1, 0},
                        {1, 0, 0}
                    };
                    glm::vec2 backUV[4] = {
                        {1, 0},
                        {1, 1},
                        {0, 1},
                        {0, 0},
                    };


                    if (x + 1 == CHUNK_SIZE || chunk->data.cubes[x + 1][y][z] == 0)
                        addFace(vertices, indices, {x, y, z}, {1, 0, 0}, rightFace, rightUV);
                    if (x - 1 == -1 || chunk->data.cubes[x - 1][y][z] == 0)
                        addFace(vertices, indices, {x, y, z}, {-1, 0, 0}, leftFace, leftUV);
                    if (y + 1 == CHUNK_SIZE || chunk->data.cubes[x][y + 1][z] == 0)
                        addFace(vertices, indices, {x, y, z}, {0, 1, 0}, topFace, topUV);
                    if (y - 1 == -1 || chunk->data.cubes[x][y - 1][z] == 0)
                        addFace(vertices, indices, {x, y, z}, {0, -1, 0}, bottomFace, bottomUV);
                    if (z + 1 == CHUNK_SIZE || chunk->data.cubes[x][y][z + 1] == 0)
                        addFace(vertices, indices, {x, y, z}, {0, 0, 1}, frontFace, frontUV);
                    if (z - 1 == -1 || chunk->data.cubes[x][y][z - 1] == 0)
                        addFace(vertices, indices, {x, y, z}, {0, 0, -1}, backFace, backUV);
                }
            }
        }

        chunk->meshData->index_count = indices.size();

        glad::Bind(chunk->meshData->vertexArray);

        chunk->meshData->arrayBuffer.data(sizeof(CubeVertex) * vertices.size(), vertices.data());
        chunk->meshData->elementArrayBuffer.data(sizeof(GLuint) * indices.size(), indices.data());

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
