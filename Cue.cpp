#include "Cue.hpp"
#include "Util.hpp"
#include "Ball.hpp"
#include "external/loadShaders.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <cstdio>

Cue::Cue()
{
    createVBO();

    Util::loadTexture("Assets/pool_cue.png", this->texture);

    program_id = LoadShaders("shader.vert", "shader.frag");
    my_matrix_location = glGetUniformLocation(program_id, "myMatrix");

    position = glm::vec2(0.0f, 0.0f);
}

Cue::~Cue()
{
    if (program_id) glDeleteProgram(program_id);
    if (texture) glDeleteTextures(1, &texture);
    if (ebo_id) glDeleteBuffers(1, &ebo_id);
    if (vbo_id) glDeleteBuffers(1, &vbo_id);
    if (vao_id) glDeleteVertexArrays(1, &vao_id);
}

void Cue::createVBO()
{
    const float half_h = thickness * 0.5f;
    GLfloat vertices[] = {
        // x, y, z, w,    r,g,b,     u,v
        0.0f,      -half_h, 0.0f, 1.0f,   1.0f,1.0f,1.0f,   0.0f, 0.0f,
        length,    -half_h, 0.0f, 1.0f,   1.0f,1.0f,1.0f,   1.0f, 0.0f,
        length,     half_h, 0.0f, 1.0f,   1.0f,1.0f,1.0f,   1.0f, 1.0f,
        0.0f,      half_h,  0.0f, 1.0f,   1.0f,1.0f,1.0f,   0.0f, 1.0f
    };

    GLuint indices[] = { 0,1,2, 0,2,3 };

    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
}

void Cue::render(const glm::mat4& resize_matrix, const glm::vec2& center, const glm::vec2& aim_pos, float dt)
{
    glm::vec2 dir = aim_pos - center;
    float len = glm::length(dir);
    float ang = angle;
    if (len > 1e-4f) {
        ang = atan2f(dir.y, dir.x);
    }

    const float gap = Ball::RADIUS + 6.0f;
    const float extra_distance = length * -0.2f;
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(center, 0.0f));
    model = glm::rotate(model, ang, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(- (gap + extra_distance), 0.0f, 0.0f));

    transform = resize_matrix * model;

    glUseProgram(program_id);
    glBindVertexArray(vao_id);

    glUniformMatrix4fv(my_matrix_location, 1, GL_FALSE, glm::value_ptr(transform));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(program_id, "myTexture"), 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}