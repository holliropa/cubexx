#pragma once

#include <map>
#include <string>
#include <memory>
#include "shader.hpp"

class ShaderManager {
public:
    static std::map<std::string, std::shared_ptr<Shader>> Shaders;

    static std::shared_ptr<Shader> Get(const std::string &name);

    static void Clear();

private:
    ShaderManager() {}
};