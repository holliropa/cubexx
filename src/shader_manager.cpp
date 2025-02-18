#include <stdexcept>
#include "shader_manager.hpp"

std::map<std::string, std::shared_ptr<Shader>> ShaderManager::Shaders;

std::shared_ptr<Shader> ShaderManager::Get(const std::string &name) {
    if (Shaders.find(name) == Shaders.end())
        Shaders[name] = std::make_shared<Shader>();

    return Shaders[name];
}

void ShaderManager::Clear() {
    for (const auto &iter: Shaders) {
        glDeleteProgram(iter.second->ID);
    }

    Shaders.clear();
}
