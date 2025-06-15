#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Cleave {
class Window {
public:
    Window(int width, int height, const char* title);
    ~Window();
    bool shouldClose() const;
    void pollEvents() const;
    void swapBuffers() const;

    int GetWidth() const;
    int GetHeight() const;
    GLFWwindow* getGLFWwindow() const;

private:
    GLFWwindow* window;
};
}  // namespace Cleave