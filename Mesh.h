#pragma once

class Mesh {
public:
    Mesh(ShaderProgram &&prog);
    ~Mesh();
protected:
    GLuint vbo, vao;
    ShaderProgram prog;
};

