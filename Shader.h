#pragma once

#include "OpenGL.h"

#include <istream>
#include <string>
#include <iterator>
#include <stdexcept>
#include <fstream>

class ShaderError : public std::runtime_error {
public:
    inline explicit ShaderError(const std::string &what) : runtime_error(what) {}
};

class ShaderCreationError : public ShaderError {
public:
    inline explicit ShaderCreationError() : ShaderError("Error creating shader.") {}
};

class ShaderCompilationError : public ShaderError {
public:
    inline explicit ShaderCompilationError(const std::string &what) : ShaderError(what) {}
};

template<GLenum SType>
class Shader {
    friend class ShaderProgram;
protected:
    GLuint id;

    void load(const char *text) {
        id = glCreateShader(SType);
        if (!id)
            throw ShaderCreationError();
        // compile
        glShaderSource(id, 1, &text, NULL);
        glCompileShader(id);
        // check
        GLint result = GL_FALSE, log_length;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);
        if (result == GL_FALSE) {
            if (log_length == 0)
                throw ShaderCompilationError("Unknown error");
            char *log = new char[log_length];
            glGetShaderInfoLog(id, log_length, NULL, log);
            std::string str_log(log, log_length);
            delete[] log;
            throw ShaderCompilationError(str_log);
        }
    }
public:
    typedef Shader<SType> TShader;
    Shader(const char *text) {
        load(text);
    }
    Shader(std::istream &stream) {
        std::string text;
        stream.seekg(0, std::ios::end);
        text.resize(stream.tellg());
        stream.seekg(0, std::ios::beg);
        stream.read(&text[0], text.size());
        load(text.c_str());
    }
    Shader(const char *fname, int) { // what can you do
        std::ifstream stream(fname);
        std::string text;
        stream.seekg(0, std::ios::end);
        text.resize(stream.tellg());
        stream.seekg(0, std::ios::beg);
        stream.read(&text[0], text.size());
        load(text.c_str());
    }
    Shader(TShader &&move) {
        id = move.id;
        move.id = 0;
    }
    ~Shader() {
        // 0 is ignored
        glDeleteShader(id);
    }
    Shader &operator=(TShader &&move) {
        if (this != &move) {
            glDeleteShader(id);
            id = move.id;
            move.id = 0;
        }
        return *this;
    }
};

#ifndef _MSC_VER
extern template class Shader<GL_VERTEX_SHADER>;
extern template class Shader<GL_FRAGMENT_SHADER>;
#endif

typedef Shader<GL_VERTEX_SHADER> VertexShader;
typedef Shader<GL_FRAGMENT_SHADER> FragmentShader;
