#include <memory>
#include "game.hpp"
#include "shader_manager.hpp"
#include "mesh_manager.hpp"
#include "files.hpp"
#include "tiny_obj_loader.h"
#include "camera.hpp"
#include "camera_behaviour.hpp"
#include "texture_manager.hpp"
#include "gizmo_object.hpp"
#include "chunk_object.hpp"

void update_screen_size(unsigned int width, unsigned int height);

float m_widthF, m_heightF;


std::shared_ptr<Camera> m_mainCamera = std::make_shared<Camera>();
std::vector<std::shared_ptr<Object>> m_pObjects;

Game::~Game() {
    ShaderManager::Clear();
    MeshManager::Clear();
    TextureManager::Clear();
}

void Game::Initialize() {
    srand(time(nullptr));

    CameraBehaviour cameraBehaviour;
    cameraBehaviour.Camera = m_mainCamera;
    m_pObjects.emplace_back(std::make_shared<CameraBehaviour>(cameraBehaviour));
    m_pObjects.emplace_back(std::make_shared<ChunkObject>());

    GizmoObject gizmoObject;
    m_pObjects.emplace_back(std::make_shared<GizmoObject>(gizmoObject));
    for (auto &behaviour: m_pObjects) {
        behaviour->Initialize();
    }
}

void Game::Resize(unsigned int width, unsigned int height) {
    this->Width = width;
    this->Height = height;

    update_screen_size(width, height);
}

void Game::Update(float deltaTime) {
    if (Input::GetKeyDown(glfw::KeyCode::Escape))
        this->IsExit = true;

    for (auto &behaviour: m_pObjects) {
        behaviour->Update(deltaTime);
    }
}

void Game::Render() {
    for (auto &object: m_pObjects) {
        object->Render(*m_mainCamera);
    }
}

void update_screen_size(unsigned int width, unsigned int height) {
    m_widthF = static_cast<float>(width);
    m_heightF = static_cast<float>(height);

    m_mainCamera->m_plane.x = m_widthF;
    m_mainCamera->m_plane.y = m_heightF;
}