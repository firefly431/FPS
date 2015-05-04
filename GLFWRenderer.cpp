#include "GLFWRenderer.h"

#include <iostream>
#include <stdexcept>
#include <cmath>

#include "OBJFile.h"

static const double RADS_PER_PX = -0.01;

static void error_callback(int error, const char *description) {
    std::cerr << "GLFW error!" << std::endl;
    std::cerr << description << std::endl;
}

void resize_callback(GLFWwindow *window, int width, int height) {
    GLFWRenderer *renderer = (GLFWRenderer *)glfwGetWindowUserPointer(window);
    glfwGetFramebufferSize(window, &width, &height);
    renderer->resize(width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    GLFWRenderer *renderer = (GLFWRenderer *)glfwGetWindowUserPointer(window);
    switch (key) {
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GL_TRUE);
        break;
    case GLFW_KEY_W:
        renderer->players[renderer->current_player].input.up = action != GLFW_RELEASE;
        break;
    case GLFW_KEY_S:
        renderer->players[renderer->current_player].input.down = action != GLFW_RELEASE;
        break;
    case GLFW_KEY_A:
        renderer->players[renderer->current_player].input.left = action != GLFW_RELEASE;
        break;
    case GLFW_KEY_D:
        renderer->players[renderer->current_player].input.right = action != GLFW_RELEASE;
        break;
    }
}

void cursor_callback(GLFWwindow *window, double xpos, double ypos) {
    // doubles can hold up to around 9e15 exactly
    // but just to be safe let's clamp it at 1e5
    if (xpos > 1e5 || xpos < -1e5) {
        xpos = fmod(xpos, 1e10);
        glfwSetCursorPos(window, xpos, ypos);
    }
    GLFWRenderer *renderer = (GLFWRenderer *)glfwGetWindowUserPointer(window);
    renderer->players[renderer->current_player].heading = fmod(xpos * RADS_PER_PX, 2 * M_PI);
}

GLFWRenderer::GLFWRenderer(unsigned int width, unsigned int height) : players(), current_player(0) {
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        throw std::runtime_error("Failed to init GLFW");
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#ifndef NDEBUG
    //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
    window = glfwCreateWindow(width, height, "Spearthrowers", NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create window");
    }
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // let callbacks access us
    glfwSetWindowUserPointer(window, (void *)this);
    glfwMakeContextCurrent(window);
    camera = std::unique_ptr<Camera>(new Camera());
    p_mesh = std::unique_ptr<PlayerMesh>(new PlayerMesh(
        ShaderProgram(
            VertexShader("player.vert", 0),
            FragmentShader("basic.frag", 0)
        ),
        OBJFile("teapotSmooth.obj").result(),
        Texture("default.png")
    ));
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glfwSwapInterval(1);
    glfwGetFramebufferSize(window, (int *)&width, (int *)&height);
    camera->updateView(0, 0, 0, 1, 0, 0, 0, 0, 1);
    resize(width, height);
    glfwSetWindowSizeCallback(window, resize_callback);
    // print context version
    std::cout << "OpenGL version: ";
    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLFW version: " << glfwGetVersionString() << std::endl;
    // add some players
    players.push_back(Player(vector(), 0));
    players.push_back(Player(vector(8, 0), M_PI / 2));
    players.push_back(Player(vector(-8, 0), -M_PI / 2));
    players.push_back(Player(vector(0, 8), M_PI));
    players.push_back(Player(vector(0, -8), 0));
}

void GLFWRenderer::mainloop() {
    std::vector<Line> walls;
    while (!glfwWindowShouldClose(window)) {
        players[current_player].move(walls);
        draw();
        glfwPollEvents();
    }
}

void GLFWRenderer::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// draw the meshes
    camera->updateView(players[current_player], 1.0);
    p_mesh->activate();
    p_mesh->updateVP(*camera);
    auto end = players.cend();
    auto me = players.cbegin() + current_player;
    for (auto it = players.cbegin(); it != end; ++it) {
        if (me == it) continue;
        auto &p = *it;
        p_mesh->update(p);
        p_mesh->draw();
    }
    Mesh::deactivate();
    glfwSwapBuffers(window);
}

void GLFWRenderer::resize(unsigned int width, unsigned int height) {
    camera->updateProj(45, (double)width / height, 0.5, 100);
    glViewport(0, 0, width, height);
}

GLFWRenderer::~GLFWRenderer() {
    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}

