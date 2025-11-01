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

class Ball
{
public:
	Ball();
	~Ball();
	void render(glm::mat4 resize_matrix);
private:
	GLuint vao_id, vbo_id, ebo_id, texture, program_id, my_matrix_location;
	void createVBO();
};

