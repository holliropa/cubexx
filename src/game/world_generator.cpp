#include "world_generator.h"
#include "stb_perlin.h"

namespace cubexx {
    void WorldGenerator::Generate(const std::shared_ptr<Chunk>& chunk) {
        constexpr int GROUND_LEVEL = 64;
        constexpr int AMPLITUDE = 16;
        constexpr float FREQUENCY = 0.01f;

        const glm::ivec3 chunkPos = chunk->index;

        std::array<std::array<int, CHUNK_SIZE>, CHUNK_SIZE> heightmap{};
        for (int x = 0; x < CHUNK_SIZE; ++x) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                const int worldX = chunkPos.x * CHUNK_SIZE + x;
                const int worldZ = chunkPos.z * CHUNK_SIZE + z;

                const float noise = stb_perlin_noise3(
                    static_cast<float>(worldX) * FREQUENCY,
                    static_cast<float>(worldZ) * FREQUENCY,
                    0, 0, 0, 0
                );

                heightmap[x][z] = GROUND_LEVEL + static_cast<int>(noise * AMPLITUDE);
            }
        }

        for (int x = 0; x < CHUNK_SIZE; ++x) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                const int height = heightmap[x][z];
                for (int y = 0; y < CHUNK_SIZE; ++y) {
                    const int worldY = chunkPos.y * CHUNK_SIZE + y;

                    if (worldY < height) {
                        chunk->data.isEmpty = false;
                        chunk->data.cubes[x][y][z] = 1;
                    }
                    else {
                        chunk->data.cubes[x][y][z] = 0;
                    }
                }
            }
        }
    }
}
