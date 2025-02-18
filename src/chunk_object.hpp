#pragma once

#include <memory>
#include "object.hpp"
#include "camera.hpp"
#include "mesh.hpp"
#include "mesh_manager.hpp"
#include "shader.hpp"
#include "texture2D.hpp"
#include "shader_manager.hpp"
#include "texture_manager.hpp"
#include "files.hpp"
#include "stb_image.h"

const glm::vec3 BLOCK_SIZE = glm::vec3(1.0f, 1.0f, 1.0f);
const unsigned int CHUNK_SIZE = 15;
const unsigned int CHUNK_SIZE_SQUARE = CHUNK_SIZE * CHUNK_SIZE;
const unsigned int CHUNK_SIZE_CUBE = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;


void make_top(
        const glm::vec3 &index,
        std::vector<float> &vertices,
        std::vector<float> &normals,
        std::vector<float> &uvs,
        std::vector<unsigned int> &triangles) {

    auto x = index.x;
    auto z = index.z;
    auto x_ = index.x + 1.0f;
    auto y_ = index.y + 1.0f;
    auto z_ = index.z + 1.0f;

    vertices.push_back(x);
    vertices.push_back(y_);
    vertices.push_back(z);

    vertices.push_back(x);
    vertices.push_back(y_);
    vertices.push_back(z_);

    vertices.push_back(x_);
    vertices.push_back(y_);
    vertices.push_back(z_);

    vertices.push_back(x);
    vertices.push_back(y_);
    vertices.push_back(z);

    vertices.push_back(x_);
    vertices.push_back(y_);
    vertices.push_back(z_);

    vertices.push_back(x_);
    vertices.push_back(y_);
    vertices.push_back(z);

    normals.push_back(0.0f);
    normals.push_back(1.0f);
    normals.push_back(0.0f);

    normals.push_back(0.0f);
    normals.push_back(1.0f);
    normals.push_back(0.0f);

    normals.push_back(0.0f);
    normals.push_back(1.0f);
    normals.push_back(0.0f);

    normals.push_back(0.0f);
    normals.push_back(1.0f);
    normals.push_back(0.0f);

    normals.push_back(0.0f);
    normals.push_back(1.0f);
    normals.push_back(0.0f);

    normals.push_back(0.0f);
    normals.push_back(1.0f);
    normals.push_back(0.0f);

    uvs.push_back(0.5f);
    uvs.push_back(0.0f);

    uvs.push_back(0.5f);
    uvs.push_back(1.0f);

    uvs.push_back(0.75f);
    uvs.push_back(1.0f);

    uvs.push_back(0.5f);
    uvs.push_back(0.0f);

    uvs.push_back(0.75f);
    uvs.push_back(1.0f);

    uvs.push_back(0.75f);
    uvs.push_back(0.0f);

    triangles.push_back(triangles.size());
    triangles.push_back(triangles.size());
    triangles.push_back(triangles.size());

    triangles.push_back(triangles.size());
    triangles.push_back(triangles.size());
    triangles.push_back(triangles.size());
}

void make_bottom(
        const glm::vec3 &index,
        std::vector<float> &vertices,
        std::vector<float> &normals,
        std::vector<float> &uvs,
        std::vector<unsigned int> &triangles) {

    auto x = index.x;
    auto y = index.y;
    auto z = index.z;
    auto x_ = index.x + 1.0f;
    auto z_ = index.z + 1.0f;

    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);

    vertices.push_back(x_);
    vertices.push_back(y);
    vertices.push_back(z_);

    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z_);

    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);

    vertices.push_back(x_);
    vertices.push_back(y);
    vertices.push_back(z);

    vertices.push_back(x_);
    vertices.push_back(y);
    vertices.push_back(z_);

    normals.push_back(0.0f);
    normals.push_back(-1.0f);
    normals.push_back(0.0f);

    normals.push_back(0.0f);
    normals.push_back(-1.0f);
    normals.push_back(0.0f);

    normals.push_back(0.0f);
    normals.push_back(-1.0f);
    normals.push_back(0.0f);

    normals.push_back(0.0f);
    normals.push_back(-1.0f);
    normals.push_back(0.0f);

    normals.push_back(0.0f);
    normals.push_back(-1.0f);
    normals.push_back(0.0f);

    normals.push_back(0.0f);
    normals.push_back(-1.0f);
    normals.push_back(0.0f);

    uvs.push_back(0.0f);
    uvs.push_back(0.0f);

    uvs.push_back(0.25f);
    uvs.push_back(1.0f);

    uvs.push_back(0.0f);
    uvs.push_back(1.0f);

    uvs.push_back(0.0f);
    uvs.push_back(0.0f);

    uvs.push_back(0.25f);
    uvs.push_back(0.0f);

    uvs.push_back(0.25f);
    uvs.push_back(1.0f);

    triangles.push_back(triangles.size());
    triangles.push_back(triangles.size());
    triangles.push_back(triangles.size());

    triangles.push_back(triangles.size());
    triangles.push_back(triangles.size());
    triangles.push_back(triangles.size());
}

