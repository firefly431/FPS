#include "Mesh.h"

Mesh::Mesh(ShaderProgram &&prog, VertexArray &&vao) : prog(std::move(prog)), vao(std::move(vao)) {}
Mesh::Mesh(Mesh &&move) : prog(std::move(move.prog)), vao(std::move(move.vao)) {}

void Mesh::activate() {
    prog.activate();
    vao.activate();
}

void Mesh::deactivate() {
    ShaderProgram::deactivate();
    VertexArray::deactivate();
}

void Mesh::useCamera(const Camera &camera) {
    prog.bindBuffer("view_projection", camera.getBindingPoint());
}

void Mesh::draw() {
    vao.draw();
}
