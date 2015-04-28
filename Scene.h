#pragma once

#include <vector>

#include "Player.h"

class Spear;
class Line;

// the model class
class Scene {
	std::vector<Player> players;
	std::vector<Spear> spears;
	std::vector<Line> walls;
public:
	Scene();
	void loadWalls(const std::string &fname);
};
