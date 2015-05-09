#include "TopDownRenderer.h"

#include <iostream>

#include <SFML/Graphics.hpp>

#include "Circle.h"
#include "Line.h"
#include "Spear.h"

void TopDownRenderer::drawLine(const Line &l) {
    shapes.wall[0].position.x = l.p1.x;
    shapes.wall[0].position.y = l.p1.y;
    shapes.wall[1].position.x = l.p2.x;
    shapes.wall[1].position.y = l.p2.y;
    window.draw(shapes.wall, 2, sf::Lines);
}

void TopDownRenderer::drawPlayer(const Player &p) {
	shapes.player.setPosition(p.position.x, p.position.y);
	window.draw(shapes.player);
}

TopDownRenderer::TopDownRenderer(int width, int height)
	: scene(), window(sf::VideoMode(width, height), "Top Down View"), shapes()
{
    window.setVerticalSyncEnabled(true);
	shapes.player = sf::CircleShape(Player::COLLISION_RADIUS);
	shapes.player.setFillColor(sf::Color::Yellow);
    shapes.player.setOrigin(Player::COLLISION_RADIUS, Player::COLLISION_RADIUS);
	shapes.wall[0] = sf::Vertex(sf::Vector2f(), sf::Color::Green);
	shapes.wall[1] = sf::Vertex(sf::Vector2f(), sf::Color::Green);
	// test players
#if _MSC_VER < 1800
	scene.players.push_back(Player(vector(0, 0), 0));
#else
	scene.players.emplace_back(vector(0, 0), 0);
#endif
    sf::View view;
    view.setSize(sf::Vector2f(28.8 * width/height, 28.8));
    view.setCenter(sf::Vector2f(0, 0));
    window.setView(view);
    scene.loadWalls("walls.obj");
}

void TopDownRenderer::mainloop() {
	while (window.isOpen()) {
		sf::Event ev;
        auto mpos_ = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        auto mpos = vector(mpos_.x, mpos_.y);
        scene.players[0].heading = (mpos - scene.players[0].position).angle();
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed)
				window.close();
			else if (ev.type == sf::Event::KeyPressed) {
				scene.players[0].input.up = true;
			}
			else if (ev.type == sf::Event::KeyReleased) {
				scene.players[0].input.up = false;
			}
            else if (ev.type == sf::Event::MouseButtonPressed) {
                scene.players[0].input.fire = true;
            }
            else if (ev.type == sf::Event::MouseButtonReleased) {
                scene.players[0].input.fire = false;
            }
		}
		window.clear();
#if _MSC_VER < 1800
		auto w_it = scene.walls.end();
		for (auto it = scene.walls.begin(); it != w_it; it++) {
			auto &w = *it;
#else
		for (Line &w : scene.walls) {
#endif
            drawLine(w);
		}
#if _MSC_VER < 1800
		auto e_it = scene.players.end();
		for (auto it = scene.players.begin(); it != e_it; it++) {
			auto &p = *it;
#else
		for (Player &p : scene.players) {
#endif
			drawPlayer(p);
			p.move(scene.walls, scene.spears);
		}
		for (auto it = scene.spears.begin(); it != scene.spears.end();) {
			auto &s = *it;
            drawLine(s.getCollisionBounds());
            if (!s.move(scene.players, scene.walls)) {
                auto old_it = it++;
                scene.spears.erase(old_it);
            } else
                it++;
		}
		window.display();
	}
}
