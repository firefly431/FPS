#pragma once

#include "Mesh.h"
#include "Spear.h"

// mesh for a spear, slightly different from player mesh
class SpearMesh : public Mesh {
public:
    SpearMesh(ShaderProgram &&prog, VertexArray &&vao, Texture &&tex);
    SpearMesh(SpearMesh &&move);
    void update(const Spear &spear); // update model
    static const double Z; // Z position of all spears
protected:
    GLfloat model[16];
    GLint model_loc; // index of model in memory
};

