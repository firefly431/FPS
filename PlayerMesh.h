#pragma once

#include "Mesh.h"
#include "Player.h"

class PlayerMesh : public Mesh {
public:
    static const GLfloat MATRIX_IDENTITY[];
    PlayerMesh(ShaderProgram &&prog, VertexArray &&vao);
    PlayerMesh(PlayerMesh &&move);
    void update(const Player &player); // update ubo
protected:
    GLint binding_point;
    UniformBuffer ubo;
};

