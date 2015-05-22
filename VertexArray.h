#pragma once

#include "OpenGL.h"

#include <array>

#include "VertexAttribute.h"
#include "Buffer.h"

/*
 * attribute indices: see VertexAttribute.h
 */
class VertexArray {
protected:
    GLuint id; // OpenGL id
    std::unique_ptr<VertexBuffer> buffers[ATTRIBUTE_NUM]; // list of buffers
    std::unique_ptr<IndexBuffer> indices; // index buffer object
    GLsizei size; // number of vertices
    GLenum mode, index_type; // mode is the primitive type, index_type is the type of the index (e.g. GL_FLOAT)
public:
    VertexArray(GLsizei size, GLenum mode = GL_TRIANGLES);
    VertexArray(VertexArray &&move);
    ~VertexArray();
    VertexArray &operator=(VertexArray &&move);
    void activate();
    static void deactivate();
    void bindBuffer(VertexBuffer &&buf, std::size_t i, GLint size = 3, GLenum type = GL_FLOAT, GLsizei stride = 0); // must be active
    void bindBuffer(IndexBuffer &&buf, GLenum index_type = GL_UNSIGNED_INT);
    void draw();
};

