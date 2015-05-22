#pragma once

#include "OpenGL.h"

#include <cstddef>
#include <cstdlib>
#include <cstring>

// use a templace
// there are vertex buffers, index buffers, all sorts
// they should be different types
template<GLenum BType>
class Buffer {
    friend class VertexArray;
protected:
    GLsizeiptr size;
    GLuint id;
public:
    // typedef ourselves
    typedef Buffer<BType> TBuffer;
    // data buffer
    void *data;
    Buffer(GLsizeiptr size, GLenum usage, void *copy=NULL) : size(size), id(0), data(std::malloc(size)) {
        glGenBuffers(1, &id);
        glBindBuffer(BType, id);
        if (copy != NULL)
            std::memcpy(data, copy, size);
        glBufferData(BType, size, data, usage);
    }
    Buffer(TBuffer &&move) {
        // set things to zero so we don't destroy the real thing
        id = move.id;
        move.id = 0;
        data = move.data;
        move.data = 0;
        size = move.size;
        move.size = 0;
    }
    ~Buffer() {
        glDeleteBuffers(1, &id);
        std::free(data);
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
    // update the data
    void update(GLintptr offset, GLsizeiptr size) {
        glBufferSubData(BType, offset, size, (char *)data + offset);
    }
    // update the whole thing
    void update() {
        glBufferSubData(BType, 0, size, data);
    }
};

// don't initialize the template in each file (wasteful)
extern template class Buffer<GL_ARRAY_BUFFER>;
extern template class Buffer<GL_ELEMENT_ARRAY_BUFFER>;

// useful typedefs
typedef Buffer<GL_ARRAY_BUFFER> ArrayBuffer;
typedef ArrayBuffer VertexBuffer;
typedef Buffer<GL_ELEMENT_ARRAY_BUFFER> ElementArrayBuffer;
typedef ElementArrayBuffer IndexBuffer;
