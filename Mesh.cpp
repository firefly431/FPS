#include "Mesh.h"

Mesh::Mesh(ShaderProgram &&prog, VertexArray &&vao) : prog(std::move(prog)), vao(std::move(vao)) {
    this->prog.activate();
    view_loc = this->prog.getUniformLocation("view");
    proj_loc = this->prog.getUniformLocation("projection");
    ShaderProgram::deactivate();
}

Mesh::Mesh(Mesh &&move) : prog(std::move(move.prog)), vao(std::move(move.vao)), view_loc(move.view_loc), proj_loc(move.proj_loc) {
    move.view_loc = move.proj_loc = 0;
}

void Mesh::activate() {
    prog.activate();
    vao.activate();
}

void Mesh::deactivate() {
    ShaderProgram::deactivate();
    VertexArray::deactivate();
}

void Mesh::updateVP(const Camera &camera) {
    camera.setViewUniform(view_loc);
    camera.setProjUniform(proj_loc);
}

void Mesh::draw() {
    vao.draw();
}
