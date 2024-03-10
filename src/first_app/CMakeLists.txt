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
