#include "chunk_mesh_generator.h"
#include "cubes/cube_utils.h"

namespace cubexx {
    ChunkMeshGenerator::ChunkMeshGenerator(const std::shared_ptr<CubeTypeRegistry>& cube_type_registry,
                                           const std::shared_ptr<TextureManager>& texture_manager)
        : cube_type_registry_(cube_type_registry),
          texture_manager_(texture_manager) {}

    // Helper to add a quad with correct winding
    void ChunkMeshGenerator::add_quad(std::vector<CubeVertex>& vertices,
                                      std::vector<GLuint>& indices,
                                      const glm::vec3& p0,
                                      const glm::vec3& p1,
                                      const glm::vec3& p2,
                                      const glm::vec3& p3,
                                      const float width,
                                      const float height,
                                      const glm::vec3& normal,
                                      const CubeType type,
                                      const bool backFace
    ) const {
        const auto tile_index = cube_type_registry_->get_definition(type).faceTiles[0];
        const auto uv_corners = texture_manager_->get_tile_uv(tile_index);

        glm::vec4 bounds;
        bounds.x = uv_corners[0].x; // Min U
        bounds.y = uv_corners[0].y; // Min V
        bounds.z = uv_corners[2].x; // Max U
        bounds.w = uv_corners[2].y; // Max V

        const glm::vec2 uv0(0.0f, 0.0f);
        const glm::vec2 uv1(width, 0.0f);
        const glm::vec2 uv2(width, height);
        const glm::vec2 uv3(0.0f, height);


        const unsigned int index = static_cast<unsigned int>(vertices.size());

        vertices.push_back({p0, normal, uv0, bounds});
        vertices.push_back({p1, normal, uv1, bounds});
        vertices.push_back({p2, normal, uv2, bounds});
        vertices.push_back({p3, normal, uv3, bounds});

        // 4. Indices (Standard Winding)
        if (backFace) {
            indices.push_back(index + 0);
            indices.push_back(index + 2);
            indices.push_back(index + 1);

            indices.push_back(index + 0);
            indices.push_back(index + 3);
            indices.push_back(index + 2);
        }
        else {
            indices.push_back(index + 0);
            indices.push_back(index + 1);
            indices.push_back(index + 2);

            indices.push_back(index + 0);
            indices.push_back(index + 2);
            indices.push_back(index + 3);
        }
    }

    MeshData ChunkMeshGenerator::Generate(const std::shared_ptr<Chunk>& chunk) {
        MeshData mesh_data;

        generate_axis_mesh(chunk, mesh_data, 0);
        generate_axis_mesh(chunk, mesh_data, 1);
        generate_axis_mesh(chunk, mesh_data, 2);

        return mesh_data;
    }

    CubeType ChunkMeshGenerator::get_cube_type(const std::shared_ptr<Chunk>& chunk,
                                               const int x,
                                               const int y,
                                               const int z
    ) {
        if (x >= 0 && x < CHUNK_SIZE &&
            y >= 0 && y < CHUNK_SIZE &&
            z >= 0 && z < CHUNK_SIZE) {
            return chunk->data.cubes[x][y][z];
        }

        int neighbor_idx;
        if (x < 0) neighbor_idx = 3;
        else if (x >= CHUNK_SIZE) neighbor_idx = 2;
        else if (y < 0) neighbor_idx = 1;
        else if (y >= CHUNK_SIZE) neighbor_idx = 0;
        else if (z < 0) neighbor_idx = 5;
        else neighbor_idx = 4;


        if (const auto& neighbor = chunk->neighbors[neighbor_idx]) {
            const int nx = (x + CHUNK_SIZE) % CHUNK_SIZE;
            const int ny = (y + CHUNK_SIZE) % CHUNK_SIZE;
            const int nz = (z + CHUNK_SIZE) % CHUNK_SIZE;
            return neighbor->data.cubes[nx][ny][nz];
        }

        return CubeType::Air;
    }

