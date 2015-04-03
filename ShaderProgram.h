#pragma once

/*
 * attribute indices: see VertexAttribute.h
 */
class ShaderProgram {
protected:
    GLuint id;
public:
    ShaderProgram(VertexShader &&v, FragmentShader &&f);
    ShaderProgram(ShaderProgram &&move);
    ~ShaderProgram();
    ShaderProgram &operator=(ShaderProgram &&move);
    void activate();
    static void deactivate();
    void bindBuffer(const char *name, GLuint point);
    void setSampler(const char *name, GLint idx);
};

