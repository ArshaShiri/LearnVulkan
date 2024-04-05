#ifndef SRC_COMMON_INCLUDE_SIMPLE_RENDER_SYSTEM
#define SRC_COMMON_INCLUDE_SIMPLE_RENDER_SYSTEM

#include <memory>

#include <device.hpp>
#include <game_object.hpp>
#include <pipeline.hpp>

class SimpleRenderSystem
{
  public:
    SimpleRenderSystem(Device &device, VkRenderPass renderPass);
    SimpleRenderSystem(const SimpleRenderSystem &) = delete;
    SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

    void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject> &gameObjects);

    ~SimpleRenderSystem();

  private:
    void createPipelineLayout();
    void createPipeline(VkRenderPass renderPass);

    Device &device_;

    std::unique_ptr<Pipeline> pipeline_{};
    VkPipelineLayout pipelineLayout_{};
};

#endif /* SRC_COMMON_INCLUDE_SIMPLE_RENDER_SYSTEM */
