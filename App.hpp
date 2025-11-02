#pragma once

#if defined(_WIN32)
#include <windows.h>
#endif

#include <stdlib.h>
#include <stdio.h>
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

#include "PoolTable.hpp"
#include "Ball.hpp"
#include "Util.hpp"

#include <chrono>
#include <thread>
#include <vector>

class App
{
public:
    static void init(int argc, char **argv);

    static glm::mat4 resize_matrix;

private:
    static float delta_time;
    static std::chrono::time_point<std::chrono::steady_clock> last_frame_time;

    static PoolTable *pool_table;
    static std::vector<Ball*> balls;
    static bool ball_hit;
    static Ball* white_ball;

    static constexpr float COLLISION_FACTOR = 0.9f;
    static constexpr float SURFACE_FRICTION_FACTOR = 0.5f;
    static constexpr float WHITE_BALL_HIT_SPEED = 1500.f;

    static void initWindow(int argc, char **argv);
    static void cleanup();
    static void handleKeyboardInput(unsigned char key, int x, int y);
    static void handleMouseInput(int button, int state, int x, int y);

    static void ballsInteraction();
    static void edgeCollision();
    static void applySurfaceFriction();
    static void applyRotation();
    static bool allBallsStopped();

    static void update();
    static void render();
};
