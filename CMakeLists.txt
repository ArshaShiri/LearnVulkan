cmake_minimum_required(VERSION 3.21)
project(vulakn_tutorial CXX)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# This is going to be useful to link to the VS Code.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)

# add_executable(test)
# target_sources(test PRIVATE main.cpp)
# target_link_libraries(test PRIVATE glfw glm::glm Vulkan::Vulkan)
