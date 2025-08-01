#include "world_generator.h"
#include "stb_perlin.h"

namespace cubexx {
    // void WorldGenerator::Generate(const std::shared_ptr<Chunk>& chunk) {
    //     constexpr int GROUND_LEVEL = 64;
    //     constexpr int AMPLITUDE = 16;
    //     constexpr float FREQUENCY = 0.05f;
    //
    //     const glm::ivec3 chunkPos = chunk->index;
    //
    //     std::array<std::array<int, CHUNK_SIZE>, CHUNK_SIZE> heightmap{};
    //     for (int x = 0; x < CHUNK_SIZE; ++x) {
    //         for (int z = 0; z < CHUNK_SIZE; ++z) {
    //             const int worldX = chunkPos.x * CHUNK_SIZE + x;
    //             const int worldZ = chunkPos.z * CHUNK_SIZE + z;
    //
    //             const float noise = stb_perlin_noise3(
    //                 static_cast<float>(worldX) * FREQUENCY,
    //                 static_cast<float>(worldZ) * FREQUENCY,
    //                 0, 0, 0, 0
    //             );
    //
    //             heightmap[x][z] = GROUND_LEVEL + static_cast<int>(noise * AMPLITUDE);
    //         }
    //     }
    //
    //     for (int x = 0; x < CHUNK_SIZE; ++x) {
    //         for (int z = 0; z < CHUNK_SIZE; ++z) {
    //             const int height = heightmap[x][z];
    //             for (int y = 0; y < CHUNK_SIZE; ++y) {
    //                 const int worldY = chunkPos.y * CHUNK_SIZE + y;
    //
    //                 if (worldY > height) {
    //                     chunk->data.cubes[x][y][z] = CubeType::Air;
    //                     continue;
    //                 }
    //
    //                 chunk->data.isEmpty = false;
    //
    //
    //                 if (worldY == height && worldY > GROUND_LEVEL) {
    //                     chunk->data.cubes[x][y][z] = CubeType::Grass;
    //                     continue;
    //                 }
    //
    //                 if (worldY > height - 6) {
    //                     chunk->data.cubes[x][y][z] = CubeType::Dirt;
    //                     continue;
    //                 }
    //
    //
    //                 chunk->data.cubes[x][y][z] = CubeType::Stone;
    //             }
    //         }
    //     }
    // }
    constexpr int GROUND_LEVEL = 64;

    inline CubeType determine_cube_type(const int cube_y) {
        if (cube_y > GROUND_LEVEL)
            return CubeType::Air;

        if (cube_y == GROUND_LEVEL)
            return CubeType::Grass;

        if (cube_y > GROUND_LEVEL - 6)
            return CubeType::Dirt;

        return CubeType::Stone;
    }

    void WorldGenerator::Generate(const std::shared_ptr<Chunk>& chunk) {
        const glm::ivec3 chunkPos = chunk->index;

        for (int x = 0; x < CHUNK_SIZE; ++x) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                for (int y = 0; y < CHUNK_SIZE; ++y) {
                    const auto cube_world_y = chunkPos.y * CHUNK_SIZE + y;

                    chunk->data.cubes[x][y][z] = determine_cube_type(cube_world_y);
                }
            }
        }
    }
}
