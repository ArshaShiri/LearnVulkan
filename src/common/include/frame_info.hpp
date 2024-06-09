#ifndef SRC_COMMON_INCLUDE_FRAME_INFO
#define SRC_COMMON_INCLUDE_FRAME_INFO

#include "camera.hpp"

// lib
#include <vulkan/vulkan.h>

struct FrameInfo
{
    int frameIndex;
    float frameTime;
    VkCommandBuffer commandBuffer;
    Camera &camera;
};

#endif /* SRC_COMMON_INCLUDE_FRAME_INFO */
