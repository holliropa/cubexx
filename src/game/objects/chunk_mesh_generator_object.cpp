#include "chunk_mesh_generator_object.h"

namespace cubexx {
    void setup_mesh(Mesh& mesh, const std::vector<CubeVertex>& vertices, const std::vector<GLuint>& indices) {
        glad::Bind(mesh.vao);

        mesh.vbo.data(sizeof(CubeVertex) * vertices.size(), vertices.data());
        mesh.ebo.data(sizeof(GLuint) * indices.size(), indices.data());


        glad::VertexAttribute(0)
            .pointer(3,
                     glad::DataType::Float,
                     false,
                     sizeof(CubeVertex))
            .enable();
        glad::VertexAttribute(1)
            .pointer(3,
                     glad::DataType::Float,
                     false,
                     sizeof(CubeVertex),
                     reinterpret_cast<void*>(offsetof(CubeVertex, normal)))
            .enable();
        glad::VertexAttribute(2)
            .pointer(2,
                     glad::DataType::Float,
                     false,
                     sizeof(CubeVertex),
                     reinterpret_cast<void*>(offsetof(CubeVertex, uv)))
            .enable();
        glad::VertexAttribute(3)
            .pointer(4,
                     glad::DataType::Float,
                     false,
                     sizeof(CubeVertex),
                     reinterpret_cast<void*>(offsetof(CubeVertex, tile_bounds)))
            .enable();


        glad::Unbind(mesh.vao);
    }

    ChunkMeshGeneratorObject::ChunkMeshGeneratorObject(const std::shared_ptr<Config>& config,
                                                       const std::shared_ptr<World>& world,
                                                       const std::shared_ptr<ChunkMeshGenerator>& chunk_mesh_generator)
        : config_(config),
          world_(world),
          chunk_mesh_generator_(chunk_mesh_generator),
          task_system_(std::make_unique<TaskSystem>(4)) {}

    void ChunkMeshGeneratorObject::update(float deltaTime) {
        {
            std::unique_lock lock(results_mutex_);
            while (!completed_meshes_.empty()) {
                auto& [chunk, mesh] = completed_meshes_.front();


                if (!chunk->mesh) chunk->mesh = std::make_unique<Mesh>();
                chunk->mesh->index_count = mesh.solidIndices.size();
                setup_mesh(*chunk->mesh, mesh.solidVertices, mesh.solidIndices);

                if (!mesh.transparentIndices.empty()) {
                    if (!chunk->transparent_mesh) chunk->transparent_mesh = std::make_unique<Mesh>();
                    chunk->transparent_mesh->index_count = mesh.transparentIndices.size();
                    setup_mesh(*chunk->transparent_mesh, mesh.transparentVertices, mesh.transparentIndices);
                }

                chunk->isDirty = false;
                chunk->isGeneratingMesh = false;
                completed_meshes_.pop();
            }
        }

        for (unsigned i = 0; i < config_->ChunkMeshesPerFrame; ++i) {
            if (world_->chunksToGenerateMesh.empty()) break;

            const auto& chunkIndex = world_->chunksToGenerateMesh.front();
            world_->chunksToGenerateMesh.pop_front();
            const auto& chunk = world_->chunks[chunkIndex];

            if (chunk->mesh && !chunk->isDirty) continue;
            chunk->isGeneratingMesh = true;

            task_system_->enqueue([this, chunk]() {
                auto data = chunk_mesh_generator_->Generate(chunk);

                std::unique_lock lock(results_mutex_);
                completed_meshes_.emplace(chunk, std::move(data));
            });
        }
    }
}
