#include <array>
#include <stdexcept>

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "first_app.hpp"
#include "simple_render_system.hpp"

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
} // namespace

FirstApp::FirstApp()
{
    loadGameObjects();
}

void FirstApp::run()
{
    SimpleRenderSystem simpleRenderSystem{device_, renderer_.getSwapChainRenderPass()};

    while (!window_.shouldClose())
    {
        glfwPollEvents();

        if (auto commandBuffer = renderer_.beginFrame())
        {
            renderer_.beginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects_);
            renderer_.endSwapChainRenderPass(commandBuffer);
            renderer_.endFrame();
        }
    }
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
