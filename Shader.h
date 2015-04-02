#pragma once

#include <SFML/OpenGL.hpp>
#include <istream>
#include <string>
#include <iterator>
#include <stdexcept>

class ShaderError : public runtime_error {
public:
    inline explicit ShaderError(const string &what) : runtime_error(what);
}

class ShaderCreationError : public ShaderError {
public:
    inline explicit ShaderCreationError() : ShaderError("Error creating shader.");
}

class ShaderCompilationError : public ShaderError {
public:
    inline explicit ShaderCompilationError(const string &what) : ShaderError(what);
}

template<GLenum SType>
class Shader {
protected:
    void load(const char *text);
public:
    typedef Shader<SType> TShader;
    friend class ShaderProgam;
    Shader(const char *text) {
        load(text);
    }
    Shader(std::istream &stream) {
        std::string text(istream_iterator(stream), istream_iterator());
        load(text->c_str());
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
        if (this != &other) {
            glDeleteShader(id);
            id = move.id;
            move.id = 0;
        }
        return *this;
    }
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
            throw ShaderCompilationError(log);
        }
    }
};

extern template Shader<GL_VERTEX_SHADER>;
extern template Shader<GL_FRAGMENT_SHADER>;

typedef Shader<GL_VERTEX_SHADER> VertexShader;
typedef Shader<GL_FRAGMENT_SHADER> FragmentShader;
