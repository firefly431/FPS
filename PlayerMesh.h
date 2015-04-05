#pragma once

#include "Mesh.h"
#include "Player.h"

class PlayerMesh : public Mesh {
public:
    PlayerMesh(ShaderProgram &&prog, VertexArray &&vao, Texture &&tex);
    PlayerMesh(PlayerMesh &&move);
    void update(const Player &player); // update model
protected:
    GLfloat model[16];
    GLint model_loc;
};

