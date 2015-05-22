#include "SpearMesh.h"

#include <cmath>
#include "Player.h"

const double SpearMesh::Z = 0.8;

SpearMesh::SpearMesh(ShaderProgram &&prog, VertexArray &&vao, Texture &&tex)
: Mesh(std::move(prog), std::move(vao), std::move(tex)), model() {
    this->prog.activate();
    // model location
    model_loc = this->prog.getUniformLocation("model");
    model[0] = model[5] = model[10] = model[15] = 1;
}

SpearMesh::SpearMesh(SpearMesh &&move) : Mesh(std::move(move)), model_loc(move.model_loc) {
    memcpy(model, move.model, sizeof(GLfloat) * 16);
    move.model_loc = 0;
}

void SpearMesh::update(const Spear &spear) {
    // recalculate matrix
    // the mesh is rotated
    vector cs = spear.velocity.normalized();
    const GLfloat cos = -cs.y, sin = cs.x;
    model[0]  =  cos;
    model[1]  =  sin;
    model[4]  = -sin;
    model[5]  =  cos;
    model[12] =  spear.head.x;
    model[13] =  spear.head.y;
    model[14] =  Z;
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, model);
}
