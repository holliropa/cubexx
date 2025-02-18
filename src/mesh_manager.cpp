#include <stdexcept>
#include "mesh_manager.hpp"

std::map<std::string, std::shared_ptr<Mesh>> MeshManager::Meshes;

std::shared_ptr<Mesh> MeshManager::Get(const std::string &name) {
    if (Meshes.find(name) == Meshes.end())
        Meshes[name] = std::make_shared<Mesh>();

    return Meshes[name];
}

void MeshManager::Clear() {
    for (const auto &iter: Meshes) {
        auto mesh = iter.second; // Reference to the current mesh

        glDeleteVertexArrays(1, &mesh->VAO);
        glDeleteBuffers(1, &mesh->VBO);
        glDeleteBuffers(1, &mesh->EBO);
    }

    Meshes.clear();
}
