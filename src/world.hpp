#pragma once

#include <queue>
#include <unordered_set>

#include "camera.h"
#include "chunk_mesh_generator.hpp"
#include "world_generator.hpp"
#include "bw/engine/game_object.h"
#include "bw/engine/gl.h"


namespace cubexx {
    class World final : public bw::engine::GameObject {
    public:
        explicit World(const std::shared_ptr<Camera>& camera);

        void init() override;

        void update(float deltaTime) override;

        void render(const bw::engine::Camera& camera) override;

    private:
        bool enabled_ = false;
        std::shared_ptr<Camera> camera_;
        Transform transform_;
        glm::ivec3 userChunkIndex_ = glm::zero<glm::ivec3>();
        glad::Program shaderProgram_;
        WorldGenerator worldGenerator_;
        ChunkMeshGenerator chunkMeshGenerator_;
        std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> pChunks_;
        std::vector<glm::ivec3> visibleChunks_;
        std::queue<glm::ivec3> loadQueue_;
        std::queue<glm::ivec3> generateMeshesQueue_;
        std::vector<glm::ivec3> loadedChunks_;
        std::vector<glm::ivec3> generatedMeshes_;
        glad::Texture2D texture_;

        bool check_ = false;

        std::deque<glm::ivec3> loadingQueue_;
        std::unordered_set<glm::ivec3> queuedChunks_;
    };
}
