#include <array>
#include <stdexcept>

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

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
    glm::mat2 transform{1.f};
    alignas(8) glm::vec2 offset;
    alignas(16) glm::vec3 color;
};

} // namespace

FirstApp::FirstApp()
{
    loadGameObjects();
    createPipelineLayout();
    createPipeline();
}

void FirstApp::run()
{
    while (!window_.shouldClose())
    {
        glfwPollEvents();

        if (auto commandBuffer = renderer_.beginFrame())
        {
            renderer_.beginSwapChainRenderPass(commandBuffer);
            renderGameObjects(commandBuffer);
            renderer_.endSwapChainRenderPass(commandBuffer);
            renderer_.endFrame();
        }
    }
}

FirstApp::~FirstApp()
{
    vkDestroyPipelineLayout(device_.device(), pipelineLayout_, nullptr);
}

void FirstApp::loadGameObjects()
{
    std::vector<Model::Vertex> vertices{
      {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}}, {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}}, {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
    auto model = std::make_shared<Model>(device_, vertices);

    auto triangle = GameObject::createGameObject();
    triangle.model = model;
    triangle.color = {.1f, .8f, .1f};
    triangle.transform2d.translation.x = .2f;
    triangle.transform2d.scale = {2.f, .5f};
    triangle.transform2d.rotation = .25f * glm::two_pi<float>();

    gameObjects_.push_back(std::move(triangle));
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

void FirstApp::createPipeline()
{
    assert(pipelineLayout_ != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    Pipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = renderer_.getSwapChainRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout_;
    pipeline_ = std::make_unique<Pipeline>(device_, "simple_shader.vert.spv", "simple_shader.frag.spv", pipelineConfig);
}

void FirstApp::renderGameObjects(VkCommandBuffer commandBuffer)
{
    pipeline_->bind(commandBuffer);

    for (auto &obj : gameObjects_)
    {
        obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.01f, glm::two_pi<float>());

        SimplePushConstantData push{};
        push.offset = obj.transform2d.translation;
        push.color = obj.color;
        push.transform = obj.transform2d.mat2();

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
