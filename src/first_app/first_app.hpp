#ifndef SRC_FIRST_APP_FIRST_APP
#define SRC_FIRST_APP_FIRST_APP

#include <memory>

#include <descriptors.hpp>
#include <device.hpp>
#include <game_object.hpp>
#include <renderer.hpp>
#include <window.hpp>

class FirstApp
{
  public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    FirstApp();
    FirstApp(const FirstApp &) = delete;
    FirstApp &operator=(const FirstApp &) = delete;

    void run();

  private:
    void loadGameObjects();

    Window window_{WIDTH, HEIGHT, "Hello Vulkan!"};
    Device device_{window_};
    Renderer renderer_{window_, device_};
    // note: order of declarations matters
    std::unique_ptr<DescriptorPool> globalPool_{};
    std::vector<GameObject> gameObjects_;
};

#endif /* SRC_FIRST_APP_FIRST_APP */
