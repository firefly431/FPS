#pragma once

#include "OpenGL.h"
#include <string>

class Texture {
protected:
    GLuint id;
    int texture;
public:
    Texture(const std::string &fname, int texture = 0);
    Texture(Texture &&move);
    ~Texture();
    void activate();
    // deactivate only deactivates a single texture unit
    // so these overloads are to allow the program to
    // deactivate multiple texture units
    static void deactivate(const Texture &tex);
    static void deactivate(const int texture);
    // since our program only uses one, this is fine
    static void deactivate();
    int getUnit();
};
