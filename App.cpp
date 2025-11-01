#include "App.hpp"

float App::delta_time = 0.f;
std::chrono::time_point<std::chrono::steady_clock> App::last_frame_time = std::chrono::steady_clock::now();

glm::mat4 App::resize_matrix;
PoolTable *App::pool_table = nullptr;
std::vector<Ball *> App::balls;
bool App::ballHit = false;

void App::cleanup()
{
    delete pool_table;
    pool_table = nullptr;

    for (auto ball : balls)
        delete ball;
}

void App::handleInput(unsigned char key, int x, int y)
{
    if (key == ' ' && !ballHit)
    {
        balls.back()->velocity.x = 1000.f;
        balls.back()->velocity.y = 10.f;
        ballHit = true;
    }
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
                                                 glm::vec2{Util::WIN_WIDTH * (2.f / 3.f), Util::WIN_HEIGHT / 2.f});
    for (int i = 0; i < 15; ++i)
        balls.push_back(new Ball(i + 1, positions[i]));
    balls.push_back(new Ball(16, glm::vec2{Util::WIN_WIDTH / 5.f, Util::WIN_HEIGHT / 2.f}));

    glutDisplayFunc(render);
    glutIdleFunc(update);
    glutKeyboardFunc(handleInput);

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

void App::ballsInteraction()
{
    // O(n^2)
    for (unsigned int i = 0; i < balls.size() - 1; ++i)
    {
        for (unsigned int j = i + 1; j < balls.size(); ++j)
        {
            auto b1 = balls[i];
            auto b2 = balls[j];

            const auto distance = Util::distance(b1->position, b2->position);

            // Not a collision
            if (distance > 2.f * Ball::RADIUS)
                continue;

            const auto v1 = sqrtf(b1->velocity.x * b1->velocity.x + b1->velocity.y * b1->velocity.y);
            const auto v2 = sqrtf(b2->velocity.x * b2->velocity.x + b2->velocity.y * b2->velocity.y);

            const auto theta1 = atan2f(b1->velocity.y, b1->velocity.x);
            const auto theta2 = atan2f(b2->velocity.y, b2->velocity.x);

            const auto phi = Util::rotationToPoint(b1->position, b2->position);

            b1->velocity.x = v2 * cosf(theta2 - phi) * cosf(phi) + v1 * sinf(theta1 - phi) * cosf(phi + Util::PI / 2.f);
            b1->velocity.y = v2 * cosf(theta2 - phi) * sinf(phi) + v1 * sinf(theta1 - phi) * sinf(phi + Util::PI / 2.f);

            b2->velocity.x = v1 * cosf(theta1 - phi) * cosf(phi) + v2 * sinf(theta2 - phi) * cosf(phi + Util::PI / 2.f);
            b2->velocity.y = v1 * cosf(theta1 - phi) * sinf(phi) + v2 * sinf(theta2 - phi) * sinf(phi + Util::PI / 2.f);

            const auto half_distance = Ball::RADIUS - distance / 2.f;
            b1->position.x += cosf(phi + Util::PI) * half_distance;
            b1->position.y += sinf(phi + Util::PI) * half_distance;

            b2->position.x += cosf(phi) * half_distance;
            b2->position.y += sinf(phi) * half_distance;
        }
    }
}

// This is probably pretty broken
void App::update()
{
    using namespace std::chrono;

    auto current_time = steady_clock::now();
    delta_time = static_cast<float>(duration_cast<milliseconds>(current_time - last_frame_time).count()) / 1000.f;
    last_frame_time = current_time;

    ballsInteraction();
    for (const auto ball : balls)
        ball->update(delta_time);

    // 60fps
    std::this_thread::sleep_for(16ms);

    glutPostRedisplay();
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