#ifndef SRC_FIRST_APP_FIRST_APP
#define SRC_FIRST_APP_FIRST_APP

#include <device.hpp>
#include <memory>
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
    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void drawFrame();

    Window window_{WIDTH, HEIGHT, "Hello Vulkan!"};
    Device device_{window_};
    SwapChain swapChain_{device_, window_.getExtent()};
    PipelineConfigInfo pipelineConfig_{};
    std::unique_ptr<Pipeline> pipeline_{};
    VkPipelineLayout pipelineLayout_{};
    std::vector<VkCommandBuffer> commandBuffers_{};
};

#endif /* SRC_FIRST_APP_FIRST_APP */
