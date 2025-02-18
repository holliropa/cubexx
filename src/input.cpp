#include "input.hpp"

typedef struct {
    glfw::KeyCode Code;
    glfw::KeyAction Action;
} UnhandledKey;

std::map<glfw::KeyCode, bool> m_pressed_keys;
std::map<glfw::KeyCode, bool> m_up_keys;
std::map<glfw::KeyCode, bool> m_down_keys;

std::queue<UnhandledKey> m_unhandled_keys;

glm::vec2 m_current_mouse_position{};
glm::vec2 m_last_mouse_position{};
glm::vec2 m_offset_mouse_position{};

Input::Input() = default;

Input::~Input() = default;

bool Input::GetKeyPressed(glfw::KeyCode keyCode) {
    return m_pressed_keys[keyCode];
}

bool Input::GetKeyDown(glfw::KeyCode keyCode) {
    return m_down_keys[keyCode];
}

bool Input::GetKeyUp(glfw::KeyCode keyCode) {
    return m_up_keys[keyCode];
}

const glm::vec2 &Input::GetCursorPosition() {
    return m_current_mouse_position;
}

const glm::vec2 &Input::GetCursorOffset() {
    return m_offset_mouse_position;
}

void Input::Update() {
    m_down_keys.clear();
    m_up_keys.clear();

    while (!m_unhandled_keys.empty()) {
        auto unhandledKey = m_unhandled_keys.front();
        m_unhandled_keys.pop();

        if (unhandledKey.Action == glfw::KeyAction::Press) {
            m_down_keys[unhandledKey.Code] = true;
            m_pressed_keys[unhandledKey.Code] = true;
        } else if (unhandledKey.Action == glfw::KeyAction::Release) {
            m_up_keys[unhandledKey.Code] = true;
            m_pressed_keys[unhandledKey.Code] = false;
        }
    }

    m_offset_mouse_position = m_current_mouse_position - m_last_mouse_position;
    m_last_mouse_position = m_current_mouse_position;
}

void Input::Clear() {
    m_pressed_keys.clear();
    m_up_keys.clear();
    m_down_keys.clear();
}

void Input::InitCursor(float x, float y) {
    m_current_mouse_position = glm::vec2(x, y);
    m_last_mouse_position = glm::vec2(x, y);
}

void Input::HandleKey(glfw::KeyCode keyCode, glfw::KeyAction keyAction) {
    m_unhandled_keys.push({keyCode, keyAction});
}

void Input::HandleCursorPosition(float x, float y) {
    m_current_mouse_position.x = x;
    m_current_mouse_position.y = y;
}
