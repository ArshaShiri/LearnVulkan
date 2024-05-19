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

    VkExtent2D getExtent()
    {
        return {static_cast<uint32_t>(width_), static_cast<uint32_t>(height_)};
    }

    bool wasWindowResized()
    {
        return frameBufferResized_;
    }

    void resetWindowResizedFlag()
    {
        frameBufferResized_ = false;
    }

    GLFWwindow *getGLFWwindow() const
    {
        return window_.get();
    }

    ~Window();

  private:
    static void frameBufferResizeCallback(GLFWwindow *window, int width, int height);
    void initWindow();

    int width_;
    int height_;
    bool frameBufferResized_ = false;

    std::string windowName_;
    std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> window_;
};

#endif /* SRC_WINDOW */
