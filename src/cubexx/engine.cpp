#include "cubexx/engine.hpp"

namespace cubexx {
    Engine::Engine() {
        if (!glad::load()) {
            throw std::runtime_error("Failed to load GLAD");
        }
    }

    Engine::~Engine() {
        glad::unload();
    }
}