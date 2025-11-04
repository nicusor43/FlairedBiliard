#include "Cue.hpp"
#include "Util.hpp"
#include "Ball.hpp"
#include "external/loadShaders.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <cstdio>
#include <GL/freeglut.h>

Cue::Cue()
{
    // Shader pentru linie/punct
    line_program = LoadShaders("line.vert", "line.frag");
    line_matrix_loc = glGetUniformLocation(line_program, "uMatrix");
    line_color_loc  = glGetUniformLocation(line_program, "uColor");

    createVBO();
    initLineVAO(); // VAO + VBO pentru linie/marker

    Util::loadTexture("Assets/pool_cue.png", this->texture);

    // Shader pentru tac
    program_id = LoadShaders("shader.vert", "shader.frag");
    my_matrix_location = glGetUniformLocation(program_id, "myMatrix");

    position = glm::vec2(0.0f, 0.0f);
}

Cue::~Cue()
{
    if (program_id) glDeleteProgram(program_id);
    if (line_program) glDeleteProgram(line_program);
    if (texture) glDeleteTextures(1, &texture);
    if (ebo_id) glDeleteBuffers(1, &ebo_id);
    if (vbo_id) glDeleteBuffers(1, &vbo_id);
    if (vao_id) glDeleteVertexArrays(1, &vao_id);
    if (line_vbo) glDeleteBuffers(1, &line_vbo);
    if (line_vao) glDeleteVertexArrays(1, &line_vao);
}

void Cue::createVBO()
{
    const float half_h = thickness * 0.5f;
    GLfloat vertices[] = {
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

    glBindVertexArray(0);
}

void Cue::initLineVAO() {
    glGenVertexArrays(1, &line_vao);
    glGenBuffers(1, &line_vbo);

    glBindVertexArray(line_vao);
    glBindBuffer(GL_ARRAY_BUFFER, line_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4, nullptr, GL_DYNAMIC_DRAW); // 2 puncte x,y
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
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
    glBindVertexArray(0);

    drawLineAndMarker(center, aim_pos, len, ang, resize_matrix);
}

// --- Funcția linie/punct cu GLUT ---
void Cue::drawLineAndMarker(const glm::vec2& center, const glm::vec2& aim_pos,
                            float len, float angle, const glm::mat4& proj_matrix)
{
    glm::vec2 dir_n = len > 1e-4f ? glm::normalize(aim_pos - center)
                                  : glm::vec2(cosf(angle), sinf(angle));

    const float GAP_BEHIND_BALL = 20.0f;
    float ball_radius = Ball::RADIUS;
    glm::vec2 line_start = center - dir_n * (ball_radius + GAP_BEHIND_BALL);

    const float MAX_LINE_LENGTH = 150.0f;
    float current_line_length = glm::clamp(len, 0.0f, MAX_LINE_LENGTH);
    glm::vec2 line_end = line_start - dir_n * current_line_length;

    // --- Dimensiunea ferestrei cu GLUT ---
    int screen_width  = glutGet(GLUT_WINDOW_WIDTH);
    int screen_height = glutGet(GLUT_WINDOW_HEIGHT);

    auto toNDC = [&](glm::vec2 pos) -> glm::vec2 {
    float x = 2.0f * pos.x / float(screen_width) - 1.0f;
    float y = 2.0f * pos.y / float(screen_height) - 1.0f; // schimbăm semnul aici
    return glm::vec2(x, y);
};

    glm::vec2 ndc_start = toNDC(line_start);
    glm::vec2 ndc_end   = toNDC(line_end);
    glm::vec2 ndc_point = toNDC(aim_pos);

    float line_vertices[] = {
        ndc_start.x, ndc_start.y,
        ndc_end.x,   ndc_end.y
    };

    float point_vertex[] = { ndc_point.x, ndc_point.y };

    glUseProgram(line_program);
    glBindVertexArray(line_vao);
    glBindBuffer(GL_ARRAY_BUFFER, line_vbo);

    // Linie albă
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(line_vertices), line_vertices);
    glUniform4f(line_color_loc, 1.f, 1.f, 1.f, 0.8f);
    glLineWidth(2.0f);
    glDrawArrays(GL_LINES, 0, 2);

    glBindVertexArray(0);
    glUseProgram(0);
}
