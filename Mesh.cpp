#include "Mesh.h"

Mesh::Mesh(ShaderProgram &&prog, VertexArray &&vao, Texture &&tex) : prog(std::move(prog)), vao(std::move(vao)), tex(std::move(tex)) {
    this->prog.activate();
    // get location of shader uniforms
    view_loc = this->prog.getUniformLocation("view");
    proj_loc = this->prog.getUniformLocation("projection");
    // set the shader's sampler to our texture
    this->prog.setSampler("tex", tex.getUnit());
    ShaderProgram::deactivate();
}

Mesh::Mesh(Mesh &&move) : prog(std::move(move.prog)), vao(std::move(move.vao)), tex(std::move(move.tex)), view_loc(move.view_loc), proj_loc(move.proj_loc) {
    move.view_loc = move.proj_loc = 0;
}

void Mesh::activate() {
    tex.activate();
    prog.activate();
    vao.activate();
}

void Mesh::deactivate() {
    VertexArray::deactivate();
    ShaderProgram::deactivate();
    Texture::deactivate();
}

void Mesh::updateVP(const Camera &camera) {
    // update view and projection matrices from camera
    camera.setViewUniform(view_loc);
    camera.setProjUniform(proj_loc);
}

void Mesh::draw() {
    vao.draw();
}
