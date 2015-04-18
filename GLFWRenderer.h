#pragma once

#include <unique_ptr>

#include "OpenGL.h"

#include "PlayerMesh.h"
#include "Camera.h"

// TODO: refactor scene variables out of renderer

class GLFWRenderer {
public:
    GLFWRenderer(unsigned int width, unsigned int height);
    ~GLFWRenderer();

    void mainloop();

    void resize(unsigned int width, unsigned int height);
    void draw();
protected:
    GLFWwindow *window;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<PlayerMesh> p_mesh;
    std::vector<Player> players;
    int current_player;
};
