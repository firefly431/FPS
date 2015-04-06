#include "OpenGLRenderer.h"
#include "OpenGL.h"

#include <fstream>
#include <iostream>

#include "ShaderProgram.h"
#include "VertexArray.h"
#include "OBJFile.h"

// negative because right = clockwise
static const double RADS_PER_PX = -0.01;

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
        p_mesh(
            ShaderProgram(
                VertexShader("player.vert", 0),
                FragmentShader("basic.frag", 0)
            ),
            OBJFile("teapotSmooth.obj").result(),
            Texture("default.png")
        ), players(), current_player(0) {
    glEnable(GL_DEPTH_TEST);
    window.setVerticalSyncEnabled(true);
    camera.updateView(0, 0, 1, 1, 0, 0, 0, 0, 1);
    camera.updateProj(45, (double)width / height, 0.5, 100);
    // print settings
    sf::ContextSettings settings = window.getSettings();
    std::cout << "OpenGL version: ";
    std::cout << settings.majorVersion << "." << settings.minorVersion;
    std::cout << std::endl;
    // add some players
    players.push_back(Player(vector(), 0));
    players.push_back(Player(vector(8, 0), M_PI / 2));
    players.push_back(Player(vector(-8, 0), -M_PI / 2));
    players.push_back(Player(vector(0, 8), M_PI));
    players.push_back(Player(vector(0, -8), 0));
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
            }
        }
        if (!window.isOpen()) break;
        auto ws = window.getSize();
        int hw = ws.x / 2;
        sf::Vector2i h(hw, ws.y / 2);
        double prot = (sf::Mouse::getPosition(window).x - hw) * RADS_PER_PX;
        sf::Mouse::setPosition(h, window);
        auto &player = players[current_player];
        player.rotate(prot);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            player.moveForward();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            player.moveLeft();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            player.moveRight();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            player.moveBack();
        draw();
    }
}

void OpenGLRenderer::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// draw the meshes
    camera.updateView(players[current_player], 1.0);
    p_mesh.activate();
    p_mesh.updateVP(camera);
    auto end = players.cend();
    auto me = players.cbegin() + current_player;
    for (auto it = players.cbegin(); it != end; ++it) {
        if (me == it) continue;
        auto &p = *it;
        p_mesh.update(p);
        p_mesh.draw();
    }
    Mesh::deactivate();
    window.display();
}

void OpenGLRenderer::resize(unsigned int width, unsigned int height) {
    camera.updateProj(45, (double)width / height, 0.5, 100);
    glViewport(0, 0, width, height);
}
