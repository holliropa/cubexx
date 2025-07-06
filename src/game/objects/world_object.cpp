#include <iostream>
#include <deque>
#include <future>

#include "world_object.h"
#include "stb_image.h"
#include "../world.h"
#include "bw/engine/input.h"

namespace cubexx {
    std::vector<glm::ivec3> getSpherePoints(const glm::ivec3& center, const int r) {
        std::vector<glm::ivec3> points;
        const auto r_squared = r * r;
        for (auto x = -r; x <= r; ++x) {
            const auto x_squared = x * x;
            const auto remaining_squared_y = r_squared - x_squared;
            if (remaining_squared_y < 0)
                continue;

            const auto y_max = static_cast<int>(sqrt(remaining_squared_y));
            for (auto y = -y_max; y <= y_max; ++y) {
                const auto y_squared = y * y;
                const auto remaining_squared_z = remaining_squared_y - y_squared;
                if (remaining_squared_z < 0)
                    continue;
                const auto z_max = static_cast<int>(sqrt(remaining_squared_z));
                for (auto z = -z_max; z <= z_max; ++z) {
                    points.emplace_back(center + glm::ivec3{x, y, z});
                }
            }
        }
        return points;
    }

    WorldObject::WorldObject(const std::shared_ptr<Config>& config,
                             const std::shared_ptr<Camera>& camera,
                             const std::shared_ptr<World>& world)
        : config_(config), camera_(camera), world_(world) {}

    void WorldObject::init() {
        userChunkIndex_ = glm::ivec3(glm::floor(camera_->transform.position * (1.0f / CHUNK_SIZE)));
    }

    void WorldObject::update(float deltaTime) {
        const auto currentChunkIndex = glm::ivec3(glm::floor(camera_->transform.position * (1.0f / CHUNK_SIZE)));
        if (currentChunkIndex != userChunkIndex_ || first_) {
            userChunkIndex_ = currentChunkIndex;
            auto chunksInView = getSpherePoints(userChunkIndex_, static_cast<int>(config_->ViewDistance));

            std::sort(chunksInView.begin(), chunksInView.end(),
                      [this](const glm::ivec3& a, const glm::ivec3& b) {
                          const auto distA = glm::distance2(glm::vec3(a), glm::vec3(userChunkIndex_));
                          const auto distB = glm::distance2(glm::vec3(b), glm::vec3(userChunkIndex_));
                          return distA < distB;
                      });

            world_->visibleChunks = std::unordered_set(chunksInView.begin(), chunksInView.end());
            world_->chunksToLoad = std::deque(chunksInView.begin(), chunksInView.end());
            world_->chunksToGenerateMesh = std::deque<glm::ivec3>();

            // world_->chunksToUnload = std::deque<glm::ivec3>();
            // for (const auto& [index, chunk] : world_->chunks) {
            //     if (world_->visibleChunks.find(index) == world_->visibleChunks.end()) {
            //         world_->chunksToUnload.push_back(index);
            //     }
            // }
            //
            // std::sort(world_->chunksToUnload.begin(), world_->chunksToUnload.end(),
            //           [this](const glm::ivec3& a, const glm::ivec3& b) {
            //               const auto distA = glm::distance2(glm::vec3(a), glm::vec3(userChunkIndex_));
            //               const auto distB = glm::distance2(glm::vec3(b), glm::vec3(userChunkIndex_));
            //               return distA > distB;
            //           });

            first_ = false;
        }
    }
}
