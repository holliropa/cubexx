#pragma once
#include <memory>

#include "../world.h"
#include "bw/engine/base_object.h"

namespace cubexx {
    class BlockOutlineObject final : public bw::engine::BaseObject {
    private:
        std::string vertexShaderSource = R"(
            #version 330 core
            layout(location = 0) in vec3 aPos;
            uniform mat4 model, view, projection;
            void main() {
                gl_Position = projection * view * model * vec4(aPos, 1.0);
        })";

        std::string fragmentShaderSource = R"(
            #version 330 core
            out vec4 FragColor;
            void main() {
                FragColor = vec4(1.0, 1.0, 0.0, 1.0); // yellow outline
        })";

    public:
        explicit BlockOutlineObject(const std::shared_ptr<World>& world)
            : world_(world) {}

        void init() override {
            auto vertexShader = glad::VertexShader();
            vertexShader.set_source(vertexShaderSource);

            auto fragmentShader = glad::FragmentShader();
            fragmentShader.set_source(fragmentShaderSource);

            shaderProgram_.attach_shader(vertexShader, fragmentShader);
            shaderProgram_.link();

            const std::vector<glm::vec3> vertices = {
                {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}, // Front
                {0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1} // Back
            };

            const std::vector<uint32_t> indices = {
                0, 1, 1, 2, 2, 3, 3, 0, // Front face edges
                4, 5, 5, 6, 6, 7, 7, 4, // Back face edges
                0, 4, 1, 5, 2, 6, 3, 7 // Side edges
            };

            Bind(mesh_.vao);
            mesh_.vbo.data(vertices);
            mesh_.ebo.data(indices);
            mesh_.index_count = static_cast<GLsizei>(indices.size());

            glad::VertexAttribute(0)
                .pointer(3, glad::DataType::Float, false, sizeof(glm::vec3))
                .enable();
        }

        void render(const bw::engine::Camera& camera) override {
            if (!world_->selectedBlock.has_value())
                return;

            glad::Enable(glad::Capability::DepthTest);
            glad::Enable(glad::Capability::PolygonOffsetLine);
            glad::PolygonOffset(-3.0f, -1.0f);
            const auto temp_poly_mode = glad::TemporaryPolygonMode(glad::PolyMode::Line);
            glad::LineWidth(5.0f);

            glad::Bind(shaderProgram_);

            const auto model = glm::translate(glm::mat4(1.0f), glm::vec3(world_->selectedBlock.value()));
            glad::UniformMat4(shaderProgram_, "model").set(glm::value_ptr(model));
            glad::UniformMat4(shaderProgram_, "view").set(glm::value_ptr(camera.get_view()));
            glad::UniformMat4(shaderProgram_, "projection").set(glm::value_ptr(camera.get_projection()));

            glad::Bind(mesh_.vao);
            glad::DrawElements(glad::PrimitiveType::Lines, mesh_.index_count, glad::IndexType::UnsignedInt);

            glad::Disable(glad::Capability::PolygonOffsetLine);
        }

    private:
        std::shared_ptr<World> world_;
        Mesh mesh_;
        glad::Program shaderProgram_;
    };
}
