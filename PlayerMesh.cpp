#include "PlayerMesh.h"

#include <cmath>

PlayerMesh::PlayerMesh(ShaderProgram &&prog, VertexArray &&vao, Texture &&tex)
: Mesh(std::move(prog), std::move(vao), std::move(tex)), model() {
    this->prog.activate();
    model_loc = this->prog.getUniformLocation("model");
    model[0] = model[5] = model[10] = model[15] = 1;
}

PlayerMesh::PlayerMesh(PlayerMesh &&move) : Mesh(std::move(move)), model_loc(move.model_loc) {
    memcpy(model, move.model, sizeof(GLfloat) * 16);
    move.model_loc = 0;
}

void PlayerMesh::update(const Player &player) {
    // recalculate matrix
    double a = player.getRotation();
    GLfloat cos = std::cos(a), sin = std::sin(a);
    model[0]  =  cos;
    model[1]  =  sin;
    model[4]  = -sin;
    model[5]  =  cos;
    model[12] =  player.position.x;
    model[13] =  player.position.y;
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, model);
}
