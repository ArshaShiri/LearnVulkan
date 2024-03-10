#include "first_app.hpp"

void FirstApp::run()
{
    while (!window_.shouldClose())
    {
        glfwPollEvents();
    }
}
