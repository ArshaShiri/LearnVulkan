#include <array>
#include <chrono>
#include <stdexcept>

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "camera.hpp"
#include "first_app.hpp"
#include "keyboard_movement_controller.hpp"
#include "simple_render_system.hpp"

namespace
{

} // namespace

FirstApp::FirstApp()
{
    loadGameObjects();
}

void FirstApp::run()
{
    SimpleRenderSystem simpleRenderSystem{device_, renderer_.getSwapChainRenderPass()};
    Camera camera{};

    auto viewerObject = GameObject::createGameObject();
    KeyboardMovementController cameraController{};

    auto currentTime = std::chrono::high_resolution_clock::now();

    while (!window_.shouldClose())
    {
        glfwPollEvents();

        const auto newTime = std::chrono::high_resolution_clock::now();
        const float frameTime =
          std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();

        cameraController.moveInPlaneXZ(window_.getGLFWwindow(), frameTime, viewerObject);
        camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

        currentTime = newTime;

        const auto aspect = renderer_.getAspectRatio();
        // camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
        camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

        if (auto commandBuffer = renderer_.beginFrame())
        {
            renderer_.beginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects_, camera);
            renderer_.endSwapChainRenderPass(commandBuffer);
            renderer_.endFrame();
        }
    }
}

void FirstApp::loadGameObjects()
{
    std::shared_ptr<Model> model = Model::createModelFromFile(device_, "models/flat_vase.obj");
    auto flatVase = GameObject::createGameObject();
    flatVase.model = model;
    flatVase.transform.translation = {-.5f, .5f, 2.5f};
    flatVase.transform.scale = {3.f, 1.5f, 3.f};
    gameObjects_.push_back(std::move(flatVase));

    model = Model::createModelFromFile(device_, "models/smooth_vase.obj");
    auto smoothVase = GameObject::createGameObject();
    smoothVase.model = model;
    smoothVase.transform.translation = {.5f, .5f, 2.5f};
    smoothVase.transform.scale = {3.f, 1.5f, 3.f};
    gameObjects_.push_back(std::move(smoothVase));
}
