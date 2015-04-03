#pragma once

#include "OpenGL.h"

#include <cstddef>
#include <cstdlib>
#include <cstring>

template<GLenum BType>
class Buffer {
    friend class VertexArray;
protected:
    GLsizeiptr size;
    GLuint id;
public:
    typedef Buffer<BType> TBuffer;
    void *data;
    Buffer(GLsizeiptr size, GLenum usage, GLuint binding_point = 0, void *copy=NULL) : size(size), id(0), data(std::malloc(size)) {
        glGenBuffers(1, &id);
        glBindBuffer(BType, id);
        if (copy != NULL)
            std::memcpy(data, copy, size);
        glBufferData(BType, size, data, usage);
        if (BType == GL_UNIFORM_BUFFER)
            glBindBufferBase(BType, binding_point, id);
    }
    Buffer(TBuffer &&move) {
        id = move.id;
        move.id = 0;
        data = move.data;
        move.data = 0;
        size = move.size;
        move.size = 0;
    }
    ~Buffer() {
        glDeleteBuffers(1, &id);
        free(data);
    }
    TBuffer &operator=(TBuffer &&move) {
        if (this != &move) {
            this->~Buffer();
            id = move.id;
            move.id = 0;
            data = move.data;
            move.data = 0;
            size = move.size;
            move.size = 0;
        }
        return *this;
    }
    void activate() {
        glBindBuffer(BType, id);
    }
    static void deactivate() {
        glBindBuffer(BType, 0);
    }
    void update(GLintptr offset, GLsizeiptr size) {
        glBufferSubData(BType, offset, size, data);
    }
    void update() {
        glBufferSubData(BType, 0, size, data);
    }
};

extern template class Buffer<GL_UNIFORM_BUFFER>;
extern template class Buffer<GL_ARRAY_BUFFER>;

typedef Buffer<GL_UNIFORM_BUFFER> UniformBuffer;
typedef Buffer<GL_ARRAY_BUFFER> ArrayBuffer;
typedef ArrayBuffer VertexBuffer;
