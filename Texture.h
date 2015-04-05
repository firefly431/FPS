#pragma once

#include "OpenGL.h"
#include <string>

class Texture {
protected:
    GLuint id;
    int texture;
public:
    // can't do my usual thing with the inputstream here
    Texture(const std::string &fname, int texture = 0);
    Texture(Texture &&move);
    ~Texture();
    void activate();
    static void deactivate(const Texture &tex);
    static void deactivate(const int texture);
    static void deactivate();
    int getUnit();
};
