#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#if defined(_WIN32)
    #include "loadShaders.h"
    #include "SOIL.h"
#else
    #include "external/loadShaders.h"
    #include "external/soil/include/SOIL/SOIL.h"
#endif

class Util
{
public:
    static void loadTexture(const char* texturePath, GLuint& texture);
};

