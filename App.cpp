#include "App.hpp"

float App::delta_time = 0.f;
std::chrono::time_point<std::chrono::steady_clock> App::last_frame_time = std::chrono::steady_clock::now();

glm::mat4 App::resize_matrix;
PoolTable *App::pool_table = nullptr;
std::vector<Ball *> App::balls;
bool App::ball_hit = false;
Ball* App::white_ball = nullptr;

static Cue* cue = nullptr;
static glm::vec2 aim_pos = glm::vec2{0.f, 0.f};

void App::cleanup()
{
    delete pool_table;
    pool_table = nullptr;

    for (auto ball : balls)
        delete ball;

    delete cue;
    cue = nullptr;
}

static void passiveMouseCallback(int x, int y)
{
    y = Util::WIN_HEIGHT - y;
    aim_pos = glm::vec2{static_cast<float>(x), static_cast<float>(y)};
}

void App::handleKeyboardInput(unsigned char key, int x, int y)
{
}

void App::handleMouseInput(int button, int state, int x, int y)
{
    y = Util::WIN_HEIGHT - y;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        if (!allBallsStopped())
            return;

        glm::vec2 target = aim_pos;
        if (glm::length(target - white_ball->position) < 1e-3f)
            target = glm::vec2{static_cast<float>(x), static_cast<float>(y)};

        const auto angle = Util::rotationToPoint(target, white_ball->position);

        float power = glm::max(0.2f, cue->getPower()); // Minimum 10% power
        float shot_speed = WHITE_BALL_HIT_SPEED * power;
 

        white_ball->velocity.x = shot_speed * cosf(angle);
        white_ball->velocity.y = shot_speed * sinf(angle);
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
    white_ball = balls.back();

    cue = new Cue();
    aim_pos = white_ball->position + glm::vec2{100.f, 0.f};

    glutDisplayFunc(render);
    glutIdleFunc(update);
    glutKeyboardFunc(handleKeyboardInput);
    glutMouseFunc(handleMouseInput);
    glutPassiveMotionFunc(passiveMouseCallback); 
    glutMotionFunc(passiveMouseCallback);

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

            const auto v1 = sqrtf(b1->velocity.x * b1->velocity.x + b1->velocity.y * b1->velocity.y) * COLLISION_FACTOR;
            const auto v2 = sqrtf(b2->velocity.x * b2->velocity.x + b2->velocity.y * b2->velocity.y) * COLLISION_FACTOR;

            const auto theta1 = atan2f(b1->velocity.y, b1->velocity.x);
            const auto theta2 = atan2f(b2->velocity.y, b2->velocity.x);

            const auto phi = Util::rotationToPoint(b1->position, b2->position);

            glm::vec2 tangent = glm::vec2(cosf(phi + Util::PI / 2.f), sinf(phi + Util::PI / 2.f));
            glm::vec2 relVel = glm::vec2(b2->velocity.x - b1->velocity.x, b2->velocity.y - b1->velocity.y);
            float relTangential = glm::dot(relVel, tangent); 

            constexpr float spin_factor = 0.5f; 
            if (abs(relTangential) > 0.0001f) {
                b1->angularVelocity -= spin_factor * relTangential / Ball::RADIUS;
                b2->angularVelocity += spin_factor * relTangential / Ball::RADIUS;
            }

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

// TODO: Current margin is not pixel perfect
void App::edgeCollision()
{
    constexpr float margin = 100.f;

    constexpr float top_edge = Util::WIN_HEIGHT - margin;
    constexpr float bottom_edge = margin;

    constexpr float right_edge = Util::WIN_WIDTH - margin;
    constexpr float left_edge = margin;

    const float POCKET_ALLOWANCE = Ball::RADIUS * 1.0f;    
    const float POCKET_INSIDE = Ball::RADIUS * 1.0f;  

    for (auto b : balls)
    {
        if (b->position.x + Ball::RADIUS > right_edge || b->position.x - Ball::RADIUS < left_edge) {
            b->velocity.x *= -1.f;

            if (b->position.x + Ball::RADIUS > right_edge)
                b->position.x = right_edge - Ball::RADIUS;

            else
                b->position.x = left_edge + Ball::RADIUS;
        }
         if (b->position.y + Ball::RADIUS > top_edge || b->position.y - Ball::RADIUS < bottom_edge)
        {
            b->velocity.y *= -1.f;
            if (b->position.y + Ball::RADIUS > top_edge)
                b->position.y = top_edge - Ball::RADIUS;
            else
                b->position.y = bottom_edge + Ball::RADIUS;
        }
    }
}


void App::applySurfaceFriction()
{
    constexpr float EPSILON = 5.f;
    for (auto b : balls)
    {
        b->velocity.x *= 1.f - SURFACE_FRICTION_FACTOR * delta_time;
        b->velocity.y *= 1.f - SURFACE_FRICTION_FACTOR * delta_time;

        float speed = hypotf(b->velocity.x, b->velocity.y);
        if (speed < EPSILON)
        {
            b->velocity.x = 0.0f;
            b->velocity.y = 0.0f;
        }

        b->angularVelocity *= 1.f - SURFACE_FRICTION_FACTOR * delta_time;
    }
}

bool App::allBallsStopped()
{
    auto velocity_sum = glm::vec2{0.f};
    for (auto b : balls)
    {
        velocity_sum.x += b->velocity.x;
        velocity_sum.y += b->velocity.y;
    }

    return velocity_sum.x == 0.f && velocity_sum.y == 0.f;
}

void App::update()
{
    using namespace std::chrono;

    auto current_time = steady_clock::now();
    delta_time = static_cast<float>(duration_cast<milliseconds>(current_time - last_frame_time).count()) / 1000.f;
    last_frame_time = current_time;

    ballsInteraction();
    edgeCollision();
    applySurfaceFriction();
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

    if (allBallsStopped()) {
        cue->render(resize_matrix, white_ball->position, aim_pos, delta_time);
    }
    glutSwapBuffers();
    glFlush();
}