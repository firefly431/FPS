#include "OpenGLRenderer.h"

#include <ifstream>
#include <SFML/OpenGL.hpp>

// used in initializer list
static sf::ContextSettings opengl_settings() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 0;
    settings.majorVerson = 3;
    settings.minorVersion = 0;
}

OpenGLRenderer::OpenGLRenderer(int width, int height) :
        window(
            sf::VideoMode(width, height), "Spearthrowers",
            sf::Style::Default, opengl_settings()
        ), players(),
        scene_mesh(ShaderProgram(
            std::ifstream("static.v"),
            std::ifstream("basic.f")
        )),
        player_mesh(ShaderProgram(
            std::ifstream("player.v"),
            std::ifstream("basic.f")
        )) {
    window.setVerticalSyncEnabled(true);
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
	scene_mesh.use();
	scene_mesh.draw();
	player_mesh.use();
	auto it = players.cbegin(), end = players.cend();
	for (; it != end; ++it) {
		const Player &p = *it;
		player_mesh.draw(p);
	}
	Mesh::unbind();
    window.display();
}

void OpenGLRenderer::resize(unsigned int width, unsigned int height) {
    // adjust the view uniforms
    glViewport(0, 0, width, height);
}
