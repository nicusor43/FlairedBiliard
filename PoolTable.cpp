#include "PoolTable.hpp"

PoolTable::PoolTable()
{
	this->createVBO();
	Util::loadTexture("Assets/table.png", this->texture);

	program_id = LoadShaders("shader.vert", "shader.frag");

	my_matrix_location = glGetUniformLocation(program_id, "myMatrix");
}

void PoolTable::render(glm::mat4 resize_matrix) {
	glUseProgram(program_id);

	glBindVertexArray(vao_id);

	glUniformMatrix4fv(my_matrix_location, 1, GL_FALSE, &resize_matrix[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glUniform1i(glGetUniformLocation(program_id, "myTexture"), 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

PoolTable::~PoolTable()
{
	glDeleteProgram(program_id);
	glDeleteTextures(1, &texture);
	glDeleteBuffers(1, &ebo_id);
	glDeleteBuffers(1, &vbo_id);
	glDeleteVertexArrays(1, &vao_id);
}

void PoolTable::createVBO()
{
	GLfloat vertices[] = {
		//	Coordonate;					Culori;				Coordonate de texturare;
		   0.0f,   0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f,	// Stanga jos;
		   1920.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 0.0f,	1.0f, 0.0f, // Dreapta jos;
		   1920.0f,1080.0f, 0.0f, 1.0f,   0.0f, 0.0f, 0.0f,	1.0f, 1.0f,	// Dreapta sus;
		   0.0f, 1080.0f, 0.0f, 1.0f,   0.0f, 0.0f, 0.0f,	0.0f, 1.0f  // Stanga sus;
	};


	GLuint indices[] = {
	  0, 1, 2,
	  0, 2, 3
	};

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
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
	//  Se asociaza atributul (1 =  culoare) pentru shader;
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
	//  Se asociaza atributul (2 =  texturare) pentru shader;
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
}