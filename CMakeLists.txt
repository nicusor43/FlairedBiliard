cmake_minimum_required(VERSION 3.10)

project(biliard)

find_package(OpenGL REQUIRED)
find_library(FREEGLUT_LIBRARY glut)
find_library(GLEW_LIBRARY GLEW)

add_subdirectory(external/soil)

add_executable(biliard main.cpp
    App.hpp App.cpp
    PoolTable.hpp PoolTable.cpp
    Util.hpp Util.cpp
    Ball.hpp Ball.cpp
    external/loadShaders.h external/loadShaders.cpp)

target_link_libraries(biliard PRIVATE
    ${OPENGL_gl_LIBRARY}
    ${FREEGLUT_LIBRARY}
    ${GLEW_LIBRARY}
    soil::soil)

target_include_directories(biliard PRIVATE /usr/include/glm)
target_include_directories(biliard PRIVATE external/soil/include)