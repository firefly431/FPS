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
	std::vector<vector> spawnPoints;
public:
	// common methods
	// use for each (auto &a in scene.getPlayers()) in VC++
	// and for (auto &a : scene.getPlayers()) in C++11
	const std::vector<Player> &getPlayers() const;
	const std::vector<Spear> &getSpears() const;
	const std::vector<Line> &getWalls() const;
	// server methods
	void loadStage(const std::string &fname); // load walls and spawn points
	void setPlayerInput(int idx, bool up, bool down, bool left, bool right, double rotation);
	void update();
	vector addPlayer(); // returns spawn point
};
