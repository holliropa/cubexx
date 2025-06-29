#include <iostream>
#include <deque>
#include <unordered_set>
#include <future>
#include <algorithm>

#include "world.hpp"
#include "stb_image.h"
#include "bw/engine/input.h"

namespace cubexx {
    auto worldVertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec2 TryTestCoords;

void main() {
    TryTestCoords = aTexCoord;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Normal = mat3(transpose(inverse(model))) * aNormal;
})";

    auto worldFragmentShaderSource = R"(
#version 330 core
in vec3 Normal;
in vec2 TryTestCoords;

out vec4 FragColor;

uniform sampler2D texture1;
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 ambientColor;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDirNorm = normalize(-lightDir);

    float diff = max(dot(norm, lightDirNorm), 0.0);
    vec3 diffuse = diff * lightColor;

    vec4 texColor = texture(texture1, TryTestCoords);
    vec3 result = (ambientColor + diffuse) * texColor.rgb;

    FragColor = vec4(result, texColor.a);
})";
    constexpr unsigned CHUNKS_PER_FRAME = 7;

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

    World::World(const std::shared_ptr<Camera>& camera)
        : camera_(camera) {}

    void World::init() {
        auto vertexShader = glad::VertexShader();
        vertexShader.set_source(worldVertexShaderSource);

        auto fragmentShader = glad::FragmentShader();
        fragmentShader.set_source(worldFragmentShaderSource);

        shaderProgram_.attach_shader(vertexShader, fragmentShader);
        shaderProgram_.link();

        int width, height, nrComponents;
        stbi_set_flip_vertically_on_load(1);
        const auto data = stbi_load("assets/textures/default_block.png", &width, &height, &nrComponents, 0);
        if (data) {
            glad::Bind(texture_);
            texture_.upload(0, glad::PixelDataInternalFormat::RGBA, width, height, glad::PixelDataFormat::RGBA,
                            glad::PixelDataType::UnsignedByte, data);
            texture_.generateMipmap();

            texture_.wrapS(glad::WrapMode::Repeat);
            texture_.wrapT(glad::WrapMode::Repeat);
            texture_.minFilter(glad::MinFilter::Nearest);
            texture_.magFilter(glad::MagFilter::Nearest);

            stbi_image_free(data);
        }
        else {
            std::cerr << "Failed to load texture\n";
            stbi_image_free(data);
        }
    }

    void World::update(float deltaTime) {
        if (bw::engine::Input::GetKeyDown(glfw::KeyCode::T)) {
            enabled_ = !enabled_;
        }

        if (check_) {
            // Populate loading queue with new chunks
            for (const auto& chunkPos : visibleChunks_) {
                if (pChunks_.find(chunkPos) == pChunks_.end() &&
                    queuedChunks_.find(chunkPos) == queuedChunks_.end()) {
                    loadingQueue_.push_back(chunkPos);
                    queuedChunks_.insert(chunkPos);
                }
            }

            // Sort chunks by distance to player (closest first)
            std::sort(loadingQueue_.begin(), loadingQueue_.end(),
                      [this](const glm::ivec3& a, const glm::ivec3& b) {
                          auto distA = glm::distance2(glm::vec3(a), glm::vec3(userChunkIndex_));
                          auto distB = glm::distance2(glm::vec3(b), glm::vec3(userChunkIndex_));
                          return distA < distB;
                      });

            check_ = false;
        }

        // Process chunks from loading queue
        for (unsigned i = 0; i < CHUNKS_PER_FRAME && !loadingQueue_.empty(); ++i) {
            auto chunkPos = loadingQueue_.front();
            loadingQueue_.pop_front();
            queuedChunks_.erase(chunkPos);

            auto chunk = std::make_shared<Chunk>();
            chunk->position = chunkPos;
            worldGenerator_.Generate(chunk);
            if (!chunk->isEmpty()) {
                chunkMeshGenerator_.Generate(chunk);
            }
            pChunks_[chunkPos] = chunk;
        }


        if (auto currentChunkIndex = glm::ivec3(glm::floor(camera_->transform.position * (1.0f / Chunk::SIZE)));
            currentChunkIndex != userChunkIndex_ && enabled_) {
            userChunkIndex_ = currentChunkIndex;
            visibleChunks_ = getSpherePoints(userChunkIndex_, 10);
            check_ = true;
        }
    }

    void World::render(const bw::engine::Camera& camera) {
        auto view = camera.get_view();
        auto projection = camera.get_projection();

        glad::Bind(shaderProgram_);
        auto model_u = glad::UniformMat4(shaderProgram_, "model");
        auto view_u = glad::UniformMat4(shaderProgram_, "view");
        auto projection_u = glad::UniformMat4(shaderProgram_, "projection");

        // Set lighting uniforms
        auto lightDirUniform = glad::UniformVec3(shaderProgram_, "lightDir");
        auto lightColorUniform = glad::UniformVec3(shaderProgram_, "lightColor");
        auto ambientColorUniform = glad::UniformVec3(shaderProgram_, "ambientColor");

        constexpr glm::vec3 lightDirection(-0.2f, -1.0f, -0.3f);
        constexpr glm::vec3 lightColor(0.8f, 0.8f, 0.8f);
        constexpr glm::vec3 ambient(0.2f, 0.2f, 0.2f);

        lightDirUniform.set(glm::value_ptr(lightDirection));
        lightColorUniform.set(glm::value_ptr(lightColor));
        ambientColorUniform.set(glm::value_ptr(ambient));

        glad::ActiveTextureUnit(glad::TextureUnit::Unit0);
        glad::Bind(texture_);

        int location = 0;
        glad::UniformInt(shaderProgram_, "texture1").set(&location);

        view_u.set(glm::value_ptr(view));
        projection_u.set(glm::value_ptr(projection));

        for (const auto& chunkIndex : visibleChunks_) {
            auto chunkEntry = pChunks_.find(chunkIndex);
            if (chunkEntry == pChunks_.end())
                continue;

            auto& chunk = pChunks_.find(chunkIndex)->second;

            if (chunk->isEmpty())
                continue;

            auto model = glm::translate(transform_.getMatrix(),
                                        glm::vec3(chunkIndex) * static_cast<float>(Chunk::SIZE));

            model_u.set(glm::value_ptr(model));

            glad::Bind(chunk->vertexArray);
            glad::DrawElements(glad::PrimitiveType::Triangles,
                               chunk->index_count,
                               glad::IndexType::UnsignedInt);
        }
    }
}
