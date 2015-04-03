#include "OBJFile.h"
#include <fstream>
#include <iostream>

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

void OBJFile::write(std::ostream &out) {
    // write back, simple operation
    // write vertices
    for (auto it = vertices.begin(); it != vertices.end(); it += 3)
        out << "v " << *it << " " << *(it + 1) << " " << *(it + 2) << std::endl;
    for (auto it = texcoords.begin(); it != texcoords.end(); it += 2)
        out << "vt " << *it << " " << *(it + 1) << std::endl;
    for (auto it = normals.begin(); it != normals.end(); it += 3)
        out << "vn " << *it << " " << *(it + 1) << " " << *(it + 2) << std::endl;
    for (auto it = faces.begin(); it != faces.end(); it += 3) {
        auto a = *it, &b = *(it + 1), &c = *(it + 2);
        ++a;++b;++c;
        out << "f " << a << "/" << a << "/" << a << " " << b << "/" << b << "/" << b << " " << c << "/" << c << "/" << c << std::endl;
    }
}

int main() {
    std::ifstream in("test.obj");
    std::ofstream out("out.obj");
    try {
        OBJFile file(in);
        file.write(out);
    } catch (const FileFormatException &ffe) {
        std::cerr << "Error: " << ffe.what() << std::endl;
    }
}
