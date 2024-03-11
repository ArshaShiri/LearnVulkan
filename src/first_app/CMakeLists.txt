cmake_minimum_required(VERSION 3.21)

project(
    first_app
    LANGUAGES CXX
)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    first_app.cpp
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE common)


add_custom_command(TARGET ${PROJECT_NAME}
    PRE_BUILD
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/../shaders/simple_shaders/simple_shader.frag.spv .
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/../shaders/simple_shaders/simple_shader.vert.spv .
)
