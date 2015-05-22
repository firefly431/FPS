#pragma once

#include "Mesh.h"
#include "Player.h"

// mesh with model uniform so it can be moved/rotated
class PlayerMesh : public Mesh {
public:
    PlayerMesh(ShaderProgram &&prog, VertexArray &&vao, Texture &&tex);
    PlayerMesh(PlayerMesh &&move);
    void update(const Player &player); // update model
protected:
    // vertex data
    GLfloat model[16];
    // uniform index
    GLint model_loc;
};

