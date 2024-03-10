#include <cstdlib>
#include <exception>
#include <iostream>

#include "first_app.hpp"

int main()
{
    FirstApp app;

    try
    {
        app.run();
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
