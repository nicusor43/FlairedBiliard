#pragma once

#if defined(_WIN32)
    #include <windows.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#if defined(_WIN32)
    #include "loadShaders.h"
    #include "SOIL.h"
#else
    #include "external/loadShaders.h"
    #include "external/soil/include/SOIL/SOIL.h"
#endif

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "PoolTable.hpp"

#include <memory>

class App
{
public:
    static void init(int argc, char** argv);

    static const int WIN_WIDTH = 1920;
    static const int WIN_HEIGHT = 1080;

    static glm::mat4 resize_matrix;

private:
    static std::unique_ptr<PoolTable> pool_table;

    static void initWindow(int argc, char** argv);

    static void render();
};

