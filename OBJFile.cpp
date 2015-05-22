#include "OBJFile.h"
#include <fstream>
#include <sstream>
#include <iostream>

// TBH, this is pretty hacky. it's a single-use class.

static const std::streamsize cmax = std::numeric_limits<std::streamsize>::max();

GLuint OBJFile::read_vertex(std::istream &in) {
    GLuint a, b, c;
    char slash;
    // read the index of the position
    in >> a >> std::ws >> slash;
    if (slash != '/') // verify next character is a slash
        throw FileFormatException("No normal or texcoord");
    if (in.peek() == '/') // if the next character is a slash, we're missing a texcoord
        throw new FileFormatException("No texcoord");
    in >> b >> std::ws >> slash; // read in a texcoord
    if (slash != '/') // verify slash
        throw FileFormatException("No normal");
    in >> c; // read in the normal
    uint3 trio = std::make_tuple(--a, --b, --c); // remember OBJ is 1-based
    auto it = vtof.find(trio); // use an already-existing vertex if possible
    GLuint ret;
    if (it != vtof.end()) // already exists! yay
        ret = it->second;
    else {
        if (a * 3 >= filev.size())
            throw FileFormatException("Out of bounds");
        if (b * 2 >= filet.size())
            throw FileFormatException("Out of bounds");
        if (c * 3 >= filen.size())
            throw FileFormatException("Out of bounds");
        // copy the indices to a new position
        auto itv = filev.begin() + a * 3;
        std::copy(itv, itv + 3, std::back_inserter(vertices));
        auto itt = filet.begin() + b * 2;
        std::copy(itt, itt + 2, std::back_inserter(texcoords));
        auto itn = filen.begin() + c * 3;
        std::copy(itn, itn + 3, std::back_inserter(normals));
        // insert the new position
        vtof.insert(std::make_pair(trio, ret = idx++));
    }
    return ret;
}

// figured out the problem with triangulation
OBJFile::OBJFile(std::istream &in) : idx(0) {load(in);}
OBJFile::OBJFile(const char *fname) : idx(0) { // super hack
    std::ifstream stream(fname);
    load(stream);
}
void OBJFile::load(std::istream &in1) {
    // will modify in1 state
    in1 >> std::skipws;
    std::string type;
    std::string line;
    GLfloat f;
    // keep reading lines
    while (in1.good()) {
        std::getline(in1, line);
        std::istringstream in(line);
        if (line == "") continue;
        in >> type;
        // face
        if (strcasecmp(type.c_str(), "f") == 0) {
            try {
                // face = triangle
                GLuint a, b, c;
                faces.push_back(a = read_vertex(in));
                faces.push_back(b = read_vertex(in));
                faces.push_back(c = read_vertex(in));
                in >> std::ws; // skip ws
                if (isdigit(in.peek())) {
                    // quad
                    GLuint d = read_vertex(in);
                    // triangulate
                    faces.push_back(c);
                    faces.push_back(d);
                    faces.push_back(a);
                }
            } catch (const FileFormatException &f) {
                std::cout << line << std::endl;
            }
        } else if (strcasecmp(type.c_str(), "v") == 0) {
            // vertex
            in >> f; filev.push_back(f);
            in >> f; filev.push_back(f);
            in >> f; filev.push_back(f);
        } else if (strcasecmp(type.c_str(), "vt") == 0) {
            // texcoord
            in >> f; filet.push_back(f);
            in >> f; filet.push_back(f);
        } else if (strcasecmp(type.c_str(), "vn") == 0) {
            // normal
            in >> f; filen.push_back(f);
            in >> f; filen.push_back(f);
            in >> f; filen.push_back(f);
        }
    }
}

VertexArray OBJFile::result() {
    // make a VA from the results
    VertexArray ret(faces.size(), GL_TRIANGLES);
    ret.activate();
    ret.bindBuffer(VertexBuffer(sizeof(GLfloat) * vertices.size(), GL_STATIC_DRAW, &vertices[0]), ATTRIBUTE_POSITION, 3, GL_FLOAT, 0);
    ret.bindBuffer(VertexBuffer(sizeof(GLfloat) * texcoords.size(), GL_STATIC_DRAW, &texcoords[0]), ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, 0);
    ret.bindBuffer(VertexBuffer(sizeof(GLfloat) * normals.size(), GL_STATIC_DRAW, &normals[0]), ATTRIBUTE_NORMAL, 3, GL_FLOAT, 0);
    ret.bindBuffer(IndexBuffer(sizeof(GLuint) * faces.size(), GL_STATIC_DRAW, &faces[0]), GL_UNSIGNED_INT);
    ret.deactivate();
    return ret;
}
