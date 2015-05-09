#include "Scene.h"

#include <fstream>
#include <sstream>
#include <vector>
#ifdef _MSC_VER
#define strcasecmp _stricmp
#endif

void Scene::loadWalls(const std::string &fname) {
    std::ifstream fin(fname);
    fin >> std::skipws;
    std::string type, line;
    std::vector<vector> vertices;
    float x, y;
    int a, b;
    while (fin.good()) {
        std::getline(fin, line);
        std::istringstream in(line);
        if (line == "") continue;
        in >> type;
        if (strcasecmp(type.c_str(), "v") == 0) {
            in >> x; in >> y; // ignore z
#if _MSC_VER < 1800
            vertices.push_back(vector(x, y));
#else
            vertices.emplace_back(x, y);
#endif
        } else if (strcasecmp(type.c_str(), "l") == 0) {
            in >> a; in >> b;
#if _MSC_VER < 1800
            walls.push_back(Line(vertices[a - 1], vertices[b - 1]));
#else
            walls.emplace_back(vertices[a - 1], vertices[b - 1]);
#endif
        }
    }
}
