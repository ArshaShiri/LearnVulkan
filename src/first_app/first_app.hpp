#ifndef SRC_FIRST_APP_FIRST_APP
#define SRC_FIRST_APP_FIRST_APP

#include <pipeline.hpp>
#include <window.hpp>

class FirstApp
{
  public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    void run();

  private:
    Pipeline pipeline{"simple_shader.vert.spv", "simple_shader.frag.spv"};
    Window window_{WIDTH, HEIGHT, "Hello Vulkan!"};
};

#endif /* SRC_FIRST_APP_FIRST_APP */
