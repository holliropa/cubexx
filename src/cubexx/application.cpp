#include "cubexx/application.hpp"

namespace cubexx {
    Application::Application(int width, int height, const char* title)
        : width_(width), height_(height) {
        if (!glfw::init())
            throw std::runtime_error("Failed to initialize GLFW\n");

        glfw::ContextHints{
            .contextVersionMajor = 3,
            .contextVersionMinor = 3,
            .openglProfile = glfw::OpenGLProfile::Core,
        }.apply();

        pWindow_ = std::make_unique<glfw::Window>(width, height, title);

        makeContextCurrent(*pWindow_);
    }

    Application::~Application() {
        pWindow_.reset();

        glfw::terminate();
    }
}
