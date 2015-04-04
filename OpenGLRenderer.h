#pragma once

#include <vector>
#include <SFML/Window.hpp>

#include "PlayerMesh.h"
#include "Camera.h"

class OpenGLRenderer {
public:
	OpenGLRenderer(int width, int height);

	void resize(unsigned int width, unsigned int height);
	void draw();
protected:
    sf::Window window;
    Camera camera;
    PlayerMesh teapot;
};

