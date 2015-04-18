#include "GLFWRenderer.h"

#include <iostream>
#include <stdexcept>

#include "OBJFile.h"

void error_callback(int error, const char *description) {
    std::cerr << "GLFW error!" << std::endl;
    std::cerr << description << std::endl;
}

void resize_callback(GLFWwindow *window, int width, int height) {
    GLFWRenderer *renderer = (GLFWRenderer *)window;
    glfwGetFramebufferSize(window, &width, &height);
    renderer->resize(width, height);
}

GLFWRenderer::GLFWRenderer(unsigned int width, unsigned int height) : players(), current_player(0) {
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        throw std::runtime_error("Failed to init GLFW");
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
    window = glfwCreateWindow(width, height, "Spearthrowers", NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create window");
    }
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
    glfwSwapInterval(1);
    glfwGetFramebufferSize(window, (int *)&width, (int *)&height);
    camera->updateView(0, 0, 1, 1, 0, 0, 0, 0, 1);
    camera->updateProj(45, (double)width / height, 0.5, 100);
    glfwSetWindowSizeCallback(window, resize_callback);
    // print context version
    std::cout << "OpenGL version: ";
    std::cout << glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR) << "." << glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR) << std::endl;
    // add some players
    players.push_back(Player(vector(), 0));
    players.push_back(Player(vector(8, 0), M_PI / 2));
    players.push_back(Player(vector(-8, 0), -M_PI / 2));
    players.push_back(Player(vector(0, 8), M_PI));
    players.push_back(Player(vector(0, -8), 0));
}

void GLFWRenderer::mainloop() {
    while (!glfwWindowShouldClose(window)) {
        // do stuff
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

