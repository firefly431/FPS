#include "OBJFile.h"

static const std::streamsize cmax = std::numeric_limits<std::streamsize>::max();

GLuint OBJFile::read_vertex() {
    GLuint a, b, c;
    char slash;
    in >> a >> std::ws >> slash;
    if (slash != '/')
        throw FileFormatException("No normal or texcoord");
    if (in.peek() == '/')
        throw new FileFormatException("No texcoord");
    in >> b >> std::ws >> slash;
    if (slash != '/')
        throw FileFormatException("No normal");
    in >> c;
    uint3 trio = std::make_tuple(--a, --b, --c);
    auto it = vtof.find(trio);
    GLuint ret;
    if (it != vtof.end())
        ret = it->second;
    else {
        if (a * 3 >= filev.size())
            throw FileFormatException("Out of bounds");
        if (b * 2 >= filet.size())
            throw FileFormatException("Out of bounds");
        if (c * 3 >= filen.size())
            throw FileFormatException("Out of bounds");
        auto itv = filev.begin() + a * 3;
        std::copy(itv, itv + 3, std::back_inserter(vertices));
        auto itt = filet.begin() + b * 2;
        std::copy(itt, itt + 2, std::back_inserter(texcoords));
        auto itn = filen.begin() + c * 3;
        std::copy(itn, itn + 3, std::back_inserter(normals));
        vtof.insert(std::make_pair(trio, ret = idx++));
    }
    return ret;
}

// note that triangulation is super crappy and kind of works
// for only simple files
OBJFile::OBJFile(std::istream &in) : in(in) {
    // will modify in state
    in >> std::skipws;
    std::string type;
    GLfloat f;
    while (in.good()) {
        in >> type;
        if (strcasecmp(type.c_str(), "f") == 0) {
            // face = triangle
            GLuint a, b, c;
            faces.push_back(a = read_vertex());
            faces.push_back(b = read_vertex());
            faces.push_back(c = read_vertex());
            in >> std::ws; // skip ws
            if (isdigit(in.peek())) {
                GLuint d = read_vertex();
                // triangulate
                faces.push_back(c);
                faces.push_back(d);
                faces.push_back(a);
            }
        } else if (strcasecmp(type.c_str(), "v") == 0) {
            in >> f; filev.push_back(f);
            in >> f; filev.push_back(f);
            in >> f; filev.push_back(f);
        } else if (strcasecmp(type.c_str(), "vt") == 0) {
            in >> f; filet.push_back(f);
            in >> f; filet.push_back(f);
        } else if (strcasecmp(type.c_str(), "vn") == 0) {
            in >> f; filen.push_back(f);
            in >> f; filen.push_back(f);
            in >> f; filen.push_back(f);
        }
        // if none, treat as comment
        // ignore rest of line
        in.ignore(cmax, '\n');
    }
}

VertexArray OBJFile::result() {
    VertexArray ret(faces.size(), GL_TRIANGLES);
    ret.activate();
    ret.bindBuffer(VertexBuffer(sizeof(GLfloat) * vertices.size(), GL_STATIC_DRAW, 0, &vertices[0]), ATTRIBUTE_POSITION, 3, GL_FLOAT, 0);
    ret.bindBuffer(VertexBuffer(sizeof(GLfloat) * texcoords.size(), GL_STATIC_DRAW, 0, &texcoords[0]), ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, 0);
    ret.bindBuffer(VertexBuffer(sizeof(GLfloat) * normals.size(), GL_STATIC_DRAW, 0, &normals[0]), ATTRIBUTE_NORMAL, 3, GL_FLOAT, 0);
    ret.bindBuffer(IndexBuffer(sizeof(GLuint) * faces.size(), GL_STATIC_DRAW, 0, &faces[0]), GL_UNSIGNED_INT);
    ret.deactivate();
    return ret;
}
