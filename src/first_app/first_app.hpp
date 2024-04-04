#ifndef SRC_FIRST_APP_FIRST_APP
#define SRC_FIRST_APP_FIRST_APP

#include <memory>

#include <device.hpp>
#include <game_object.hpp>
#include <pipeline.hpp>
#include <swap_chain.hpp>
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
    void createCommandBuffers();
    void freeCommandBuffers();
    void drawFrame();
    void recreateSwapChain();
    void recordCommandBuffer(int imageIndex);
    void renderGameObjects(VkCommandBuffer commandBuffer);

    Window window_{WIDTH, HEIGHT, "Hello Vulkan!"};
    Device device_{window_};
    std::unique_ptr<SwapChain> swapChain_{};
    std::unique_ptr<Pipeline> pipeline_{};
    VkPipelineLayout pipelineLayout_{};
    std::vector<VkCommandBuffer> commandBuffers_{};
    std::vector<GameObject> gameObjects_;
};

#endif /* SRC_FIRST_APP_FIRST_APP */
