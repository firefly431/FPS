#include "OpenGLRenderer.h"
#include "OpenGL.h"

#include <fstream>
#include <iostream>

#include "ShaderProgram.h"

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
        ), players() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    window.setVerticalSyncEnabled(true);
    // print settings
    sf::ContextSettings settings = window.getSettings();
    std::cout << "OpenGL version: ";
    std::cout << settings.majorVersion << "." << settings.minorVersion;
    std::cout << std::endl;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::Resized) {
                resize(event.size.width, event.size.height);
            } // others such as keyboard input
        }
        draw();
    }
}

void OpenGLRenderer::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// draw the meshes
    window.display();
}

void OpenGLRenderer::resize(unsigned int width, unsigned int height) {
    // adjust the view uniforms
    glViewport(0, 0, width, height);
}
