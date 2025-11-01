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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <vector>

class Util
{
public:
    static const int WIN_WIDTH = 1440;
    static const int WIN_HEIGHT = 810;

    static void loadTexture(const char* texturePath, GLuint& texture);
    static std::vector<glm::vec2> triangleRackPositions(unsigned rows, float r, float padding, glm::vec2 origin);
};

