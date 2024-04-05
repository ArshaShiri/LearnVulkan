#ifndef SRC_FIRST_APP_FIRST_APP
#define SRC_FIRST_APP_FIRST_APP

#include <memory>

#include <device.hpp>
#include <game_object.hpp>
#include <pipeline.hpp>
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

    ~FirstApp();

  private:
    void loadGameObjects();
    void createPipelineLayout();
    void createPipeline();
    void renderGameObjects(VkCommandBuffer commandBuffer);

    Window window_{WIDTH, HEIGHT, "Hello Vulkan!"};
    Device device_{window_};
    Renderer renderer_{window_, device_};

    std::unique_ptr<Pipeline> pipeline_{};
    VkPipelineLayout pipelineLayout_{};
    std::vector<GameObject> gameObjects_;
};

#endif /* SRC_FIRST_APP_FIRST_APP */