    void ChunkMeshGenerator::generate_axis_mesh(const std::shared_ptr<Chunk>& chunk, MeshData& mesh_data, int axis) {
        int u = (axis + 1) % 3;
        int v = (axis + 2) % 3;

        int x[3] = {};
        int q[3] = {};
        q[axis] = 1;

        for (int p = 0; p < 2; ++p) {
            bool backFace = (p == 0);

            std::vector<CubeType> mask(CHUNK_SIZE * CHUNK_SIZE);

            for (x[axis] = -1; x[axis] < CHUNK_SIZE; ++x[axis]) {
                int n = 0;

                for (x[v] = 0; x[v] < CHUNK_SIZE; ++x[v]) {
                    for (x[u] = 0; x[u] < CHUNK_SIZE; ++x[u]) {
                        int cx = x[0];
                        int cy = x[1];
                        int cz = x[2];

                        // Neighbor Coordinate Vector (Move 1 step along current axis)
                        int nx = x[0] + q[0];
                        int ny = x[1] + q[1];
                        int nz = x[2] + q[2];

                        CubeType cube1 = get_cube_type(chunk, cx, cy, cz);
                        CubeType cube2 = get_cube_type(chunk, nx, ny, nz);

                        if (backFace) {
                            bool isVisible = cube_utils::is_face_visible(cube2, cube1);
                            mask[n++] = isVisible ? cube2 : CubeType::Air;
                        }
                        else {
                            bool isVisible = cube_utils::is_face_visible(cube1, cube2);
                            mask[n++] = isVisible ? cube1 : CubeType::Air;
                        }
                    }
                }

                n = 0;
                for (int j = 0; j < CHUNK_SIZE; ++j) {
                    for (int i = 0; i < CHUNK_SIZE;) {
                        const auto cube_type = mask[n];

                        if (cube_type == CubeType::Air) {
                            i++;
                            n++;
                            continue;
                        }

                        int w = 1;
                        while (i + w < CHUNK_SIZE && mask[n + w] == cube_type) w++;

                        int h = 1;
                        bool done = false;
                        while (j + h < CHUNK_SIZE) {
                            for (int k = 0; k < w; ++k) {
                                if (mask[n + k + h * CHUNK_SIZE] != cube_type) {
                                    done = true;
                                    break;
                                }
                            }

                            if (done) break;
                            h++;
                        }

                        x[u] = i;
                        x[v] = j;

                        int du[3] = {0};
                        du[u] = w;
                        int dv[3] = {0};
                        dv[v] = h;

                        glm::vec3 pos(x[0], x[1], x[2]);
                        pos[axis] += 1.0f; // Always shift +1 to be between blocks

                        glm::vec3 p0 = pos;
                        glm::vec3 p1 = pos + glm::vec3(du[0], du[1], du[2]);
                        glm::vec3 p2 = pos + glm::vec3(du[0], du[1], du[2]) + glm::vec3(dv[0], dv[1], dv[2]);
                        glm::vec3 p3 = pos + glm::vec3(dv[0], dv[1], dv[2]);

                        glm::vec3 normal(0);
                        normal[axis] = backFace ? -1.0f : 1.0f;

                        if (cube_type != CubeType::Water)
                            add_quad(mesh_data.solidVertices,
                                     mesh_data.solidIndices,
                                     p0, p1, p2, p3,
                                     w, h,
                                     normal,
                                     cube_type,
                                     backFace);
                        else
                            add_quad(mesh_data.transparentVertices, mesh_data.transparentIndices,
                                     p0, p1, p2, p3,
                                     w, h,
                                     normal,
                                     cube_type,
                                     backFace);

                        // 4. Clear Mask
                        for (int l = 0; l < h; ++l) {
                            for (int k = 0; k < w; ++k) {
                                mask[n + k + l * CHUNK_SIZE] = CubeType::Air;
                            }
                        }

                        i += w;
                        n += w;
                    }
                }
            }
        }
    }
}
