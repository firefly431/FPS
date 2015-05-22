#pragma once

#include "Shader.h"

/*
 * attribute indices: see VertexAttribute.h
 * must be named position, normal, texcoord
 * color out is named color_out
 */
// a shader program, represents a set of linked shaders
class ShaderProgram {
    friend class Mesh;
protected:
    GLuint id;
public:
    ShaderProgram(VertexShader &&v, FragmentShader &&f);
    ShaderProgram(ShaderProgram &&move); // move ctor
    ~ShaderProgram();
    ShaderProgram &operator=(ShaderProgram &&move);
    void activate();
    static void deactivate();
    // must be active
    // gets the index in GPU memory of the uniform specified
    GLint getUniformLocation(const char *name);
    // must be active
    // set the sampler with the name to the texture unit idx
    void setSampler(const char *name, GLint idx);
};

