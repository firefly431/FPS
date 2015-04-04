#pragma once

#include "ShaderProgram.h"
#include "VertexArray.h"
#include "Camera.h"

class Mesh {
public:
    Mesh(ShaderProgram &&prog, VertexArray &&vao);
    Mesh(Mesh &&move);
    void activate();
    static void deactivate();
    void updateVP(const Camera &camera); // must be active
    void draw();
protected:
    ShaderProgram prog;
    VertexArray vao;
    GLint view_loc, proj_loc;
};

