#pragma once

#include <memory>
#include <iostream>
#include "object.hpp"
#include "transform.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "tiny_obj_loader.h"
#include "mesh_manager.hpp"
#include "shader_manager.hpp"
#include "files.hpp"

class GizmoObject : public Object {
public:
    Transform m_transforms[3];
    glm::vec3 m_colors[3];
    std::shared_ptr<Shader> m_pShader;
    std::shared_ptr<Mesh> m_pArrowMesh;


    void Initialize() override {
//        tinyobj::attrib_t attrib;
//        std::vector<tinyobj::shape_t> shapes;
//        std::vector<tinyobj::material_t> materials;
//        std::string error;
//
//        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &error, "assets\\models\\arrow.obj")) {
//            std::cout << error << std::endl;
//        }

        std::vector<float> vertices{};
        std::vector<float> normals{};
        std::vector<float> uvs{};
        std::vector<unsigned int> triangles{};
//        for (unsigned int i = 0; i < shapes[0].mesh.indices.size(); ++i) {
//            const auto &index = shapes[0].mesh.indices[i];
//
//            vertices.push_back(attrib.vertices[(index.vertex_index * 3)]);
//            vertices.push_back(attrib.vertices[(index.vertex_index * 3) + 1]);
//            vertices.push_back(attrib.vertices[(index.vertex_index * 3) + 2]);
//
//            normals.push_back(attrib.normals[(index.normal_index * 3)]);
//            normals.push_back(attrib.normals[(index.normal_index * 3) + 1]);
//            normals.push_back(attrib.normals[(index.normal_index * 3) + 2]);
//
//            if (index.texcoord_index == -1) {
//                uvs.push_back(0.0f);
//                uvs.push_back(0.0f);
//            } else {
//                uvs.push_back(attrib.texcoords[(index.texcoord_index * 2)]);
//                uvs.push_back(attrib.texcoords[(index.texcoord_index * 2) + 1]);
//            }
//
//            triangles.push_back(i);
//        }

        vertices = {
                0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f,
        };
        normals = {
                0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f,
        };
        uvs = {
                0.0f, 0.0f,
                0.0f, 0.0f,
                0.0f, 0.0f,
        };
        triangles = {
                0, 1, 2
        };

        m_pArrowMesh = MeshManager::Get("arrow");
        m_pArrowMesh->vertices = vertices;
        m_pArrowMesh->normals = normals;
        m_pArrowMesh->uvs = uvs;
        m_pArrowMesh->triangles = triangles;
        m_pArrowMesh->mode = glad::PrimitiveType::kLines;
        m_pArrowMesh->upload();

        m_pShader = ShaderManager::Get("gizmo");
        auto gizmoVertexShaderFile = utils::readFile("assets/shaders/gizmo.vert");
        auto gizmoFragmentShaderFile = utils::readFile("assets/shaders/gizmo.frag");
        m_pShader->compile(gizmoVertexShaderFile.c_str(), gizmoFragmentShaderFile.c_str());

        m_colors[0] = glm::vec3(1.0f, 0.0f, 0.0f);
        m_transforms[0].rotation = glm::vec3(0.0f, 0.0f, -90.0f);

        m_colors[1] = glm::vec3(0.0f, 1.0f, 0.0f);

        m_colors[2] = glm::vec3(0.0f, 0.0f, 1.0f);
        m_transforms[2].rotation = glm::vec3(0.0f, 90.0f, 90.0f);

        for (auto &transform: m_transforms) {
            transform.scale.x = .2f;
            transform.scale.y = 3.0f;
            transform.scale.z = .2f;
        }
    }

    void Update(float deltaTime) override {
//        m_transforms[0].position.x += deltaTime;
//        m_transforms[1].position.y += deltaTime;
//        m_transforms[2].position.z += deltaTime;
    }

    void Render(const Camera &camera) const override {
        auto projection = camera.getProjection();
        auto view = camera.getView();

        m_pShader->use();
        m_pShader->setMatrix4fv("projection", projection);
        m_pShader->setMatrix4fv("view", view);

        glLineWidth(2.0f);
        for (int i = 0; i < 3; ++i) {
            m_pShader->setVec3f("color", m_colors[i]);
            m_pShader->setMatrix4fv("model", m_transforms[i].getMatrix());
            m_pArrowMesh->draw();
        }
        glLineWidth(1.0f);
    }
};