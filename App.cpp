#include "App.hpp"

glm::mat4 App::resize_matrix;
std::unique_ptr<PoolTable> App::pool_table;

void App::init(int argc, char** argv)
{
    resize_matrix = glm::ortho(0, WIN_WIDTH, 0, WIN_HEIGHT);

    initWindow(argc, argv);

    glewInit();

    glutDisplayFunc(render);
    glutIdleFunc(render);

    pool_table = std::make_unique<PoolTable>();

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

    glutSwapBuffers();
    glFlush();
}
