#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Util.hpp"
#include "Ball.hpp"

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

class Cue {
public:
    Cue();
    ~Cue();

    void createVBO();

    // resize_matrix: projection matrix (same as App::resize_matrix)
    // center: white ball position
    // aim_pos: mouse/world aim position
    // spinning: when true the cue slowly rotates around the white ball
    // dt: delta time in seconds
    void render(const glm::mat4& resize_matrix, const glm::vec2& center, const glm::vec2& aim_pos, float dt);

private:
    // GL objects
    GLuint vao_id = 0;
    GLuint vbo_id = 0;
    GLuint ebo_id = 0;
    GLuint texture = 0;
    GLuint program_id = 0;
    GLint my_matrix_location = -1;

    // transform / position state
    glm::mat4 transform = glm::mat4(1.0f);
    glm::vec2 position = glm::vec2(0.0f, 0.0f); // kept to match Cue.cpp usage

    // visual properties
    float angle = 0.0f;                 // current angle (radians)
    float spin_speed = glm::radians(60.0f); // rad/s when spinning
    float length = 420.0f;              // cue length in pixels
    float thickness = 10.0f;            // visual thickness in pixels
};