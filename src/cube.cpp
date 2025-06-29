#include "cube.hpp"

namespace
cubexx {
    // Define the cube vertices (positions and colors)
    float vertices[] = {
        // Positions           // Colors
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // Front face
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // Back face
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Left face
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, // Right face
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, // Bottom face
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, // Top face
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f
    };

    // Indices for drawing the cube using triangles
    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0, // Front face
        4, 5, 6, 6, 7, 4, // Back face
        8, 9, 10, 10, 11, 8, // Left face
        12, 13, 14, 14, 15, 12, // Right face
        16, 17, 18, 18, 19, 16, // Bottom face
        20, 21, 22, 22, 23, 20 // Top face
    };

    // Shaders
    auto vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
out vec3 fragColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() {
    fragColor = aColor;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
})";

    auto fragmentShaderSource = R"(
#version 330 core
in vec3 fragColor;
out vec4 FragColor;
void main() {
    FragColor = vec4(fragColor, 1.0);
})";


    Cube::Cube() {
        auto vertexShader = glad::VertexShader();
        vertexShader.set_source(vertexShaderSource);

        auto fragmentShader = glad::FragmentShader();
        fragmentShader.set_source(fragmentShaderSource);

        shaderProgram_.attach_shader(vertexShader, fragmentShader);
        shaderProgram_.link();

        glad::Bind(vertexArray_);

        arrayBuffer_.data(sizeof(vertices), vertices);
        elementArrayBuffer_.data(sizeof(indices), indices);

        glad::VertexAttribute(0)
            .pointer(3,
                     glad::DataType::Float,
                     false,
                     6 * sizeof(float))
            .enable();

        glad::VertexAttribute(1)
            .pointer(3,
                     glad::DataType::Float,
                     false,
                     6 * sizeof(float),
                     reinterpret_cast<void*>(3 * sizeof(float)))
            .enable();
    }

    void Cube::update(float deltaTime) {
        transform.rotation += glm::vec3(0.3f, 0.7f, 0.5f) * deltaTime * 90.0f;
    }

    void Cube::render(const bw::engine::Camera& camera) {
        auto view = camera.get_view();
        auto projection = camera.get_projection();

        glad::Bind(shaderProgram_);
        auto model_u = glad::UniformMat4(shaderProgram_, "model");
        auto view_u = glad::UniformMat4(shaderProgram_, "view");
        auto projection_u = glad::UniformMat4(shaderProgram_, "projection");

        model_u.set(glm::value_ptr(transform.getMatrix()));
        view_u.set(glm::value_ptr(view));
        projection_u.set(glm::value_ptr(projection));


        glad::Bind(vertexArray_);
        glad::DrawElements(glad::PrimitiveType::Triangles, 36, glad::IndexType::UnsignedInt);
    }
}
