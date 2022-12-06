
cmake_minimum_required(VERSION 3.20.5)

project(MyGraphics)

add_executable(MyGraphics main.cpp)


add_subdirectory(external/glfw)

target_include_directories(${PROJECT_NAME} PUBLIC external/glfw/include)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw)


