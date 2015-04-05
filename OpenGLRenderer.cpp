#include "OpenGLRenderer.h"
#include "OpenGL.h"

#include <fstream>
#include <iostream>

#include "ShaderProgram.h"
#include "VertexArray.h"
#include "OBJFile.h"

// used in initializer list
static sf::ContextSettings opengl_settings() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 0;
    settings.majorVersion = 3;
    settings.minorVersion = 2;
    return settings;
}

OpenGLRenderer::OpenGLRenderer(int width, int height) :
        window(
            sf::VideoMode(width, height), "Spearthrowers",
            sf::Style::Default, opengl_settings()
        ), camera(),
        teapot(
            ShaderProgram(
                VertexShader("player.vert", 0),
                FragmentShader("basic.frag", 0)
            ),
            OBJFile("teapotSmooth.obj").result(),
            Texture("default.png")
        ), controller() {
    glEnable(GL_DEPTH_TEST);
    window.setVerticalSyncEnabled(true);
    camera.updateView(0, -8, 1, 0, 1, 0, 0, 0, 1);
    camera.updateProj(45, (double)width / height, 0.5, 100);
    // print settings
    sf::ContextSettings settings = window.getSettings();
    std::cout << "OpenGL version: ";
    std::cout << settings.majorVersion << "." << settings.minorVersion;
    std::cout << std::endl;
}

void OpenGLRenderer::mainloop() {
    while (true) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                break;
            } else if (event.type == sf::Event::Resized) {
                resize(event.size.width, event.size.height);
            } // others such as keyboard input
        }
        if (!window.isOpen()) break;
        draw();
        controller.heading += 0.01;
    }
}

void OpenGLRenderer::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// draw the meshes
    teapot.activate();
    teapot.update(controller);
    teapot.updateVP(camera);
    teapot.draw();
    Mesh::deactivate();
    window.display();
}

void OpenGLRenderer::resize(unsigned int width, unsigned int height) {
    camera.updateProj(45, (double)width / height, 0.5, 100);
    glViewport(0, 0, width, height);
}
