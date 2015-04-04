#include "PlayerMesh.h"

#include <cmath>

#include "BindingPoint.h"

PlayerMesh::PlayerMesh(ShaderProgram &&prog, VertexArray &&vao)
: Mesh(std::move(prog), std::move(vao)), model() {
    model[0] = model[5] = model[10] = model[15] = 1;
}

PlayerMesh::PlayerMesh(PlayerMesh &&move) : Mesh(std::move(move)), model_loc(move.model_loc) {
    memcpy(model, move.model, sizeof(GLfloat) * 16);
    move.model_loc = 0;
}

void PlayerMesh::update(const Player &player) {
    // recalculate matrix
    GLfloat cos = std::cos(player.heading), sin = std::sin(player.heading);
    model[0]  =  cos;
    model[1]  =  sin;
    model[4]  = -sin;
    model[5]  =  cos;
    model[12] =  player.position.x;
    model[13] =  player.position.y;
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, model);
}
