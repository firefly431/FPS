#pragma once

/*
 * attribute indices: see VertexAttribute.h
 */
class VertexArray {
protected:
    GLuint id;
    std::array<std::unique_ptr<VertexBuffer>, ATTRIBUTE_NUM> buffers;
    GLenum mode;
    GLsizei size;
public:
    VertexArray(GLenum mode, GLsizei size);
    VertexArray(VertexArray &&move);
    ~VertexArray();
    VertexArray &operator=(VertexArray &&move);
    void activate();
    static void deactivate();
    void bindBuffer(VertexBuffer &&buf, std::size_t i, GLint size, GLenum type, GLsizei stride); // must be active
    void draw();
};

