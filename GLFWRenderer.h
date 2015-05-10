#pragma once

#include <memory>
#include <vector>

#include "OpenGL.h"

#include "PlayerMesh.h"
#include "SpearMesh.h"
#include "Camera.h"
#include "Scene.h"

class GLFWRenderer {
    friend void key_callback(GLFWwindow *, int, int, int, int);
    friend void cursor_callback(GLFWwindow *, double, double);
    friend void mouse_callback(GLFWwindow *, int, int, int);
public:
    static const double WALL_HEIGHT;
    static const double UV_HEIGHT;
    static const double FLOOR_SCALE;

    GLFWRenderer(unsigned int width, unsigned int height);
    ~GLFWRenderer();

    void mainloop();

    void resize(unsigned int width, unsigned int height);
    void draw();
protected:
    GLFWwindow *window;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<PlayerMesh> p_mesh;
    std::unique_ptr<SpearMesh> sp_mesh;
    std::unique_ptr<Mesh> s_mesh;
    std::unique_ptr<Mesh> f_mesh;
    Scene scene;
    Player *player;

    // generate scene and floor meshes
    void generateSceneMesh();
};
