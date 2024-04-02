#include <array>
#include <stdexcept>

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "first_app.hpp"

namespace
{
//! Exercise of video 6
void doCreateSierpinski(const glm::vec2 &top,
                        const glm::vec2 &left,
                        const glm::vec2 &right,
                        std::vector<Model::Vertex> &vertices,
                        std::size_t depth)
{
    if (depth <= 0)
    {
        vertices.push_back({top});
        vertices.push_back({left});
        vertices.push_back({right});
    }
    else
    {
        const auto topLeft = 0.5f * (left + top);
        const auto topRight = 0.5f * (right + top);
        const auto leftRight = 0.5f * (left + right);

        doCreateSierpinski(top, topLeft, topRight, vertices, depth - 1);
        doCreateSierpinski(topLeft, left, leftRight, vertices, depth - 1);
        doCreateSierpinski(topRight, leftRight, right, vertices, depth - 1);
    }
}

//! Exercise of video 6
std::vector<Model::Vertex>
  createSierpinski(const glm::vec2 &top, const glm::vec2 &left, const glm::vec2 &right, std::size_t depth)
{
    std::vector<Model::Vertex> vertices;
    doCreateSierpinski(top, left, right, vertices, depth);

    return vertices;
}

struct SimplePushConstantData
{
    glm::vec2 offset;
    alignas(16) glm::vec3 color;
};

} // namespace

FirstApp::FirstApp()
{
    loadModels();
    createPipelineLayout();
    recreateSwapChain();
    createCommandBuffers();
}

void FirstApp::run()
{
    while (!window_.shouldClose())
    {
        glfwPollEvents();
        drawFrame();
    }
}

FirstApp::~FirstApp()
{
    vkDestroyPipelineLayout(device_.device(), pipelineLayout_, nullptr);
}

void FirstApp::loadModels()
{
    std::vector<Model::Vertex> vertices{
      {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}}, {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}}, {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
    model_ = std::make_unique<Model>(device_, vertices);
}

void FirstApp::createPipelineLayout()
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

void FirstApp::recreateSwapChain()
{
    auto extent = window_.getExtent();
    while (extent.width == 0 || extent.height == 0)
    {
        extent = window_.getExtent();
        glfwWaitEvents();
    }

    // Wait till the current swap chain is unused before creating a new one.
    vkDeviceWaitIdle(device_.device());
    // swapChain_ = nullptr;

    if (swapChain_ == nullptr)
    {
        swapChain_ = std::make_unique<SwapChain>(device_, extent);
    }
    else
    {
        swapChain_ = std::make_unique<SwapChain>(device_, extent, std::move(swapChain_));
        if (swapChain_->imageCount() != commandBuffers_.size())
        {
            freeCommandBuffers();
            createCommandBuffers();
        }
    }

    createPipeline();
}

void FirstApp::createPipeline()
{
    assert(swapChain_ != nullptr && "Cannot create pipeline before swap chain");
    assert(pipelineLayout_ != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    Pipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = swapChain_->getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout_;
    pipeline_ = std::make_unique<Pipeline>(device_, "simple_shader.vert.spv", "simple_shader.frag.spv", pipelineConfig);
}

void FirstApp::createCommandBuffers()
{
    commandBuffers_.resize(swapChain_->imageCount());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = device_.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers_.size());

    if (vkAllocateCommandBuffers(device_.device(), &allocInfo, commandBuffers_.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void FirstApp::freeCommandBuffers()
{
    vkFreeCommandBuffers(device_.device(),
                         device_.getCommandPool(),
                         static_cast<uint32_t>(commandBuffers_.size()),
                         commandBuffers_.data());
    commandBuffers_.clear();
}

void FirstApp::recordCommandBuffer(int imageIndex)
{
    static int frame = 0;
    frame = (frame + 1) % 100;

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffers_[imageIndex], &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = swapChain_->getRenderPass();
    renderPassInfo.framebuffer = swapChain_->getFrameBuffer(imageIndex);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChain_->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffers_[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swapChain_->getSwapChainExtent().width);
    viewport.height = static_cast<float>(swapChain_->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, swapChain_->getSwapChainExtent()};
    vkCmdSetViewport(commandBuffers_[imageIndex], 0, 1, &viewport);
    vkCmdSetScissor(commandBuffers_[imageIndex], 0, 1, &scissor);

    pipeline_->bind(commandBuffers_[imageIndex]);
    model_->bind(commandBuffers_[imageIndex]);

    for (int j = 0; j < 4; j++)
    {
        SimplePushConstantData push{};
        push.offset = {-0.5f + frame * 0.02f, -0.4f + j * 0.25f};
        push.color = {0.0f, 0.0f, 0.2f + 0.2f * j};

        vkCmdPushConstants(commandBuffers_[imageIndex],
                           pipelineLayout_,
                           VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                           0,
                           sizeof(SimplePushConstantData),
                           &push);

        model_->draw(commandBuffers_[imageIndex]);
    }

    vkCmdEndRenderPass(commandBuffers_[imageIndex]);
    if (vkEndCommandBuffer(commandBuffers_[imageIndex]) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void FirstApp::drawFrame()
{
    uint32_t imageIndex;
    auto result = swapChain_->acquireNextImage(&imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        recreateSwapChain();
        return;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    recordCommandBuffer(imageIndex);
    result = swapChain_->submitCommandBuffers(&commandBuffers_[imageIndex], &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window_.wasWindowResized())
    {
        window_.resetWindowResizedFlag();
        recreateSwapChain();
        return;
    }
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to present swap chain image!");
    }
}
