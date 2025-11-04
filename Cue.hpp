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

    void render(const glm::mat4& resize_matrix, const glm::vec2& center, const glm::vec2& aim_pos, float dt);

    float getPower() const { return current_power; };
private:
    GLuint vao_id      = 0;
    GLuint vbo_id      = 0;
    GLuint ebo_id      = 0;
    GLuint texture     = 0;
    GLuint program_id  = 0;
    GLint my_matrix_location = -1;

    glm::mat4 transform = glm::mat4(1.0f);
    glm::vec2 position  = glm::vec2(0.0f, 0.0f); 
    
    float angle       = 0.0f;                 
    float length      = 420.0f;              
    float thickness   = 10.0f;  
    
    float max_power_distance = 200.f;
    float current_power = 0.f;

    GLuint line_vao   = 0;
    GLuint line_vbo   = 0;
    GLuint line_program = 0;
    GLint  line_matrix_loc = -1;
    GLint  line_color_loc  = -1;

   void drawLineAndMarker(const glm::vec2& center, const glm::vec2& aim_pos,
                            float len, float angle, const glm::mat4& resize_matrix, float power);
    void initLineVAO();
};
