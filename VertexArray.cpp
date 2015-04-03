#include "VertexArray.h"

VertexArray::VertexArray(GLenum mode, GLsizei size) : id(0), buffers(), mode(mode), size(size) {
    glGenVertexArrays(1, &id);
}

VertexArray::VertexArray(VertexArray &&move) : id(move.id), buffers(std::move(move.buffers)), mode(move.mode), size(move.size) {
    move.id = 0;
    move.mode = 0;
    move.size = 0;
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
    buffers[i] = std::unique_ptr<VertexBuffer>(&buf);
    glBindBuffer(GL_ARRAY_BUFFER, buf.id);
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, size, type, GL_FALSE, stride, 0);
}

void VertexArray::draw() {
    glDrawArrays(mode, 0, size);
}
