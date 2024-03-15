#include "first_app.hpp"

FirstApp::FirstApp()
{
    createPipeline();
}

void FirstApp::run()
{
    while (!window_.shouldClose())
    {
        glfwPollEvents();
    }
}

void FirstApp::createPipeline()
{
    PipelineConfigInfo pipelineConfig{};
    Pipeline::defaultPipelineConfigInfo(pipelineConfig, WIDTH, HEIGHT);
    Pipeline pipeline_{device_, "simple_shader.vert.spv", "simple_shader.frag.spv", pipelineConfig};
}
