#pragma once
#include <memory>
#include <optional>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "chunk.h"
#include "glm/gtx/string_cast.hpp"

namespace cubexx {
    class World {
    private:
        static glm::ivec3 world_to_chunk_index(const glm::ivec3& world_pos) {
            return {
                world_pos.x >= 0 ? world_pos.x / CHUNK_SIZE : (world_pos.x - CHUNK_SIZE + 1) / CHUNK_SIZE,
                world_pos.y >= 0 ? world_pos.y / CHUNK_SIZE : (world_pos.y - CHUNK_SIZE + 1) / CHUNK_SIZE,
                world_pos.z >= 0 ? world_pos.z / CHUNK_SIZE : (world_pos.z - CHUNK_SIZE + 1) / CHUNK_SIZE
            };
        }

        static glm::ivec3 world_to_cube_index(const glm::ivec3& world_pos) {
            auto cube_index = world_pos % CHUNK_SIZE;
            if (cube_index.x < 0) cube_index.x += CHUNK_SIZE;
            if (cube_index.y < 0) cube_index.y += CHUNK_SIZE;
            if (cube_index.z < 0) cube_index.z += CHUNK_SIZE;
            return cube_index;
        }

    public:
        std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> chunks;
        std::unordered_set<glm::ivec3> visibleChunks;
        std::deque<glm::ivec3> chunksToLoad;
        std::deque<glm::ivec3> chunksToGenerateMesh;
        std::deque<glm::ivec3> chunksToUnload;
        std::optional<glm::ivec3> selectedBlock;

        void set_block(const glm::ivec3& world_position, const CubeType type) {
            const auto chunk_index = world_to_chunk_index(world_position);
            const auto cube_index = world_to_cube_index(world_position);

            if (const auto it = chunks.find(chunk_index); it != chunks.end()) {
                it->second->data.cubes[cube_index.x][cube_index.y][cube_index.z] = type;

                if (!it->second->isDirty) {
                    it->second->isDirty = true;
                    chunksToGenerateMesh.push_back(chunk_index);
                }

                auto maybe_mark_dirty = [&](const glm::ivec3& neighbor_offset) {
                    const auto neighbor_index = chunk_index + neighbor_offset;
                    if (const auto nit = chunks.find(neighbor_index); nit != chunks.end() && !nit->second->isDirty) {
                        nit->second->isDirty = true;
                        chunksToGenerateMesh.push_back(neighbor_index);
                    }
                };

                if (cube_index.x == 0) maybe_mark_dirty({-1, 0, 0});
                if (cube_index.x == CHUNK_SIZE - 1) maybe_mark_dirty({1, 0, 0});
                if (cube_index.y == 0) maybe_mark_dirty({0, -1, 0});
                if (cube_index.y == CHUNK_SIZE - 1) maybe_mark_dirty({0, 1, 0});
                if (cube_index.z == 0) maybe_mark_dirty({0, 0, -1});
                if (cube_index.z == CHUNK_SIZE - 1) maybe_mark_dirty({0, 0, 1});
            }
        }

        CubeType get_block(const glm::ivec3 block_pos) {
            const auto chunk_index = world_to_chunk_index(block_pos);
            const auto cube_index = world_to_cube_index(block_pos);

            if (const auto it = chunks.find(chunk_index); it != chunks.end()) {
                return it->second->data.cubes[cube_index.x][cube_index.y][cube_index.z];
            }
            return CubeType::Air;
        }
    };
}
