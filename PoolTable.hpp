#pragma once

#include <windows.h>
#include <stdlib.h>     
#include <stdio.h>
#include <GL/glew.h>       
#include <GL/freeglut.h>    
#include "loadShaders.h"	
#include "glm/glm.hpp"	
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "SOIL.h"	

#include "Util.hpp"

class PoolTable
{
public:
	PoolTable();	
	~PoolTable();
	void render(glm::mat4 resize_matrix);
private:
	GLuint vao_id, vbo_id, ebo_id, texture, program_id, my_matrix_location;
	void createVBO();
};

