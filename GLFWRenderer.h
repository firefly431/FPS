#pragma once

#include <memory>
#include <vector>

#include "OpenGL.h"

#include "PlayerMesh.h"
#include "SpearMesh.h"
#include "Camera.h"
#include "Scene.h"

// GLFW renderer
class GLFWRenderer {
    // let the callbacks access our private info
    friend void key_callback(GLFWwindow *, int, int, int, int);
    friend void cursor_callback(GLFWwindow *, double, double);
    friend void mouse_callback(GLFWwindow *, int, int, int);
public:
    // height of the wall (game coordinates)
    static const double WALL_HEIGHT;
    // height of the wall (UV coordinates)
    static const double UV_HEIGHT;
    // UV coordinates of floor per game unit
    static const double FLOOR_SCALE;

    GLFWRenderer(unsigned int width, unsigned int height);
    ~GLFWRenderer();

    void mainloop();

    void resize(unsigned int width, unsigned int height);
    void draw();
protected:
    // GLFW window object
    GLFWwindow *window;
    // camera object
    std::unique_ptr<Camera> camera;
    // meshes
    std::unique_ptr<PlayerMesh> p_mesh;
    std::unique_ptr<SpearMesh> sp_mesh;
    std::unique_ptr<Mesh> s_mesh;
    std::unique_ptr<Mesh> f_mesh;
    // scene
    Scene scene;
    // current player
    Player *player;

    // generate scene and floor meshes
    void generateSceneMesh();
};
