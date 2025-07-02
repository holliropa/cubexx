#include <iostream>
#include "grid.h"

#include "bw/engine/input.h"

namespace cubexx {
    auto gridVertexShader = R"(
#version 330

out vec3 WorldPos;

uniform mat4 view = mat4(1.0);
uniform mat4 projection = mat4(1.0);
uniform float gGridSize = 100.0;
uniform vec3 gCameraWorldPos;

const vec3 Pos[4] = vec3[4](
    vec3(-1.0, 0.0, -1.0),      // bottom left
    vec3( 1.0, 0.0, -1.0),      // bottom right
    vec3( 1.0, 0.0,  1.0),      // top right
    vec3(-1.0, 0.0,  1.0)       // top left
);

const int Indices[6] = int[6](0, 2, 1, 2, 0, 3);

void main()
{
    int Index = Indices[gl_VertexID];
    vec3 vPos3 = Pos[Index] * gGridSize;

    vPos3.x += gCameraWorldPos.x;
    vPos3.z += gCameraWorldPos.z;

    vec4 vPos4 = vec4(vPos3, 1.0);

    gl_Position = projection * view * vPos4;

    WorldPos = vPos3;
})";

    auto gridFragmentShader = R"(
#version 330

in vec3 WorldPos;

layout(location = 0) out vec4 FragColor;

uniform vec3 gCameraWorldPos;
uniform float gGridSize = 100.0;
uniform float gGridMinPixelsBetweenCells = 2.0;
uniform float gGridCellSize = 0.025;
uniform vec4 gGridColorThin = vec4(0.5, 0.5, 0.5, 1.0);
uniform vec4 gGridColorThick = vec4(0.0, 0.0, 0.0, 1.0);


float log10(float x)
{
    float f = log(x) / log(10.0);
    return f;
}


float satf(float x)
{
    float f = clamp(x, 0.0, 1.0);
    return f;
}


vec2 satv(vec2 x)
{
    vec2 v = clamp(x, vec2(0.0), vec2(1.0));
    return v;
}


float max2(vec2 v)
{
    float f = max(v.x, v.y);
    return f;
}


void main()
{
    vec2 dvx = vec2(dFdx(WorldPos.x), dFdy(WorldPos.x));
    vec2 dvy = vec2(dFdx(WorldPos.z), dFdy(WorldPos.z));

    float lx = length(dvx);
    float ly = length(dvy);

    vec2 dudv = vec2(lx, ly);

    float l = length(dudv);

    float LOD = max(0.0, log10(l * gGridMinPixelsBetweenCells / gGridCellSize) + 1.0);

    float GridCellSizeLod0 = gGridCellSize * pow(10.0, floor(LOD));
    float GridCellSizeLod1 = GridCellSizeLod0 * 10.0;
    float GridCellSizeLod2 = GridCellSizeLod1 * 10.0;

    dudv *= 4.0;

    vec2 mod_div_dudv = mod(WorldPos.xz, GridCellSizeLod0) / dudv;
    float Lod0a = max2(vec2(1.0) - abs(satv(mod_div_dudv) * 2.0 - vec2(1.0)) );

    mod_div_dudv = mod(WorldPos.xz, GridCellSizeLod1) / dudv;
    float Lod1a = max2(vec2(1.0) - abs(satv(mod_div_dudv) * 2.0 - vec2(1.0)) );

    mod_div_dudv = mod(WorldPos.xz, GridCellSizeLod2) / dudv;
    float Lod2a = max2(vec2(1.0) - abs(satv(mod_div_dudv) * 2.0 - vec2(1.0)) );

    float LOD_fade = fract(LOD);
    vec4 Color;

    if (Lod2a > 0.0) {
        Color = gGridColorThick;
        Color.a *= Lod2a;
    } else {
        if (Lod1a > 0.0) {
            Color = mix(gGridColorThick, gGridColorThin, LOD_fade);
	        Color.a *= Lod1a;
        } else {
            Color = gGridColorThin;
	        Color.a *= (Lod0a * (1.0 - LOD_fade));
        }
    }

    float OpacityFalloff = (1.0 - satf(length(WorldPos.xz - gCameraWorldPos.xz) / gGridSize));

    Color.a *= OpacityFalloff;

    FragColor = Color;
})";

    Grid::Grid(const std::shared_ptr<Camera>& camera)
        : camera_(camera) {}

    void Grid::init() {
        auto vertexShader = glad::VertexShader();
        vertexShader.set_source(gridVertexShader);

        auto fragmentShader = glad::FragmentShader();
        fragmentShader.set_source(gridFragmentShader);

        shaderProgram_.attach_shader(vertexShader, fragmentShader);
        shaderProgram_.link();
    }

    void Grid::update(float deltaTime) {
        if (bw::engine::Input::GetKeyDown(glfw::KeyCode::K)) {
            enabled_ = !enabled_;
        }
    }

    void Grid::render(const bw::engine::Camera& camera) {
        if (!enabled_)
            return;

        glad::Bind(shaderProgram_);
        auto view_u = glad::UniformMat4(shaderProgram_, "view");
        auto projection_u = glad::UniformMat4(shaderProgram_, "projection");
        auto cameraWorldPos_u = glad::UniformVec3(shaderProgram_, "gCameraWorldPos");

        view_u.set(glm::value_ptr(camera.get_view()));
        projection_u.set(glm::value_ptr(camera.get_projection()));
        cameraWorldPos_u.set(glm::value_ptr(camera_->transform.position));

        glad::DrawArrays(glad::PrimitiveType::Triangles, 6);
        std::cout << glGetError() << std::endl;
    }
}
