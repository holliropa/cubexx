#pragma once
#include <iostream>

#include "stb_image.h"
#include "bw/engine/gl.h"

namespace cubexx {
    class TextureManager {
    public:
        void load_texture(const std::string& path) {
            int width, height, nrComponents;
            stbi_set_flip_vertically_on_load(1);
            if (const auto data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0)) {
                glad::Bind(texture_);
                texture_.upload(0, glad::PixelDataInternalFormat::RGBA, width, height, glad::PixelDataFormat::RGBA,
                                glad::PixelDataType::UnsignedByte, data);
                texture_.generateMipmap();

                texture_.wrapS(glad::WrapMode::Repeat);
                texture_.wrapT(glad::WrapMode::Repeat);
                texture_.minFilter(glad::MinFilter::Nearest);
                texture_.magFilter(glad::MagFilter::Nearest);

                stbi_image_free(data);
            }
            else {
                std::cerr << "Failed to load texture\n";
                stbi_image_free(data);
            }
        }

        [[nodiscard]] const glad::Texture2D& get_texture() const { return texture_; }

        std::array<glm::vec2, 4> get_tile_uv(const size_t tile_index) {
            constexpr float tile_size = 32.0f;
            constexpr float atlas_size = 256.0f;
            constexpr auto tiles_per_row = static_cast<size_t>(atlas_size / tile_size);

            const auto row = tile_index / tiles_per_row;
            const auto col = tile_index % tiles_per_row;

            const float u0 = (static_cast<float>(col) * tile_size) / atlas_size;
            const float v0 = 1.0f - (static_cast<float>(row) * tile_size) - (tile_size / atlas_size);
            const float u1 = (static_cast<float>(col + 1) * tile_size) / atlas_size;
            const float v1 = 1.0f - (static_cast<float>(row) * tile_size) / atlas_size;

            return {
                glm::vec2(u0, v0),
                glm::vec2(u1, v0),
                glm::vec2(u1, v1),
                glm::vec2(u0, v1)
            };
        }

    private:
        glad::Texture2D texture_;
    };
}
