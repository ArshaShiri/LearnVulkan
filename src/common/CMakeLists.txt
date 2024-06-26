cmake_minimum_required(VERSION 3.21)

project(
    common
    VERSION 1.0
    LANGUAGES CXX
)

add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
    buffer.cpp
    camera.cpp
    descriptors.cpp
    device.cpp
    game_object.cpp
    keyboard_movement_controller.cpp
    model.cpp
    pipeline.cpp
    point_light_system.cpp
    renderer.cpp
    simple_render_system.cpp
    swap_chain.cpp
    window.cpp
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC include
)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw glm::glm Vulkan::Vulkan tinyobjloader::tinyobjloader)
