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

#include "PoolTable.hpp"

class App
{
public:
	static void init(int argc, char** argv);

	static const int WIN_WIDTH = 1920;
	static const int WIN_HEIGHT = 1080;

	static glm::mat4 resize_matrix;

private:
	static PoolTable* pool_table;
	static void initWindow(int argc, char** argv);
	static void render();
	static void cleanup();
};

