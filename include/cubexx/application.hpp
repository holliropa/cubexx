#pragma once

#include <memory>
#include "gl_common.hpp"

namespace cubexx {
    class Application {
    public:
        Application(int width, int height, const char* title);

        ~Application();

        [[nodiscard]] glfw::Window* getWindow() const {
            return pWindow_.get();
        }

        int width_;
        int height_;

    private:
        std::unique_ptr<glfw::Window> pWindow_;
    };
}
