#pragma once
#include "bw/engine/base_object.h"
#include "bw/engine/gl.h"

namespace cubexx {
    class CrosshairObject final : public bw::engine::BaseObject {
    public:
        void init() override {
            constexpr GLfloat vertices[] = {
                -0.01f, 0.0f, // horizontal line
                0.01f, 0.0f,
                0.0f, -0.01f, // vertical line
                0.0f, 0.01f,
            };

            glad::Bind(mesh_.vao);
            mesh_.vbo.data(sizeof(vertices), vertices);

            glad::VertexAttribute(0)
                .pointer(2, glad::DataType::Float, false, 0)
                .enable();

            auto vertexShader = glad::VertexShader();
            vertexShader.set_source(R"(
                #version 330 core
                layout(location = 0) in vec2 aPos;

                uniform mat4 projection;

                void main() {
                    gl_Position = projection * vec4(aPos, 0.0, 1.0);
                })");

            auto fragmentShader = glad::FragmentShader();
            fragmentShader.set_source(R"(
                #version 330 core
                out vec4 FragColor;
                void main(){
                    FragColor = vec4(1.0);
                })");

            shaderProgram_.attach_shader(vertexShader, fragmentShader);
            shaderProgram_.link();
        }

        void render(const bw::engine::Camera& camera) override {
            const auto& game_camera = dynamic_cast<const Camera&>(camera);
            const float aspect = game_camera.m_plane.x / game_camera.m_plane.y;

            glad::Bind(shaderProgram_);
            glad::Bind(mesh_.vao);

            glm::mat4 projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f);
            glad::UniformMat4(shaderProgram_, "projection").set(glm::value_ptr(projection));

            glad::DrawArrays(glad::PrimitiveType::Lines, 4);
        }

    private:
        glad::Program shaderProgram_;
        Mesh mesh_;
    };
}
