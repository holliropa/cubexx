#include "cubexx/world_generator.hpp"
#include "stb_perlin.h"

namespace cubexx {
    void WorldGenerator::Generate(const std::shared_ptr<Chunk>& chunk) {
        constexpr int GROUND_LEVEL = 64;
        constexpr int AMPLITUDE = 16;
        constexpr float FREQUENCY = 0.01f;

        const glm::ivec3 chunkPos = chunk->position;

        std::array<std::array<int, Chunk::SIZE>, Chunk::SIZE> heightmap{};
        for (int x = 0; x < Chunk::SIZE; ++x) {
            for (int z = 0; z < Chunk::SIZE; ++z) {
                int worldX = chunkPos.x * Chunk::SIZE + x;
                int worldZ = chunkPos.z * Chunk::SIZE + z;

                float noise = stb_perlin_noise3(
                    worldX * FREQUENCY,
                    worldZ * FREQUENCY,
                    0, 0, 0, 0
                );

                heightmap[x][z] = GROUND_LEVEL + static_cast<int>(noise * AMPLITUDE);
            }
        }

        for (int x = 0; x < Chunk::SIZE; ++x) {
            for (int z = 0; z < Chunk::SIZE; ++z) {
                int height = heightmap[x][z];
                for (int y = 0; y < Chunk::SIZE; ++y) {
                    int worldY = chunkPos.y * Chunk::SIZE + y;

                    if (worldY < height) {
                        chunk->isEmpty_ = false;
                        chunk->cubes[x][y][z] = 1;
                    }
                    else {
                        chunk->cubes[x][y][z] = 0;
                    }
                }
            }
        }
    }
}
