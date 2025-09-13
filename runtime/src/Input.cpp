#include "Input.hpp"

namespace Cleave {
void Input::AddAction(const std::string& actionName, int key) {
    InputAction& action = m_actions[actionName];
    action.inputs.push_back(key);
}

float Input::GetActionState(const std::string& actionName) const {
    if (m_actions.find(actionName) == m_actions.end()) return 0.0f;
    return m_actions.at(actionName).state;
}

bool Input::IsActionPressed(const std::string& actionName) const {
    if (m_actions.find(actionName) == m_actions.end()) return false;
    return m_actions.at(actionName).state > 0.0f;
}

bool Input::IsActionJustPressed(const std::string& actionName) const {
    if (m_actions.find(actionName) == m_actions.end()) return false;
    return m_actions.at(actionName).justPressed;
}

void Input::Update() {
    for (auto& [key, action] : m_actions) {
        float state = 0.0f;
        bool justPressed = false;

        for (int input : action.inputs) {
            if (input <= GLFW_KEY_LAST) {
                float isPressed = static_cast<float>(glfwGetKey(glfwGetCurrentContext(), input) == GLFW_PRESS);
                if (isPressed > 0.0f) state = 1.0f;
                // Just pressed if was not pressed last frame and is pressed now
                if (action.state == 0.0f && isPressed == 1.0f)
                    justPressed = true;
            } else {
                // TODO: Controller
            }
        }

        action.justPressed = justPressed;
        action.state = state;
    }
}
}  // namespace Cleave