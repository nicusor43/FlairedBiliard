#include "Util.hpp"

void Util::loadTexture(const char* texturePath, GLuint& texture)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Setările de texturare rămân la fel
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height;

    // --- MODIFICARE 1: Încarcă 4 canale (RGBA) în loc de 3 (RGB) ---
    unsigned char* image = SOIL_load_image(texturePath, &width, &height, 0, SOIL_LOAD_RGBA);

    // --- MODIFICARE 3: Spune-i lui OpenGL să folosească formatul RGBA ---
    glTexImage2D(GL_TEXTURE_2D,
        0,                // Nivelul de Mipmap
        GL_RGBA,          // Formatul intern al texturii pe GPU (în loc de GL_RGB)
        width,            // Lățimea
        height,           // Înălțimea
        0,                // (Border - trebuie să fie 0)
        GL_RGBA,          // Formatul datelor sursă (în loc de GL_RGB)
        GL_UNSIGNED_BYTE, // Tipul de date al pixelilor
        image);           // Pointer-ul către datele imaginii

    // Eliberarea resurselor
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
}
