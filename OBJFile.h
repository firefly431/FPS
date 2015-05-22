#pragma once

#include <tuple>
#include <vector>
#include <string>
#include <map>
#include <istream>
#include <stdexcept>
#include "OpenGL.h"

#include "VertexArray.h"

class FileFormatException : public std::runtime_error {
public:
    FileFormatException(const std::string &what) : runtime_error(what) {}
};

// OBJ file (wavefront)
class OBJFile {
    typedef std::tuple<GLuint, GLuint, GLuint> uint3;
    // buffers
    std::vector<GLfloat> vertices, texcoords, normals;
    // below are the things in the file
    std::vector<GLfloat> filev, filet, filen;
    std::vector<GLuint> faces;
    // vertex-triple to object index
    std::map<uint3, GLuint> vtof;
    // next index in vtof
    unsigned int idx;
    // read a vertex from the file (when reading faces)
    GLuint read_vertex(std::istream &in);
    // load from a file
    void load(std::istream &in);
public:
    OBJFile(std::istream &in);
    OBJFile(const char *fname);
    VertexArray result();
};

