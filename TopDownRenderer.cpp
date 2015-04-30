#include "TopDownRenderer.h"

#include <iostream>

#include <SFML/Graphics.hpp>

#include "Circle.h"
#include "Line.h"

void TopDownRenderer::drawPlayer(const Player &p) {
	shapes.player.setPosition(p.position.x, p.position.y);
	window.draw(shapes.player);

	// todo: draw heading
}

TopDownRenderer::TopDownRenderer(int width, int height)
	: scene(), window(sf::VideoMode(width, height), "Top Down View"), shapes()
{
	shapes.player = sf::CircleShape(Player::COLLISION_RADIUS);
	shapes.player.setFillColor(sf::Color::Yellow);
	shapes.player.setOrigin(50, 50);
	shapes.wall[0] = sf::Vertex(sf::Vector2f(), sf::Color::Green);
	shapes.wall[1] = sf::Vertex(sf::Vector2f(), sf::Color::Green);
	// test players
#if _MSC_VER < 1800
	scene.players.push_back(Player(vector(100, 100), 0));
	scene.players.push_back(Player(vector(200, 100), 0));
#else
	scene.players.emplace_back(vector(100, 100), 0);
	scene.players.emplace_back(vector(200, 100), 0);
#endif
	while (window.isOpen()) {
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed)
				window.close();
			else if (ev.type == sf::Event::KeyPressed) {
				scene.players[0].input.up = true;
			}
			else if (ev.type == sf::Event::KeyReleased) {
				scene.players[0].input.up = false;
			}
		}
		window.clear();
#if _MSC_VER < 1800
		auto e_it = scene.players.end();
		for (auto it = scene.players.begin(); it != e_it; it++) {
			auto &p = *it;
#else
		for (Player &p : scene.players) {
#endif
			drawPlayer(p);
			p.move(scene.walls);
		}
		window.display();
	}
}
