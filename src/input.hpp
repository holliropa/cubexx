#pragma once

#include <map>
#include <queue>
#include "gl_common.hpp"
#include "glm_common.hpp"

class Input {
public:
    static bool GetKeyPressed(glfw::KeyCode keyCode);

    static bool GetKeyDown(glfw::KeyCode keyCode);

    static bool GetKeyUp(glfw::KeyCode keyCode);

    static const glm::vec2 &GetCursorPosition();

    static const glm::vec2 &GetCursorOffset();

    static void HandleKey(glfw::KeyCode keycode, glfw::KeyAction keyAction);

    static void HandleCursorPosition(float x, float y);

    static void Update();

    static void Clear();

    static void InitCursor(float x, float y);

private:
    Input();

    ~Input();

private:

    glm::vec2 m_scroll{};
    glm::vec2 m_lastScroll{};
    glm::vec2 m_scrollOffset{};

    glm::dvec2 m_lastCursorPos{};
    glm::dvec2 m_cursorPos{};
    glm::dvec2 m_cursorOffset{};


    std::queue<std::tuple<glfw::KeyCode, int>> unhandled_keys;
};