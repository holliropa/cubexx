#include <iostream>
#include <chrono>
#include "application.hpp"
#include "game.hpp"

void framebuffer_size_callback(int, int);

void key_callback(glfw::KeyCode, int, glfw::KeyAction, glfw::ModifierKeyBit);

void cursor_position_callback(double, double);

void mouse_button_callback(glfw::MouseButton, glfw::MouseButtonAction, glfw::ModifierKeyBit);

void cursor_enter_callback(bool);

// The Width of the screen
const unsigned int SCREEN_WIDTH = 600;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 600;

glfw::Window *pWindow;
std::string title;
Game *pGame;

double lastTime = 0.0;          // Time of the last frame
double lastDelta = 1.0 / 60.0; // Smoothed delta time
int frameCount = 0;             // Number of frames since the last title update
double fpsTime = 0.0f;           // Time accumulator for FPS calculation
double startTime = 0.0;         // Record the start time

Application::Application() {
    if (!glfw::init()) {
        std::cerr << "Failed to initialize GLFW" << '\n';
        throw std::runtime_error("GLFW Creation Failed");
    }

    glfw::ContextHints{
            .contextVersionMajor = 3,
            .contextVersionMinor = 3,
            .openglProfile = glfw::OpenGLProfile::Core,
    }.apply();

    glfw::WindowHints{
            .resizable = false,
    }.apply();

    title = "Game of Life";
    pWindow = new glfw::Window(SCREEN_WIDTH, SCREEN_HEIGHT, title.c_str());

    glfw::makeContextCurrent(*pWindow);

    if (!glad::load()) {
        std::cerr << "Failed to initialize GLAD" << '\n';
        throw std::runtime_error("GLAD Initialization Failed");
    }

//    glfwSwapInterval(1);

    pWindow->setCursorMode(glfw::CursorMode::Disabled);
    pWindow->setCursorPosition(static_cast<double>(SCREEN_WIDTH) / 2.0,
                               static_cast<double>(SCREEN_HEIGHT) / 2.0);

    double cursorX, cursorY;
    pWindow->getCursorPosition(&cursorX, &cursorY);
    Input::InitCursor(static_cast<float>(cursorX), static_cast<float>(cursorY));

    pWindow->keyEvent.set(key_callback);
    pWindow->framebufferSizeEvent.set(framebuffer_size_callback);
    pWindow->cursorPosEvent.set(cursor_position_callback);
    pWindow->mouseButtonEvent.set(mouse_button_callback);
    pWindow->cursorEnterEvent.set(cursor_enter_callback);

    // initialize game
    // ---------------
    pGame = new Game();
    pGame->Resize(SCREEN_WIDTH, SCREEN_HEIGHT);
    pGame->Initialize();

    // OpenGL configuration
    // --------------------
    glad::Viewport(SCREEN_WIDTH, SCREEN_HEIGHT);
    glad::Enable(glad::Capability::kBlend);
    glad::Enable(glad::Capability::kDepthTest);
    glad::Enable(glad::Capability::kCullFace);
    glad::BlendFunc(glad::BlendFunction::kSrcAlpha, glad::BlendFunction::kOneMinusSrcAlpha);
}

Application::~Application() {
    delete pGame;
    delete pWindow;

    Input::Clear();
    glad::unload();
    glfw::terminate();
}

bool isEnabled = false;

void Application::run() {
    startTime = glfw::getTime();

    while (!glfwWindowShouldClose(static_cast<GLFWwindow *>(*pWindow))) {
        auto frameStartTime = std::chrono::high_resolution_clock::now(); // Start time of the frame

        auto currentTime = glfw::getTime();
        auto deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Smooth the delta time
        lastDelta = (lastDelta + deltaTime) / 2;

        // Accumulate time for FPS counter
        fpsTime += deltaTime;
        frameCount++;

        // Update the window title every 1 second (or any desired interval)
        if (fpsTime >= 1.0f) {
            float fps = static_cast<float>(frameCount) / static_cast<float>(fpsTime);  // Calculate FPS
            auto elapsedTime = currentTime - startTime;  // Calculate time since start

            // Update the window title with both FPS and elapsed time
            std::string current_title = title + " | FPS: " + std::to_string(static_cast<int>(fps))
                                        + " | Time: " + std::to_string(static_cast<int>(elapsedTime)) + "s";
            pWindow->setTitle(current_title.c_str());

            // Reset counters
            fpsTime = 0.0f;
            frameCount = 0;
        }
        glfwPollEvents();

        Input::Update();

        pGame->Update(static_cast<float>(deltaTime));

        if (Input::GetKeyDown(glfw::KeyCode::LeftAlt)) {
            if (isEnabled)
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            isEnabled = !isEnabled;
        }

        glad::ClearColor(0.15625f, 0.453125f, 0.6953125f, 1.0f);
        glad::ClearBuffers().Depth().Color();

        pGame->Render();

        pWindow->swapBuffers();

        if (pGame->IsExit) {
            pWindow->setShouldClose(true);
        }
    }
}

void key_callback(glfw::KeyCode key, int, glfw::KeyAction action, glfw::ModifierKeyBit) {
    Input::HandleKey(key, action);
}

void framebuffer_size_callback(int width, int height) {
    glad::Viewport(width, height);
    pGame->Resize(width, height);
}

void cursor_position_callback(double x, double y) {
    Input::HandleCursorPosition(static_cast<float>(x), static_cast<float>(y));
}

void mouse_button_callback(glfw::MouseButton button, glfw::MouseButtonAction action, glfw::ModifierKeyBit mods) {
//    pGame->Input.handleButtonEvent(buttonCode, action);
}

void cursor_enter_callback(bool entered) {
//    pGame->Input.IsMouseInside = entered;
}