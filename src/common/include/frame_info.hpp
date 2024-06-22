#ifndef SRC_COMMON_INCLUDE_FRAME_INFO
#define SRC_COMMON_INCLUDE_FRAME_INFO

#include "camera.hpp"
#include "game_object.hpp"

// lib
#include <vulkan/vulkan.h>

struct FrameInfo
{
    int frameIndex;
    float frameTime;
    VkCommandBuffer commandBuffer;
    Camera &camera;
    VkDescriptorSet globalDescriptorSet;
    GameObject::Map &gameObjects;
};

#endif /* SRC_COMMON_INCLUDE_FRAME_INFO */
