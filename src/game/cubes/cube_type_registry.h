#pragma once
#include <stdexcept>
#include <unordered_map>

#include "cube_type.h"
#include "cube_type_definition.h"

namespace cubexx {
    class CubeTypeRegistry {
    public:
        void register_definition(const CubeType type, const CubeTypeDefinition& definition) {
            definitions_[type] = definition;
        }

        CubeTypeDefinition& get_definition(const CubeType type) {
            const auto it = definitions_.find(type);
            if (it == definitions_.end()) {
                throw std::runtime_error("Cube type not registered: " + std::to_string(static_cast<int>(type)));
            }
            return it->second;
        }

    private:
        std::unordered_map<CubeType, CubeTypeDefinition> definitions_;
    };
}
