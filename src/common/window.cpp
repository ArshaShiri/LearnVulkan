#include <stdexcept>

#include "include/window.hpp"

Window::Window(int width, int height, const std::string windowName)
  : width_{width}, height_{height}, windowName_{std::move(windowName)}, window_{nullptr, glfwDestroyWindow}
{
    initWindow();
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(window_.get());
}

void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
{
    if (glfwCreateWindowSurface(instance, window_.get(), nullptr, surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to craete window surface");
    }
}

Window::~Window()
{
    glfwTerminate();
}

void Window::initWindow()
{
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    // We do not want to make an openGL context since we are using this for Vulkan.
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // TODO The window resizing is going to be supported by another way than glfw
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow *window = glfwCreateWindow(width_, height_, windowName_.c_str(), NULL, NULL);

    if (window == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    window_ = std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)>(window, glfwDestroyWindow);
}
