#include "gizmo.h"

#include "bw/engine/input.h"

namespace cubexx {
    auto gizmoArrowVertexShader = R"(
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 color;

void main() {
    color = aColor;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
})";

    auto gizmoArrowFragmentShader = R"(
#version 330 core

out vec4 FragColor;

in vec3 color;

void main() {
    FragColor = vec4(color, 1.0);
})";

    struct GizmoArrowVertex {
        glm::vec3 position;
        glm::vec3 color;
    };

    void Gizmo::init() {
        std::vector<GizmoArrowVertex> vertices;

        //X-axis
        vertices.emplace_back(GizmoArrowVertex{
            {0.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f}
        });
        vertices.emplace_back(GizmoArrowVertex{
            {1.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f}
        });

        //Y-axis
        vertices.emplace_back(GizmoArrowVertex{
            {0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f}
        });
        vertices.emplace_back(GizmoArrowVertex{
            {0.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f}
        });

        //Z-axis
        vertices.emplace_back(GizmoArrowVertex{
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f}
        });
        vertices.emplace_back(GizmoArrowVertex{
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f}
        });

        glad::Bind(vertexArray_);
        arrayBuffer_.data(sizeof(GizmoArrowVertex) * vertices.size(), vertices.data());

        glad::VertexAttribute(0)
            .pointer(3,
                     glad::DataType::Float,
                     false,
                     sizeof(GizmoArrowVertex))
            .enable();

        glad::VertexAttribute(1)
            .pointer(3,
                     glad::DataType::Float,
                     false,
                     sizeof(GizmoArrowVertex),
                     reinterpret_cast<void*>(offsetof(GizmoArrowVertex, color)))
            .enable();

        auto vertexShader = glad::VertexShader();
        vertexShader.set_source(gizmoArrowVertexShader);

        auto fragmentShader = glad::FragmentShader();
        fragmentShader.set_source(gizmoArrowFragmentShader);

        shaderProgram_.attach_shader(vertexShader, fragmentShader);
        shaderProgram_.link();
    }

    void Gizmo::update(float deltaTime) {
        if (bw::engine::Input::GetKeyDown(glfw::enums::KeyCode::G)) {
            enabled_ = !enabled_;
        }
    }

    void Gizmo::render(const bw::engine::Camera& camera) {
        if (!enabled_)
            return;

        glad::Bind(shaderProgram_);
        auto model_u = glad::UniformMat4(shaderProgram_, "model");
        auto view_u = glad::UniformMat4(shaderProgram_, "view");
        auto projection_u = glad::UniformMat4(shaderProgram_, "projection");


        auto model = glm::identity<glm::mat4>();
        model = glm::translate(model, glm::zero<glm::vec3>());
        model = glm::scale(model, glm::vec3(10000.0f));
        model_u.set(glm::value_ptr(model));
        view_u.set(glm::value_ptr(camera.get_view()));
        projection_u.set(glm::value_ptr(camera.get_projection()));

        glad::TemporaryLineWidth temporaryLineWidth(4.0f);
        glad::Bind(vertexArray_);
        glad::DrawArrays(glad::PrimitiveType::Lines, 12);
    }
}
