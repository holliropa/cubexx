#pragma once
#include "cube_type.h"

namespace cubexx::cube_utils {
    inline bool is_face_visible(const CubeType face, const CubeType neighbor) {
        if (face == CubeType::Air) return false;
        if (face == CubeType::Water) return neighbor == CubeType::Air;
        return neighbor == CubeType::Air || neighbor == CubeType::Water;;
    }
}
