#pragma once

#include <vector>
#include "attribute_layout.hpp"

#include "gl_common.hpp"

class Mesh {
public:
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> uvs;
    std::vector<GLuint> triangles;
    glad::PrimitiveType mode;
    GLuint VAO, VBO, EBO;
    std::vector<GLuint> instanceVBOs;

    Mesh();

    void upload();

    void clear();

    void draw();

    void DrawInstanced(int instanceCount);

    GLuint GenerateInstanceBuffer();

    void SetInstanceBufferAttributes(GLuint buffer,
                                     const std::vector<AttributeLayout> &layout);

    void PutInstanceBufferData(GLuint buffer,
                               GLsizei dataSize,
                               const void *data,
                               glad::BufferUsage usage);

    GLuint addInstanceBuffer(const std::vector<AttributeLayout> &layout,
                             GLsizei dataSize,
                             const void *data,
                             glad::BufferUsage usage);

    void updateInstanceBuffer(GLuint instanceVBO, GLsizei dataSize, const void *data);
};
