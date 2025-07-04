#include "world_renderer_object.h"

#include <iostream>

#include "stb_image.h"

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

    WorldRendererObject::WorldRendererObject(const std::shared_ptr<World>& world)
        : world_(world) {}

    void WorldRendererObject::init() {
        init_shader();
        init_texture();
    }

    void WorldRendererObject::render(const bw::engine::Camera& camera) {
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

        constexpr int location = 0;
        glad::UniformInt(shaderProgram_, "texture1").set(&location);

        view_u.set(glm::value_ptr(view));
        projection_u.set(glm::value_ptr(projection));

        for (const auto& chunkIndex : world_->visibleChunks) {
            if (world_->chunks.find(chunkIndex) == world_->chunks.end()) {
                continue;
            }

            const auto& chunk = world_->chunks.at(chunkIndex);

            if (!chunk->meshData) {
                continue;
            }

            const auto& meshData = chunk->meshData;

            auto model = transform_.getMatrix();
            model = glm::translate(model, glm::vec3(chunk->index) * static_cast<float>(CHUNK_SIZE));
            model_u.set(glm::value_ptr(model));

            glad::Bind(meshData->vertexArray);
            glad::DrawElements(glad::PrimitiveType::Triangles,
                               meshData->index_count,
                               glad::IndexType::UnsignedInt);
        }
    }

    void WorldRendererObject::init_shader() {
        auto vertexShader = glad::VertexShader();
        vertexShader.set_source(worldVertexShaderSource);

        auto fragmentShader = glad::FragmentShader();
        fragmentShader.set_source(worldFragmentShaderSource);

        shaderProgram_.attach_shader(vertexShader, fragmentShader);
        shaderProgram_.link();
    }

    void WorldRendererObject::init_texture() {
        int width, height, nrComponents;
        stbi_set_flip_vertically_on_load(1);
        if (const auto data = stbi_load("assets/textures/default_block.png", &width, &height, &nrComponents, 0)) {
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
}
