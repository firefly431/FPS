#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(VertexShader &&v, FragmentShader &&f) {
    id = glCreateProgram();
    if (!id)
        throw ShaderCreationError();
    // attach and link
    glAttachShader(id, v.id);
    glAttachShader(id, f.id);
    glBindAttribLocation(id, 0, "position");
    glBindAttribLocation(id, 1, "normal");
    glBindAttribLocation(id, 2, "texcoord");
    glLinkProgram(id);
    // check
    GLint result = GL_FALSE, log_length;
    glGetProgramiv(id, GL_LINK_STATUS, &result);
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);
    if (result == GL_FALSE) {
        if (log_length == 0)
            throw ShaderCompilationError("Unknown error");
        char *log = new char[log_length];
        glGetProgramInfoLog(id, log_length, NULL, log);
        std::string str_log(log, log_length);
        delete[] log;
        throw ShaderCompilationError(log);
    }
}

ShaderProgram::ShaderProgram(ShaderProgram &&move) {
    id = move.id;
    move.id = 0;
}

ShaderProgram::~ShaderProgram() {
    // 0 ignored
    glDeleteProgram(id);
}

ShaderProgram &operator=(ShaderProgram &&move) {
    if (this != &move) {
        glDeleteProgram(id);
        id = move.id;
        move.id = 0;
    }
    return *this;
}

void ShaderProgram::activate() {
    glUseProgram(id);
}

void ShaderProgram::deactivate() {
    glUseProgram(0);
}

void ShaderProgram::bindBuffer(const char *name, GLuint point) {
    GLuint idx = glGetUniformBlockIndex(p, name);
    glUniformBlockBinding(id, idx, point);
}

void ShaderProgram::setSampler(const char *name, GLint idx) {
    GLuint loc = glGetUniformLocation(id, name);
    glProgramUniform1i(id, loc, idx);
}