void make_right(
        const glm::vec3 &index,
        std::vector<float> &vertices,
        std::vector<float> &normals,
        std::vector<float> &uvs,
        std::vector<unsigned int> &triangles) {

    auto y = index.y;
    auto z = index.z;
    auto x_ = index.x + 1.0f;
    auto y_ = index.y + 1.0f;
    auto z_ = index.z + 1.0f;

    vertices.push_back(x_);
    vertices.push_back(y);
    vertices.push_back(z);

    vertices.push_back(x_);
    vertices.push_back(y_);
    vertices.push_back(z);

    vertices.push_back(x_);
    vertices.push_back(y_);
    vertices.push_back(z_);

    vertices.push_back(x_);
    vertices.push_back(y);
    vertices.push_back(z);

    vertices.push_back(x_);
    vertices.push_back(y_);
    vertices.push_back(z_);

    vertices.push_back(x_);
    vertices.push_back(y);
    vertices.push_back(z_);

    normals.push_back(1.0f);
    normals.push_back(0.0f);
    normals.push_back(0.0f);

    normals.push_back(1.0f);
    normals.push_back(0.0f);
    normals.push_back(0.0f);

    normals.push_back(1.0f);
    normals.push_back(0.0f);
    normals.push_back(0.0f);

    normals.push_back(1.0f);
    normals.push_back(0.0f);
    normals.push_back(0.0f);

    normals.push_back(1.0f);
    normals.push_back(0.0f);
    normals.push_back(0.0f);

    normals.push_back(1.0f);
    normals.push_back(0.0f);
    normals.push_back(0.0f);

    uvs.push_back(0.25f);
    uvs.push_back(0.0f);

    uvs.push_back(0.25f);
    uvs.push_back(1.0f);

    uvs.push_back(0.5f);
    uvs.push_back(1.0f);

    uvs.push_back(0.25f);
    uvs.push_back(0.0f);

    uvs.push_back(0.5f);
    uvs.push_back(1.0f);

    uvs.push_back(0.5f);
    uvs.push_back(0.0f);

    triangles.push_back(triangles.size());
    triangles.push_back(triangles.size());
    triangles.push_back(triangles.size());

    triangles.push_back(triangles.size());
    triangles.push_back(triangles.size());
    triangles.push_back(triangles.size());
}

