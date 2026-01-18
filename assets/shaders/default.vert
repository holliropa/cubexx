#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;         // 0..Width, 0..Height
layout (location = 3) in vec4 aTileBounds; // MinU, MinV, MaxU, MaxV

out vec2 TexCoords;
out vec4 TileBounds;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    TexCoords = aUV;
    TileBounds = aTileBounds;

    // Transform the normal into world space (handles non-uniform scaling)
    Normal = mat3(transpose(inverse(model))) * aNormal;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
