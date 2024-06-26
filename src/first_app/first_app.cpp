#include <array>
#include <chrono>
#include <stdexcept>

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "buffer.hpp"
#include "camera.hpp"
#include "first_app.hpp"
#include "keyboard_movement_controller.hpp"
#include "point_light_system.hpp"
#include "simple_render_system.hpp"


FirstApp::FirstApp()
{
    globalPool_ = DescriptorPool::Builder(device_)
                    .setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
                    .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
                    .build();
    loadGameObjects();
}

void FirstApp::run()
{
    std::vector<std::unique_ptr<Buffer>> uboBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < uboBuffers.size(); i++)
    {
        uboBuffers[i] = std::make_unique<Buffer>(
          device_, sizeof(GlobalUbo), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        uboBuffers[i]->map();
    }

    auto globalSetLayout = DescriptorSetLayout::Builder(device_)
                             .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
                             .build();

    std::vector<VkDescriptorSet> globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < globalDescriptorSets.size(); i++)
    {
        auto bufferInfo = uboBuffers[i]->descriptorInfo();
        DescriptorWriter(*globalSetLayout, *globalPool_).writeBuffer(0, &bufferInfo).build(globalDescriptorSets[i]);
    }

    SimpleRenderSystem simpleRenderSystem{
      device_, renderer_.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
    PointLightSystem pointLightSystem{
      device_, renderer_.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};

    Camera camera{};

    auto viewerObject = GameObject::createGameObject();
    viewerObject.transform.translation.z = -2.5f;
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
        camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

        if (auto commandBuffer = renderer_.beginFrame())
        {
            int frameIndex = renderer_.getFrameIndex();
            FrameInfo frameInfo{
              frameIndex, frameTime, commandBuffer, camera, globalDescriptorSets[frameIndex], gameObjects_};

            // update
            GlobalUbo ubo{};
            ubo.projection = camera.getProjection();
            ubo.view = camera.getView();
            ubo.inverseView = camera.getInverseView();
            pointLightSystem.update(frameInfo, ubo);
            uboBuffers[frameIndex]->writeToBuffer(&ubo);
            uboBuffers[frameIndex]->flush();

            // render
            renderer_.beginSwapChainRenderPass(commandBuffer);

            // order here matters
            simpleRenderSystem.renderGameObjects(frameInfo);
            pointLightSystem.render(frameInfo);

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
    flatVase.transform.translation = {-.5f, .5f, 0.0f};
    flatVase.transform.scale = {3.f, 1.5f, 3.f};
    gameObjects_.emplace(flatVase.getId(), std::move(flatVase));

    model = Model::createModelFromFile(device_, "models/smooth_vase.obj");
    auto smoothVase = GameObject::createGameObject();
    smoothVase.model = model;
    smoothVase.transform.translation = {.5f, .5f, 0.0f};
    smoothVase.transform.scale = {3.f, 1.5f, 3.f};
    gameObjects_.emplace(smoothVase.getId(), std::move(smoothVase));

    model = Model::createModelFromFile(device_, "models/quad.obj");
    auto floor = GameObject::createGameObject();
    floor.model = model;
    floor.transform.translation = {0.f, .5f, 0.f};
    floor.transform.scale = {3.f, 1.f, 3.f};
    gameObjects_.emplace(floor.getId(), std::move(floor));

    std::vector<glm::vec3> lightColors{
      {1.f, .1f, .1f}, {.1f, .1f, 1.f}, {.1f, 1.f, .1f}, {1.f, 1.f, .1f}, {.1f, 1.f, 1.f}, {1.f, 1.f, 1.f} //
    };

    for (int i = 0; i < lightColors.size(); i++)
    {
        auto pointLight = GameObject::makePointLight(0.2f);
        pointLight.color = lightColors[i];
        auto rotateLight =
          glm::rotate(glm::mat4(1.f), (i * glm::two_pi<float>()) / lightColors.size(), {0.f, -1.f, 0.f});
        pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
        gameObjects_.emplace(pointLight.getId(), std::move(pointLight));
    }
}
