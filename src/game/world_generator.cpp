#include "world_generator.h"
#include "stb_perlin.h"

namespace cubexx {
    void WorldGenerator::Generate(const std::shared_ptr<Chunk>& chunk) {
        constexpr int GROUND_LEVEL = 64;
        constexpr int SEA_LEVEL = 60;
        constexpr int AMPLITUDE = 32; // Increased amplitude
        constexpr float FREQUENCY = 0.02f; // Lower base frequency for larger features
        constexpr int OCTAVES = 4;
        constexpr float LACUNARITY = 2.0f; // Frequency multiplier per octave
        constexpr float PERSISTENCE = 0.5f; // Amplitude multiplier per octave

        const glm::ivec3 chunkPos = chunk->index;

        std::array<std::array<int, CHUNK_SIZE>, CHUNK_SIZE> heightmap{};
        for (int x = 0; x < CHUNK_SIZE; ++x) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                const int worldX = chunkPos.x * CHUNK_SIZE + x;
                const int worldZ = chunkPos.z * CHUNK_SIZE + z;

                float noiseValue = 0.0f;
                float currentFreq = FREQUENCY;
                float currentAmp = 1.0f;
                float maxAmp = 0.0f;

                for (int i = 0; i < OCTAVES; ++i) {
                    noiseValue += stb_perlin_noise3(
                        static_cast<float>(worldX) * currentFreq,
                        static_cast<float>(worldZ) * currentFreq,
                        0, 0, 0, 0
                    ) * currentAmp;

                    maxAmp += currentAmp;
                    currentAmp *= PERSISTENCE;
                    currentFreq *= LACUNARITY;
                }

                // Normalize result to -1..1 range (approx)
                noiseValue /= maxAmp;

                heightmap[x][z] = GROUND_LEVEL + static_cast<int>(noiseValue * AMPLITUDE);
            }
        }


        for (int x = 0; x < CHUNK_SIZE; ++x) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                const int height = heightmap[x][z];
                for (int y = 0; y < CHUNK_SIZE; ++y) {
                    const int worldY = chunkPos.y * CHUNK_SIZE + y;

                    if (worldY > height) {
                        if (worldY <= SEA_LEVEL) {
                            chunk->data.cubes[x][y][z] = CubeType::Water;
                            chunk->data.isEmpty = false;
                            chunk->data.hasTransparent = true;
                        }
                        else {
                            chunk->data.cubes[x][y][z] = CubeType::Air;
                        }
                        continue;
                    }

                    chunk->data.isEmpty = false;


                    if (worldY == height) {
                        if (worldY >= SEA_LEVEL) {
                            chunk->data.cubes[x][y][z] = CubeType::Grass;
                        }
                        else {
                            chunk->data.cubes[x][y][z] = CubeType::Dirt;
                        }
                        continue;
                    }

                    if (worldY > height - 6) {
                        chunk->data.cubes[x][y][z] = CubeType::Dirt;
                        continue;
                    }


                    chunk->data.cubes[x][y][z] = CubeType::Stone;
                }
            }
        }
    }
}
