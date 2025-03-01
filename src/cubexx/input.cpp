#include "cubexx/input.hpp"

namespace cubexx {
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

    bool Input::GetKeyPressed(const glfw::KeyCode code) {
        return m_pressed_keys[code];
    }

    bool Input::GetKeyDown(const glfw::KeyCode code) {
        return m_down_keys[code];
    }

    bool Input::GetKeyUp(const glfw::KeyCode code) {
        return m_up_keys[code];
    }

    const glm::vec2& Input::GetCursorPosition() {
        return m_current_mouse_position;
    }

    const glm::vec2& Input::GetCursorOffset() {
        return m_offset_mouse_position;
    }

    void Input::Update() {
        m_down_keys.clear();
        m_up_keys.clear();

        while (!m_unhandled_keys.empty()) {
            auto [code, action] = m_unhandled_keys.front();
            m_unhandled_keys.pop();

            if (action == glfw::KeyAction::Press) {
                m_down_keys[code] = true;
                m_pressed_keys[code] = true;
            }
            else if (action == glfw::KeyAction::Release) {
                m_up_keys[code] = true;
                m_pressed_keys[code] = false;
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

    void Input::InitCursor(const float x, const float y) {
        m_current_mouse_position = glm::vec2(x, y);
        m_last_mouse_position = glm::vec2(x, y);
    }

    void Input::HandleKey(const glfw::KeyCode code, const glfw::KeyAction action) {
        m_unhandled_keys.push({code, action});
    }

    void Input::HandleCursorPosition(const float x, const float y) {
        m_current_mouse_position.x = x;
        m_current_mouse_position.y = y;
    }
}
