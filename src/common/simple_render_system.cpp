#include <array>
#include <stdexcept>

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "simple_render_system.hpp"

namespace
{
struct SimplePushConstantData
{
    glm::mat4 transform{1.f};
    alignas(16) glm::vec3 color;
};
} // namespace

SimpleRenderSystem::SimpleRenderSystem(Device &device, VkRenderPass renderPass) : device_{device}
{
    createPipelineLayout();
    createPipeline(renderPass);
}

void SimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject> &gameObjects)
{
    pipeline_->bind(commandBuffer);

    for (auto &obj : gameObjects)
    {
        obj.transform.rotation.y = glm::mod(obj.transform.rotation.y + 0.01f, glm::two_pi<float>());
        obj.transform.rotation.x = glm::mod(obj.transform.rotation.x + 0.005f, glm::two_pi<float>());

        SimplePushConstantData push{};
        push.color = obj.color;
        push.transform = obj.transform.mat4();

        vkCmdPushConstants(commandBuffer,
                           pipelineLayout_,
                           VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                           0,
                           sizeof(SimplePushConstantData),
                           &push);

        obj.model->bind(commandBuffer);
        obj.model->draw(commandBuffer);
    }
}

SimpleRenderSystem::~SimpleRenderSystem()
{
    vkDestroyPipelineLayout(device_.device(), pipelineLayout_, nullptr);
}

void SimpleRenderSystem::createPipelineLayout()
{
    VkPushConstantRange pushConstantRange{};

    // We want to have access to push constant data both in vertex and fragment shaders.
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};

    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

    if (vkCreatePipelineLayout(device_.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout_) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void SimpleRenderSystem::createPipeline(VkRenderPass renderPass)
{
    assert(pipelineLayout_ != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    Pipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = pipelineLayout_;
    pipeline_ = std::make_unique<Pipeline>(device_, "simple_shader.vert.spv", "simple_shader.frag.spv", pipelineConfig);
}
