#ifndef SRC_FIRST_APP_FIRST_APP
#define SRC_FIRST_APP_FIRST_APP

#include <device.hpp>
#include <pipeline.hpp>
#include <window.hpp>

class FirstApp
{
  public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    FirstApp();
    void run();

  private:
    void createPipeline();

    Window window_{WIDTH, HEIGHT, "Hello Vulkan!"};
    Device device_{window_};
};

#endif /* SRC_FIRST_APP_FIRST_APP */
