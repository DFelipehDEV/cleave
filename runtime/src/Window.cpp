#include "Window.hpp"
#include <iostream>

Window::Window(int width, int height, const char* title) {
    if (!glfwInit()) {
        std::cerr << "GLFW Init Failed\n";
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW Window Creation Failed\n";
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void Window::pollEvents() const {
    glfwPollEvents();
}

void Window::swapBuffers() const {
    glfwSwapBuffers(window);
}

int Window::GetWidth() const
{
    int w;
    glfwGetWindowSize(getGLFWwindow(), &w, nullptr);
    return w;
}

int Window::GetHeight() const
{
    int h;
    glfwGetWindowSize(getGLFWwindow(), nullptr, &h);
    return h;
}

GLFWwindow* Window::getGLFWwindow() const {
    return window;
}
