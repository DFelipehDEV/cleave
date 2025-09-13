#pragma once
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <unordered_map>

namespace Cleave {
struct InputAction {
    std::vector<int> inputs;
    float state = 0.0f;
    bool justPressed = false;
};

class Input {
public:
    void AddAction(const std::string& actionName, int key);
    
    float GetActionState(const std::string& actionName) const;
    bool IsActionPressed(const std::string& actionName) const;
    bool IsActionJustPressed(const std::string& actionName) const;

    void Update();
private:
    std::unordered_map<std::string, InputAction> m_actions;
};
}  // namespace Cleave