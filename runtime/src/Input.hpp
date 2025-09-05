#pragma once
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <unordered_map>

namespace Cleave {
class Input {
public:
    void AddAction(const std::string& action, int key);
    
    float GetActionState(const std::string& action) const;
    bool IsActionPressed(const std::string& action) const;
    bool IsActionJustPressed(const std::string& action) const;

    void Update();
private:
    std::unordered_map<std::string, std::vector<int>> m_actions;
    std::unordered_map<std::string, float> m_actionsStates;
    std::unordered_map<std::string, bool> m_actionsStatesPressed;
};
}  // namespace Cleave