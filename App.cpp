#include "App.hpp"

glm::mat4 App::resize_matrix;
PoolTable* App::pool_table = nullptr;
Ball* App::ball = nullptr;

void App::cleanup()
{
	delete pool_table;
	pool_table = nullptr;

	delete ball;
	ball = nullptr;
}

void App::init(int argc, char** argv)
{
	resize_matrix = glm::ortho(0.0f, (float)WIN_WIDTH, 0.0f, (float)WIN_HEIGHT);

    initWindow(argc, argv);

    glewInit();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	pool_table = new PoolTable();
	ball = new Ball();

	glutDisplayFunc(render);
	glutIdleFunc(render);

	glutCloseFunc(cleanup);

	glutMainLoop();
}

void App::initWindow(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
    glutInitWindowPosition(0, 0);				
    glutCreateWindow("Biliard");
}

void App::render()
{
    glClear(GL_COLOR_BUFFER_BIT);	

	if (pool_table) {
		pool_table->render(resize_matrix);
	}

	if (ball) {
		ball->render(resize_matrix);
	}
	
	glutSwapBuffers();
	glFlush();
}
