#include <array>
#include <stdexcept>

#include <glm/glm.hpp>

#include "first_app.hpp"

namespace
{
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

std::vector<Model::Vertex>
  createSierpinski(const glm::vec2 &top, const glm::vec2 &left, const glm::vec2 &right, std::size_t depth)
{
    std::vector<Model::Vertex> vertices;
    doCreateSierpinski(top, left, right, vertices, depth);

    return vertices;
}

} // namespace

FirstApp::FirstApp()
{
    loadModels();
    createPipelineLayout();
    createPipeline();
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
    std::vector<Model::Vertex> vertices{{{0.0f, -0.5f}}, {{0.5f, 0.5f}}, {{-0.5f, 0.5f}}};
    // const auto sierpinski = createSierpinski({0.0f, -0.5f}, {0.5f, 0.5f}, {-0.5f, 0.5f}, 10);
    model_ = std::make_unique<Model>(device_, vertices);
}

void FirstApp::createPipelineLayout()
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};

    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(device_.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout_) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void FirstApp::createPipeline()
{
    Pipeline::defaultPipelineConfigInfo(pipelineConfig_, swapChain_.width(), swapChain_.height());
    pipelineConfig_.renderPass = swapChain_.getRenderPass();
    pipelineConfig_.pipelineLayout = pipelineLayout_;
    pipeline_ =
      std::make_unique<Pipeline>(device_, "simple_shader.vert.spv", "simple_shader.frag.spv", pipelineConfig_);
}

void FirstApp::createCommandBuffers()
{
    commandBuffers_.resize(swapChain_.imageCount());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = device_.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers_.size());

    if (vkAllocateCommandBuffers(device_.device(), &allocInfo, commandBuffers_.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    for (int i = 0; i < commandBuffers_.size(); i++)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffers_[i], &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = swapChain_.getRenderPass();
        renderPassInfo.framebuffer = swapChain_.getFrameBuffer(i);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChain_.getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.0f, 0.1f, 0.1f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers_[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        pipeline_->bind(commandBuffers_[i]);
        model_->bind(commandBuffers_[i]);
        model_->draw(commandBuffers_[i]);

        vkCmdEndRenderPass(commandBuffers_[i]);
        if (vkEndCommandBuffer(commandBuffers_[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record command buffer!");
        }
    }
}

void FirstApp::drawFrame()
{
    uint32_t imageIndex;
    auto result = swapChain_.acquireNextImage(&imageIndex);
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    result = swapChain_.submitCommandBuffers(&commandBuffers_[imageIndex], &imageIndex);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to present swap chain image!");
    }
}