void make_left(
        const glm::vec3 &index,
        std::vector<float> &vertices,
        std::vector<float> &normals,
        std::vector<float> &uvs,
        std::vector<unsigned int> &triangles) {

    auto x = index.x;
    auto y = index.y;
    auto z = index.z;
    auto y_ = index.y + 1.0f;
    auto z_ = index.z + 1.0f;

    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z_);

    vertices.push_back(x);
    vertices.push_back(y_);
    vertices.push_back(z_);

    vertices.push_back(x);
    vertices.push_back(y_);
    vertices.push_back(z);

    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z_);

    vertices.push_back(x);
    vertices.push_back(y_);
    vertices.push_back(z);

    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);

    normals.push_back(-1.0f);
    normals.push_back(0.0f);
    normals.push_back(0.0f);

    normals.push_back(-1.0f);
    normals.push_back(0.0f);
    normals.push_back(0.0f);

    normals.push_back(-1.0f);
    normals.push_back(0.0f);
    normals.push_back(0.0f);

    normals.push_back(-1.0f);
    normals.push_back(0.0f);
    normals.push_back(0.0f);

    normals.push_back(-1.0f);
    normals.push_back(0.0f);
    normals.push_back(0.0f);

    normals.push_back(-1.0f);
    normals.push_back(0.0f);
    normals.push_back(0.0f);

    uvs.push_back(0.25f);
    uvs.push_back(0.0f);

    uvs.push_back(0.25f);
    uvs.push_back(1.0f);

    uvs.push_back(0.5f);
    uvs.push_back(1.0f);

    uvs.push_back(0.25f);
    uvs.push_back(0.0f);

    uvs.push_back(0.5f);
    uvs.push_back(1.0f);

    uvs.push_back(0.5f);
    uvs.push_back(0.0f);

    triangles.push_back(triangles.size());
    triangles.push_back(triangles.size());
    triangles.push_back(triangles.size());

    triangles.push_back(triangles.size());
    triangles.push_back(triangles.size());
    triangles.push_back(triangles.size());
}

void make_front(
        const glm::vec3 &index,
        std::vector<float> &vertices,
        std::vector<float> &normals,
        std::vector<float> &uvs,
        std::vector<unsigned int> &triangles) {

    auto x = index.x;
    auto y = index.y;
    auto x_ = index.x + 1.0f;
    auto y_ = index.y + 1.0f;
    auto z_ = index.z + 1.0f;

    vertices.push_back(x_);
    vertices.push_back(y);
    vertices.push_back(z_);

    vertices.push_back(x_);
    vertices.push_back(y_);
    vertices.push_back(z_);

    vertices.push_back(x);
    vertices.push_back(y_);
    vertices.push_back(z_);

    vertices.push_back(x_);
    vertices.push_back(y);
    vertices.push_back(z_);

    vertices.push_back(x);
    vertices.push_back(y_);
    vertices.push_back(z_);

    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z_);

    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(1.0f);

    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(1.0f);

    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(1.0f);

    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(1.0f);

    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(1.0f);

    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(1.0f);

    uvs.push_back(0.25f);
    uvs.push_back(0.0f);

    uvs.push_back(0.25f);
    uvs.push_back(1.0f);

    uvs.push_back(0.5f);
    uvs.push_back(1.0f);

    uvs.push_back(0.25f);
    uvs.push_back(0.0f);

    uvs.push_back(0.5f);
    uvs.push_back(1.0f);

    uvs.push_back(0.5f);
    uvs.push_back(0.0f);

    triangles.push_back(triangles.size());
    triangles.push_back(triangles.size());
    triangles.push_back(triangles.size());

    triangles.push_back(triangles.size());
    triangles.push_back(triangles.size());
    triangles.push_back(triangles.size());
}

void make_back(
        const glm::vec3 &index,
        std::vector<float> &vertices,
        std::vector<float> &normals,
        std::vector<float> &uvs,
        std::vector<unsigned int> &triangles) {

    auto x = index.x;
    auto y = index.y;
    auto z = index.z;
    auto x_ = index.x + 1.0f;
    auto y_ = index.y + 1.0f;

    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);

    vertices.push_back(x);
    vertices.push_back(y_);
    vertices.push_back(z);

    vertices.push_back(x_);
    vertices.push_back(y_);
    vertices.push_back(z);

    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);

    vertices.push_back(x_);
    vertices.push_back(y_);
    vertices.push_back(z);

    vertices.push_back(x_);
    vertices.push_back(y);
    vertices.push_back(z);

    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(-1.0f);

    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(-1.0f);

    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(-1.0f);

    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(-1.0f);

    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(-1.0f);

    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(-1.0f);

    uvs.push_back(0.25f);
    uvs.push_back(0.0f);

    uvs.push_back(0.25f);
    uvs.push_back(1.0f);

    uvs.push_back(0.5f);
    uvs.push_back(1.0f);

    uvs.push_back(0.25f);
    uvs.push_back(0.0f);

    uvs.push_back(0.5f);
    uvs.push_back(1.0f);

    uvs.push_back(0.5f);
    uvs.push_back(0.0f);

    triangles.push_back(triangles.size());
    triangles.push_back(triangles.size());
    triangles.push_back(triangles.size());

    triangles.push_back(triangles.size());
    triangles.push_back(triangles.size());
    triangles.push_back(triangles.size());
}

