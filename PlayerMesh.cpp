#include "PlayerMesh.h"

#include <cmath>

#include "BindingPoint.h"

const GLfloat PlayerMesh::MATRIX_IDENTITY[] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
};

PlayerMesh::PlayerMesh(ShaderProgram &&prog, VertexArray &&vao)
: Mesh(std::move(prog), std::move(vao)), binding_point(BindingPoint::next()),
  ubo(sizeof(GLfloat) * 16, GL_DYNAMIC_DRAW, binding_point, (void *)MATRIX_IDENTITY) {
    prog.bindBuffer("model", binding_point);
}

PlayerMesh::PlayerMesh(PlayerMesh &&move) : Mesh(std::move(move)), binding_point(move.binding_point), ubo(std::move(move.ubo)) {
    move.binding_point = 0;
}

void PlayerMesh::update(const Player &player) {
    // recalculate matrix
    GLfloat *data = (GLfloat *)ubo.data;
    GLfloat cos = std::cos(player.heading), sin = std::sin(player.heading);
    data[0]  =  cos;
    data[1]  =  sin;
    data[4]  = -sin;
    data[5]  =  cos;
    data[12] =  player.position.x;
    data[13] =  player.position.y;
    ubo.update();
}
