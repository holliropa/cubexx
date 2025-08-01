#pragma once
#include <array>
#include <string>
#include <utility>

namespace cubexx {
    struct CubeTypeDefinition {
        std::string name;

        std::array<size_t, 6> faceTiles;;

        CubeTypeDefinition() = default;

        CubeTypeDefinition(std::string name, const size_t tileIndex)
            : name(std::move(name)), faceTiles({tileIndex, tileIndex, tileIndex, tileIndex, tileIndex, tileIndex}) {}

        CubeTypeDefinition(std::string name, const size_t topTile, const size_t bottomTile, const size_t leftTile,
                           const size_t rightTile, const size_t frontTile, const size_t backTile)
            : name(std::move(name)), faceTiles({topTile, bottomTile, leftTile, rightTile, frontTile, backTile}) {}

        CubeTypeDefinition(std::string name, const std::array<size_t, 6>& faceTiles)
            : name(std::move(name)), faceTiles(faceTiles) {}
    };
}
