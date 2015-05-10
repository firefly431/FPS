#pragma once

#include "Mesh.h"
#include "Spear.h"

class SpearMesh : public Mesh {
public:
    SpearMesh(ShaderProgram &&prog, VertexArray &&vao, Texture &&tex);
    SpearMesh(SpearMesh &&move);
    void update(const Spear &spear); // update model
    static const double Z;
protected:
    GLfloat model[16];
    GLint model_loc;
};

