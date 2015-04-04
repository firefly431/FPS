#pragma once

#include "Shader.h"

/*
 * attribute indices: see VertexAttribute.h
 * must be named position, normal, texcoord
 * color out is named color_out
 */
class ShaderProgram {
    friend class Mesh; // TEMP
protected:
    GLuint id;
public:
    ShaderProgram(VertexShader &&v, FragmentShader &&f);
    ShaderProgram(ShaderProgram &&move);
    ~ShaderProgram();
    ShaderProgram &operator=(ShaderProgram &&move);
    void activate();
    static void deactivate();
    // must be active
    GLint getUniformLocation(const char *name);
    // must be active
    void setSampler(const char *name, GLint idx);
};

