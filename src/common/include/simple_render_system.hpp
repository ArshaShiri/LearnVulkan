#ifndef SRC_COMMON_INCLUDE_SIMPLE_RENDER_SYSTEM
#define SRC_COMMON_INCLUDE_SIMPLE_RENDER_SYSTEM

#include <memory>

#include <device.hpp>
#include <game_object.hpp>
#include <pipeline.hpp>

#include "camera.hpp"
#include "frame_info.hpp"

class SimpleRenderSystem
{
  public:
    SimpleRenderSystem(Device &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
    SimpleRenderSystem(const SimpleRenderSystem &) = delete;
    SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

    void renderGameObjects(FrameInfo &frameInfo);

    ~SimpleRenderSystem();

  private:
    void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
    void createPipeline(VkRenderPass renderPass);

    Device &device_;

    std::unique_ptr<Pipeline> pipeline_{};
    VkPipelineLayout pipelineLayout_{};
};

#endif /* SRC_COMMON_INCLUDE_SIMPLE_RENDER_SYSTEM */
