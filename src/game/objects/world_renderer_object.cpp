#include "world_renderer_object.h"

#include <iostream>

#include "stb_image.h"
#include "../../core/file_utils.h"
#include "../../core/frustum.h"
#include "bw/engine/input.h"

namespace cubexx {
    WorldRendererObject::WorldRendererObject(const std::shared_ptr<World>& world,
                                             const std::shared_ptr<TextureManager>& texture_manager,
                                             const std::shared_ptr<Camera>& camera)
        : world_(world), texture_manager_(texture_manager), camera_(camera) {}

    void WorldRendererObject::init() {
        init_shader();
    }

    void WorldRendererObject::update(float deltaTime) {
        if (bw::engine::Input::GetKeyDown(glfw::KeyCode::P)) {
            std::cout << "PolyMode: " << (isPolyMode ? "ON" : "OFF") << std::endl;
            isPolyMode = !isPolyMode;
        }
    }

    void WorldRendererObject::render(const bw::engine::Camera& camera) {
        auto view = camera.get_view();
        auto projection = camera.get_projection();
        const auto frustum = Frustum::fromViewProjection(projection * view);

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
        glad::Bind(texture_manager_->get_texture());

        constexpr int location = 0;
        glad::UniformInt(shaderProgram_, "texture1").set(&location);

        view_u.set(glm::value_ptr(view));
        projection_u.set(glm::value_ptr(projection));

        glad::TemporaryPolygonMode polygon_mode(isPolyMode ? glad::PolyMode::Line : glad::PolyMode::Fill);
        // glad::TemporaryCapability cullFaceOff(glad::Capability::CullFace, false);

        std::vector<std::shared_ptr<Chunk>> opaqueDraw;
        std::vector<std::shared_ptr<Chunk>> transparentDraw;

        {
            std::deque<std::shared_ptr<Chunk>> queue;
            std::unordered_set<glm::ivec3> visited;

            glm::ivec3 camIndex = glm::floor(camera_->transform.position / static_cast<float>(CHUNK_SIZE));
            if (world_->chunks.contains(camIndex)) {
                queue.push_back(world_->chunks.at(camIndex));
                visited.insert(camIndex);
            }

            while (!queue.empty()) {
                auto chunk = queue.front();
                queue.pop_front();

                glm::vec3 min = glm::vec3(chunk->index) * static_cast<float>(CHUNK_SIZE);
                glm::vec3 max = min + glm::vec3(CHUNK_SIZE);

                if (!frustum.isBoxVisible(min, max)) {
                    continue;
                }

                if (chunk->mesh) {
                    opaqueDraw.push_back(chunk);
                }
                if (chunk->transparent_mesh) {
                    transparentDraw.push_back(chunk);
                }

                if (chunk->data.isOpaque) {
                    continue;
                }

                for (const auto& neighbor : chunk->neighbors) {
                    if (!neighbor) continue;
                    if (visited.contains(neighbor->index)) continue;

                    visited.insert(neighbor->index);
                    queue.push_back(neighbor);
                }
            }
        }

        for (const auto& chunk : opaqueDraw) {
            auto model = transform_.getMatrix();
            model = glm::translate(model, glm::vec3(chunk->index) * static_cast<float>(CHUNK_SIZE));
            model_u.set(glm::value_ptr(model));

            glad::Bind(chunk->mesh->vao);
            glad::DrawElements(glad::PrimitiveType::Triangles,
                               chunk->mesh->index_count,
                               glad::IndexType::UnsignedInt);
        }

        const auto camPosition = camera_->transform.position;
        std::sort(transparentDraw.begin(), transparentDraw.end(),
                  [&](const auto& a, const auto& b) {
                      const glm::vec3 aCenter = (glm::vec3(a->index) * static_cast<float>(CHUNK_SIZE))
                          + glm::vec3(static_cast<float>(CHUNK_SIZE) * 0.5f);
                      const glm::vec3 bCenter = (glm::vec3(a->index) * static_cast<float>(CHUNK_SIZE))
                          + glm::vec3(static_cast<float>(CHUNK_SIZE) * 0.5f);

                      const float da = glm::length2(aCenter - camPosition);
                      const float db = glm::length2(bCenter - camPosition);
                      return da < db;
                  });

        for (const auto& chunk : transparentDraw) {
            auto model = transform_.getMatrix();
            model = glm::translate(model, glm::vec3(chunk->index) * static_cast<float>(CHUNK_SIZE));
            model_u.set(glm::value_ptr(model));

            glad::Bind(chunk->transparent_mesh->vao);
            glad::DrawElements(glad::PrimitiveType::Triangles,
                               chunk->transparent_mesh->index_count,
                               glad::IndexType::UnsignedInt);
        }
    }

    void WorldRendererObject::init_shader() {
        auto vertexShader = glad::VertexShader();
        const auto vSource = loadFile("assets/shaders/default.vert");
        vertexShader.set_source(vSource);

        auto fragmentShader = glad::FragmentShader();
        const auto fSource = loadFile("assets/shaders/default.frag");
        fragmentShader.set_source(fSource);

        shaderProgram_.attach_shader(vertexShader, fragmentShader);
        shaderProgram_.link();
    }
}
