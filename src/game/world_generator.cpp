#include "world_generator.h"

namespace cubexx::worldgen {
    namespace {
        float clamp01(float v) {
            return std::max(0.0f, std::min(1.0f, v));
        }

        int compute_surface_y(const Params& p,
                                     const Noise& n,
                                     const int worldX, const int worldZ) {
            const auto fx = static_cast<float>(worldX);
            const auto fz = static_cast<float>(worldZ);

            const float continent = n.continent(fx * p.continent_freq, fz * p.continent_freq); // -1..1

            const float h1 = n.hills(fx * p.hills_freq, fz * p.hills_freq);
            const float h2 = n.hills(fx * (p.hills_freq * 2.0f), fz * (p.hills_freq * 2.0f)) * 0.5f;
            const float hills = (h1 + h2) / 1.5f;

            const float detail = n.detail(fx * p.detail_freq, fz * p.detail_freq);

            float land = clamp01((continent + 1.0f) * 0.5f); // 0..1
            if (p.ocean_bias) land = land * land;

            const int base = p.sea_level + p.base_offset;
            return base
                + static_cast<int>(land * static_cast<float>(p.continent_amp))
                + static_cast<int>(hills * static_cast<float>(p.hills_amp))
                + static_cast<int>(detail * static_cast<float>(p.detail_amp));
        }

        bool should_carve_cave(const Params& p,
                                      const Noise& n,
                                      const int worldX, const int worldY, const int worldZ,
                                      const int surfaceY) {
            if (worldY < p.cave_min_y) return false;

            const int depth = surfaceY - worldY; // 0 at surface, increases downward
            if (depth <= 0) return false;

            const auto fx = static_cast<float>(worldX);
            const auto fy = static_cast<float>(worldY);
            const auto fz = static_cast<float>(worldZ);

            float caveV = n.cave(fx * p.cave_freq, fy * p.cave_freq, fz * p.cave_freq); // -1..1
            caveV = caveV * 0.5f + 0.5f; // 0..1

            // Deep carving rule (original intent): keep away from surface
            if (depth > p.cave_surface_buffer) {
                return caveV > p.cave_threshold;
            }

            // NEW: Entrance rule (near surface), but only in entrance regions and only
            // within a limited depth to avoid random pinholes everywhere.
            if (depth <= p.cave_entrance_max_depth) {
                float e = n.entrance(fx * p.cave_entrance_freq, fz * p.cave_entrance_freq); // -1..1
                e = e * 0.5f + 0.5f; // 0..1

                // Require both: "this area wants entrances" AND "cave field is strong"
                // Slightly lower cave requirement near surface so it actually breaks through.
                const float near_surface_cave_threshold = std::max(0.0f, p.cave_threshold - 0.10f);

                if (e > p.cave_entrance_threshold && caveV > near_surface_cave_threshold) {
                    return true;
                }
            }

            return false;
        }
    }

    WorldGenerator::WorldGenerator(const std::uint32_t seed, const Params& params)
        : seed_(seed), params_(params), noise_(seed_) {}

    void WorldGenerator::Generate(const std::shared_ptr<Chunk>& chunk) const {
        const glm::ivec3 chunkPos = chunk->index;

        std::array<std::array<int, CHUNK_SIZE>, CHUNK_SIZE> heightmap{};
        for (int x = 0; x < CHUNK_SIZE; ++x) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                const int worldX = chunkPos.x * CHUNK_SIZE + x;
                const int worldZ = chunkPos.z * CHUNK_SIZE + z;
                heightmap[x][z] = compute_surface_y(params_, noise_, worldX, worldZ);
            }
        }

        bool isTotalSolid = true;
        chunk->data.isEmpty = true;
        chunk->data.hasTransparent = false;

        for (int x = 0; x < CHUNK_SIZE; ++x) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                const int surfaceY = heightmap[x][z];
                const int worldX = chunkPos.x * CHUNK_SIZE + x;
                const int worldZ = chunkPos.z * CHUNK_SIZE + z;

                for (int y = 0; y < CHUNK_SIZE; ++y) {
                    const int worldY = chunkPos.y * CHUNK_SIZE + y;

                    if (worldY > surfaceY) {
                        isTotalSolid = false;

                        if (worldY <= params_.sea_level) {
                            chunk->data.cubes[x][y][z] = CubeType::Water;
                            chunk->data.isEmpty = false;
                            chunk->data.hasTransparent = true;
                        } else {
                            chunk->data.cubes[x][y][z] = CubeType::Air;
                        }
                        continue;
                    }

                    chunk->data.isEmpty = false;

                    if (should_carve_cave(params_, noise_, worldX, worldY, worldZ, surfaceY)) {
                        chunk->data.cubes[x][y][z] = CubeType::Air;
                        isTotalSolid = false;
                        continue;
                    }

                    if (worldY == surfaceY) {
                        chunk->data.cubes[x][y][z] = (worldY >= params_.sea_level) ? CubeType::Grass : CubeType::Dirt;
                        continue;
                    }

                    if (worldY > surfaceY - params_.topsoil_depth) {
                        chunk->data.cubes[x][y][z] = CubeType::Dirt;
                        continue;
                    }

                    chunk->data.cubes[x][y][z] = CubeType::Stone;
                }
            }
        }

        if (chunk->data.isEmpty) isTotalSolid = false;
        chunk->data.isOpaque = isTotalSolid && !chunk->data.hasTransparent;
    }
}
