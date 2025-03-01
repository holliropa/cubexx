#pragma once

#include <memory>
#include <set>
#include <queue>
#include "gl_common.hpp"
#include "game_object.hpp"
#include "chunk.hpp"
#include "world_generator.hpp"
#include "chunk_mesh_generator.hpp"
#include <unordered_set>

namespace cubexx {
    class World : public GameObject {
    public:
        explicit World(Camera* camera);

        void Initialize() override;

        void Update(float delta) override;

        void LateUpdate(float delta) override;

        void Render(const Camera& camera) const override;

    private:
        bool enabled_ = false;
        Camera* camera_;
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
