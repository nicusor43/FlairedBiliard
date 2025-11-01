#include "Util.hpp"

#include <iostream>

void Util::loadTexture(const char *texturePath, GLuint &texture)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height;

    unsigned char *image = SOIL_load_image(texturePath, &width, &height, 0, SOIL_LOAD_RGBA);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        image);

    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// TODO: Cam nebunie ce se intampla aici
std::vector<glm::vec2> Util::triangleRackPositions(unsigned rows, float r, float padding, glm::vec2 origin)
{
    std::swap(origin.x, origin.y);

    std::vector<glm::vec2> positions;
    positions.reserve((rows * (rows + 1)) / 2);

    const float r_eff = r + padding * 0.5f;
    const float dy = 1.5f * r_eff;
    const float dx = std::sqrt(3.0f) * r_eff;

    const float triangle_width = dx * rows;
    const float triangle_height = dy * (rows - 1);

    const float apex_x = origin.x - (triangle_width - dx) * 0.5f;
    const float apex_y = origin.y - triangle_height * 0.5f;

    for (unsigned row = 0; row < rows; ++row)
    {
        unsigned count = row + 1;

        float row_width = dx * count;
        float row_start_x = apex_x + (triangle_width - row_width) * 0.5f;

        float y = apex_y + row * dy;
        for (unsigned i = 0; i < count; ++i)
        {
            float x = row_start_x + i * dx;
            positions.emplace_back(y, x);
        }
    }

    return positions;
}

float Util::distance(glm::vec2 a, glm::vec2 b)
{
    return sqrtf((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

float Util::rotationToPoint(glm::vec2 a, glm::vec2 b)
{
    return atan2f(b.y - a.y, b.x - a.x);
}
