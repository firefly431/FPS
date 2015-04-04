#include "VertexArray.h"

VertexArray::VertexArray(GLsizei size, GLenum mode) : id(0), buffers(), indices(), size(size), mode(mode), index_type(0) {
    glGenVertexArrays(1, &id);
}

VertexArray::VertexArray(VertexArray &&move) : id(move.id), buffers(std::move(move.buffers)), indices(std::move(move.indices)), size(move.size), mode(move.mode), index_type(move.index_type) {
    move.id = 0;
    move.size = 0;
    move.mode = 0;
    move.index_type = 0;
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &id);
}

VertexArray &VertexArray::operator=(VertexArray &&move) {
    if (this != &move) {
        this->~VertexArray();
        id = move.id;
        move.id = 0;
        buffers = std::move(move.buffers);
        indices = std::move(move.indices);
        size = move.size;
        move.size = 0;
        mode = move.mode;
        move.mode = 0;
        index_type = move.index_type;
        move.index_type = 0;
    }
    return *this;
}

void VertexArray::activate() {
    glBindVertexArray(id);
}

void VertexArray::deactivate() {
    glBindVertexArray(0);
}

// assume already active
// small performance gain when binding multiple buffers
// most common use case
void VertexArray::bindBuffer(VertexBuffer &&buf, std::size_t i, GLint size, GLenum type, GLsizei stride) {
    buffers[i] = std::unique_ptr<VertexBuffer>(new VertexBuffer(std::move(buf)));
    buf.activate();
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, size, type, GL_FALSE, stride, 0);
}

void VertexArray::bindBuffer(IndexBuffer &&buf, GLenum index_type) {
    indices = std::unique_ptr<IndexBuffer>(new IndexBuffer(std::move(buf)));
    buf.activate();
    this->index_type = index_type;
}

void VertexArray::draw() {
    if (indices)
        glDrawElements(mode, size, index_type, 0);
    else
        glDrawArrays(mode, 0, size);
}
