#include "Ball.hpp"

Ball::Ball(unsigned int ball_number, glm::vec2 position)
{
    this->createVBO();
    Util::loadTexture(std::string{"Assets/ball_" + std::to_string(ball_number) + ".png"}.c_str(), this->texture);

    program_id = LoadShaders("shader.vert", "shader.frag");

    my_matrix_location = glGetUniformLocation(program_id, "myMatrix");

    this->position = position;
}

Ball::~Ball()
{
    glDeleteProgram(program_id);
    glDeleteTextures(1, &texture);
    glDeleteBuffers(1, &ebo_id);
    glDeleteBuffers(1, &vbo_id);
    glDeleteVertexArrays(1, &vao_id);
}

void Ball::createVBO()
{
    GLfloat vertices[] = {
        //	Coordonate;      Culori;     Coordonate de texturare;
        -this->RADIUS, -this->RADIUS, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,     // Stanga jos;
        +this->RADIUS, -this->RADIUS, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // Dreapta jos;
        +this->RADIUS, +this->RADIUS, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Dreapta sus;
        -this->RADIUS, +this->RADIUS, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f    // Stanga sus;
    };

    GLuint indices[] = {
        0, 1, 2,
        0, 2, 3};

    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //  Se asociaza atributul (0 = coordonate) pentru shader;
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid *)0);
    //  Se asociaza atributul (1 =  culoare) pentru shader;
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid *)(4 * sizeof(GLfloat)));
    //  Se asociaza atributul (2 =  texturare) pentru shader;
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid *)(7 * sizeof(GLfloat)));
}

void Ball::update(float dt)
{
    this->position.x += this->velocity.x * dt;
    this->position.y += this->velocity.y * dt;

    this->angle += this->angularVelocity * dt;
}

void Ball::render(glm::mat4 resize_matrix)
{
    glUseProgram(program_id);

    glBindVertexArray(vao_id);

    glm::mat4 transformata = glm::translate(glm::mat4(1.0f), glm::vec3(this->position.x, this->position.y, 0.f));
    glm::mat4 rotatia = glm::rotate(glm::mat4(1.0f), this->angle, glm::vec3(0.0f, 0.0f, 1.0f));
    transform = resize_matrix * transformata * rotatia;

    glUniformMatrix4fv(my_matrix_location, 1, GL_FALSE, &transform[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glUniform1i(glGetUniformLocation(program_id, "myTexture"), 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}