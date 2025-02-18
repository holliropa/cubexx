#include <stdexcept>
#include "texture_manager.hpp"
#include "gl_common.hpp"
#include "stb_image.h"

std::map<std::string, std::shared_ptr<Texture2D>> TextureManager::Texture2Ds;

std::shared_ptr<Texture2D> TextureManager::Get(const std::string &name) {
    if (Texture2Ds.find(name) == Texture2Ds.end())
        Texture2Ds[name] = std::make_shared<Texture2D>();

    return Texture2Ds[name];
}

void TextureManager::Clear() {
    for (const auto &iter: Texture2Ds) {
        glDeleteTextures(1, &iter.second->ID);
    }
}
