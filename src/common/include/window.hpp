#ifndef SRC_WINDOW
#define SRC_WINDOW

#include <memory>
#include <string>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Window
{
  public:
    Window(int width, int height, const std::string windowName);
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    bool shouldClose() const;
    void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    ~Window();

  private:
    void initWindow();

    const int width_;
    const int height_;
    std::string windowName_;
    std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> window_;
};

#endif /* SRC_WINDOW */
