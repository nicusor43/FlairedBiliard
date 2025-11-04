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
#include "external/soil/include/SOIL/SOIL.h"

#include "Util.hpp"

#include <cmath>
#include <string>

class Ball
{
public:
    Ball(unsigned int ball_number, glm::vec2 position);
    ~Ball();

    static constexpr float RADIUS = 25.f;

    glm::vec2 position = glm::vec2{0.f, 0.f};
    glm::vec2 velocity = glm::vec2{0.f, 0.f};

    // Unghiul este in radiani
    float angle = 0.f;
    float angular_velocity = 0.f;

    void update(float dt);
    void render(glm::mat4 resize_matrix);
private:
    glm::mat4 transform = glm::mat4{1.f};

    GLuint vao_id, vbo_id, ebo_id, texture, program_id, my_matrix_location;
    void createVBO();
};
