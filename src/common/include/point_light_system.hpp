#ifndef SRC_COMMON_INCLUDE_POINT_LIGHT_SYSTEM
#define SRC_COMMON_INCLUDE_POINT_LIGHT_SYSTEM


#include "camera.hpp"
#include "device.hpp"
#include "frame_info.hpp"
#include "game_object.hpp"
#include "pipeline.hpp"

// std
#include <memory>
#include <vector>


class PointLightSystem
{
  public:
    PointLightSystem(Device &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
    ~PointLightSystem();

    PointLightSystem(const PointLightSystem &) = delete;
    PointLightSystem &operator=(const PointLightSystem &) = delete;

    void update(FrameInfo &frameInfo, GlobalUbo &ubo);
    void render(FrameInfo &frameInfo);

  private:
    void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
    void createPipeline(VkRenderPass renderPass);

    Device &device_;
    std::unique_ptr<Pipeline> pipeline_;
    VkPipelineLayout pipelineLayout_;
};

#endif /* SRC_COMMON_INCLUDE_POINT_LIGHT_SYSTEM */
