#pragma once

#include <map>
#include <string>
#include <memory>
#include "texture2D.hpp"

class TextureManager {
public:
    static std::map<std::string, std::shared_ptr<Texture2D>> Texture2Ds;

    static std::shared_ptr<Texture2D> Get(const std::string &name);

    static void Clear();

private:
    TextureManager() {}
};