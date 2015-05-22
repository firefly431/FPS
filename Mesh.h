#pragma once

#include "ShaderProgram.h"
#include "VertexArray.h"
#include "Camera.h"
#include "Texture.h"

// mesh, contains all info necessary to draw
class Mesh {
public:
    Mesh(ShaderProgram &&prog, VertexArray &&vao, Texture &&tex);
    Mesh(Mesh &&move);
    void activate();
    static void deactivate();
    void updateVP(const Camera &camera); // must be active
    void draw(); // must be active
protected:
    ShaderProgram prog;
    VertexArray vao;
    Texture tex;
    GLint view_loc, proj_loc;
};

