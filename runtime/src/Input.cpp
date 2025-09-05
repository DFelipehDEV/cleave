#include "Input.hpp"

namespace Cleave {
void Input::AddAction(const std::string& action, int key) {
    m_actions[action].push_back(key);
    m_actionsStates[action] = 0.0f;
    m_actionsStatesPressed[action] = false;
}

float Input::GetActionState(const std::string& action) const {
    if (m_actionsStates.find(action) == m_actionsStates.end()) return 0.0f;
    return m_actionsStates.at(action);
}

bool Input::IsActionPressed(const std::string& action) const {
    if (m_actionsStates.find(action) == m_actionsStates.end()) return false;
    return m_actionsStates.at(action) > 0.0f;
}

bool Input::IsActionJustPressed(const std::string& action) const {
    if (m_actionsStatesPressed.find(action) == m_actionsStatesPressed.end()) return false;
    return m_actionsStatesPressed.at(action);
}

void Input::Update() {
    for (auto& [key, actions] : m_actions) {
        float pressed = 0.0f;
        bool justPressed = false;
        for (int action : actions) {
            if (action <= GLFW_KEY_LAST) {
                float isPressed = static_cast<float>(glfwGetKey(glfwGetCurrentContext(), action) == GLFW_PRESS);
                if (isPressed > 0.0f) pressed = 1.0f;
                // Just pressed if was not pressed last frame and is pressed now
                if (m_actionsStates[key] == 0.0f && isPressed == 1.0f)
                    justPressed = true;
            } else {
                // TODO: Controller
            }
        }
        m_actionsStatesPressed[key] = justPressed;
        m_actionsStates[key] = pressed;
    }
}
}  // namespace Cleave