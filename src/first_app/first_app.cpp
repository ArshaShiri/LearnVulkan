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
// //! Exercise of video 6
// void doCreateSierpinski(const glm::vec2 &top,
//                         const glm::vec2 &left,
//                         const glm::vec2 &right,
//                         std::vector<Model::Vertex> &vertices,
//                         std::size_t depth)
// {
//     if (depth <= 0)
//     {
//         vertices.push_back({top});
//         vertices.push_back({left});
//         vertices.push_back({right});
//     }
//     else
//     {
//         const auto topLeft = 0.5f * (left + top);
//         const auto topRight = 0.5f * (right + top);
//         const auto leftRight = 0.5f * (left + right);

//         doCreateSierpinski(top, topLeft, topRight, vertices, depth - 1);
//         doCreateSierpinski(topLeft, left, leftRight, vertices, depth - 1);
//         doCreateSierpinski(topRight, leftRight, right, vertices, depth - 1);
//     }
// }

// //! Exercise of video 6
// std::vector<Model::Vertex>
//   createSierpinski(const glm::vec2 &top, const glm::vec2 &left, const glm::vec2 &right, std::size_t depth)
// {
//     std::vector<Model::Vertex> vertices;
//     doCreateSierpinski(top, left, right, vertices, depth);

//     return vertices;
// }

// temporary helper function, creates a 1x1x1 cube centered at offset
std::unique_ptr<Model> createCubeModel(Device &device, glm::vec3 offset)
{
    std::vector<Model::Vertex> vertices{

      // left face (white)
      {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
      {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
      {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

      // right face (yellow)
      {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
      {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
      {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

      // top face (orange, remember y axis points down)
      {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
      {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
      {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

      // bottom face (red)
      {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
      {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
      {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

      // nose face (blue)
      {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
      {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
      {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

      // tail face (green)
      {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
      {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
      {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

    };
    for (auto &v : vertices)
    {
        v.position += offset;
    }
    return std::make_unique<Model>(device, vertices);
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
    std::shared_ptr<Model> model_ = createCubeModel(device_, {.0f, .0f, .0f});
    auto cube = GameObject::createGameObject();
    cube.model = model_;
    cube.transform.translation = {.0f, .0f, .5f};
    cube.transform.scale = {.5f, .5f, .5f};
    gameObjects_.push_back(std::move(cube));
}
