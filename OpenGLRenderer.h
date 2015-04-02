#pragma once

#include <vector>
#include <SFML/Window.hpp>

#include "Player.h"

class OpenGLRenderer {
public:
	OpenGLRenderer(int width, int height);

	void resize(unsigned int width, unsigned int height);
	void draw();
protected:
    sf::Window window;
	std::vector<Player> players;
	StaticMesh scene_mesh;
	PlayerMesh player_mesh;
};