class ChunkObject : public Object {
public:
    Transform transform;

public:
    void Initialize() override {
        auto default_vsFileContent = utils::readFile("assets/shaders/default.vert");
        auto default_fsFileContent = utils::readFile("assets/shaders/default.frag");

        m_pShader = ShaderManager::Get("default");
        m_pShader->compile(default_vsFileContent.c_str(), default_fsFileContent.c_str());


        m_pTexture = TextureManager::Get("block");
        m_pTexture->Internal_Format = GL_RGBA;
        m_pTexture->Image_Format = GL_RGBA;

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load("assets/textures/block.png", &width, &height, &nrChannels, 4);
        m_pTexture->Generate(width, height, data);
        // and finally free image data
        stbi_image_free(data);


        m_mesh = MeshManager::Get("chunk");
        m_pShader = ShaderManager::Get("default");
    }

    void Update(float deltaTime) override {

        if (Input::GetKeyDown(glfw::KeyCode::G)) {
            generate();
        }
    }

    void Render(const Camera &camera) const override {
        auto projection = camera.getProjection();
        auto view = camera.getView();

        m_pShader->use();
        m_pShader->setMatrix4fv("projection", projection);
        m_pShader->setMatrix4fv("view", view);

        m_pTexture->Bind();

        auto model = transform.getMatrix();
        m_pShader->setMatrix4fv("model", model);
        m_mesh->draw();
    }


private:
    void generate() {
        m_cubes.clear();
        m_mesh->clear();

        for (int y = 0; y < CHUNK_SIZE; ++y) {
            for (int x = 0; x < CHUNK_SIZE; ++x) {
                for (int z = 0; z < CHUNK_SIZE; ++z) {
                    if (rand() % 2 == 0)
                        continue;

                    auto index = glm::ivec3(x, y, z);
                    m_cubes.push_back(index);
                }
            }
        }

        std::vector<float> vertices{};
        std::vector<float> normals{};
        std::vector<float> uvs{};
        std::vector<unsigned int> triangles{};

        for (const auto &cube: m_cubes) {
            if (isEmpty(cube.x, cube.y + 1, cube.z))
                make_top(cube, vertices, normals, uvs, triangles);
            if (isEmpty(cube.x, cube.y - 1, cube.z))
                make_bottom(cube, vertices, normals, uvs, triangles);
            if (isEmpty(cube.x + 1, cube.y, cube.z))
                make_right(cube, vertices, normals, uvs, triangles);
            if (isEmpty(cube.x - 1, cube.y, cube.z))
                make_left(cube, vertices, normals, uvs, triangles);
            if (isEmpty(cube.x, cube.y, cube.z + 1))
                make_front(cube, vertices, normals, uvs, triangles);
            if (isEmpty(cube.x, cube.y, cube.z - 1))
                make_back(cube, vertices, normals, uvs, triangles);
        }
        m_mesh->vertices = vertices;
        m_mesh->normals = normals;
        m_mesh->uvs = uvs;
        m_mesh->triangles = triangles;
        m_mesh->mode = glad::PrimitiveType::kTriangles;
        m_mesh->upload();
    }

    bool isEmpty(unsigned int x, unsigned int y, unsigned int z) {
        for (const auto &block: m_cubes) {
            if (block.x == x && block.y == y && block.z == z) {
                return false;
            }
        }

        return true;
    }

private:
    std::vector<glm::ivec3> m_cubes;
    std::shared_ptr<Mesh> m_mesh;
    std::shared_ptr<Shader> m_pShader;
    std::shared_ptr<Texture2D> m_pTexture;
};