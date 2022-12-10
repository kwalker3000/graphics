
cmake_minimum_required(VERSION 3.20.5)

project(MyGraphics)

add_executable(MyGraphics src/main.cpp)

add_subdirectory(external/glfw)
add_subdirectory(external/glew2.0)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${GLUT_INCLUDE_DIRS} )

target_include_directories(${PROJECT_NAME} PUBLIC external/glfw/include)
target_include_directories(${PROJECT_NAME} PUBLIC external/glew2.0/include)
target_link_libraries(${PROJECT_NAME} glfw libglew_static OpenGL::GL GLUT::GLUT)

# https://github.com/Perlmint/glew-cmake

