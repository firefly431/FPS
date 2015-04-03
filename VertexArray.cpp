#include "VertexArray.h"

VertexArray::VertexArray() : id(0), buffers() {
    glGenVertexArrays(1, &id);
}

VertexArray::VertexArray(VertexArray &&move) : id(move.id), buffers(std::move(move.buffers)) {
    move.id = 0;
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
void VertexArray::bindBuffer(VertexBuffer &&buf, std::size_t i) {
    buffers[i] = std::unique_ptr<VertexBuffer>(&buf);
    glBindBuffer(GL_ARRAY_BUFFER, buf.id);
}
