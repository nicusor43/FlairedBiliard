#include "App.hpp"

glm::mat4 App::resize_matrix;
PoolTable *App::pool_table = nullptr;
std::vector<Ball*> App::balls;

void App::cleanup()
{
    delete pool_table;
    pool_table = nullptr;

    for (auto ball : balls)
        delete ball;
}

void App::init(int argc, char **argv)
{
    resize_matrix = glm::ortho(0.0f, static_cast<float>(Util::WIN_WIDTH), 0.0f, static_cast<float>(Util::WIN_HEIGHT));

    initWindow(argc, argv);

    glewInit();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    pool_table = new PoolTable();

    auto positions = Util::triangleRackPositions(5, Ball::RADIUS, 10.f,
        glm::vec2{ Util::WIN_WIDTH * (2.f / 3.f), Util::WIN_HEIGHT / 2.f });
    for (int i = 0; i < 15; ++i)
        balls.push_back(new Ball(i + 1, positions[i]));

    glutDisplayFunc(render);
    glutIdleFunc(render);

    glutCloseFunc(cleanup);

    glutMainLoop();
}

void App::initWindow(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(Util::WIN_WIDTH, Util::WIN_HEIGHT);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Biliard");
}

void App::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    pool_table->render(resize_matrix);

    for (auto ball : balls)
        ball->render(resize_matrix);

    glutSwapBuffers();
    glFlush();
}
