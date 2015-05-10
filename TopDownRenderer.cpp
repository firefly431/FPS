#include "TopDownRenderer.h"

#include <iostream>

#include <SFML/Graphics.hpp>

#include "Circle.h"
#include "Line.h"
#include "Spear.h"
#include "AIController.h"
#include "Quad.h"

void TopDownRenderer::drawLine(const Line &l, const sf::Color &c) {
    shapes.wall[0].position.x = l.p1.x;
    shapes.wall[0].position.y = l.p1.y;
    shapes.wall[1].position.x = l.p2.x;
    shapes.wall[1].position.y = l.p2.y;
    shapes.wall[0].color = shapes.wall[1].color = c;
    window.draw(shapes.wall, 2, sf::Lines);
}

void TopDownRenderer::drawPlayer(const Player &p) {
	shapes.player.setPosition(p.position.x, p.position.y);
	window.draw(shapes.player);
    drawLine(Line(p.position, p.position + vector(p.heading) * Player::COLLISION_RADIUS), sf::Color::Red);
}

void TopDownRenderer::drawPoint(const vector &v) {
    shapes.point.setPosition(v.x, v.y);
    window.draw(shapes.point);
}

void TopDownRenderer::drawTri(const Triangle &t, const sf::Color &color) {
    drawLine(Line(t.a, t.b), color);
    drawLine(Line(t.b, t.c), color);
    drawLine(Line(t.a, t.c), color);
}

TopDownRenderer::TopDownRenderer(int width, int height)
	: scene(), window(sf::VideoMode(width, height), "Top Down View"), shapes()
{
    window.setVerticalSyncEnabled(true);
	shapes.player = sf::CircleShape(Player::COLLISION_RADIUS);
	shapes.player.setFillColor(sf::Color::Yellow);
    shapes.player.setOrigin(Player::COLLISION_RADIUS, Player::COLLISION_RADIUS);
    shapes.point = sf::CircleShape(0.1);
    shapes.point.setFillColor(sf::Color::White);
    shapes.point.setOrigin(0.1, 0.1);
	shapes.wall[0] = sf::Vertex(sf::Vector2f(), sf::Color::Green);
	shapes.wall[1] = sf::Vertex(sf::Vector2f(), sf::Color::Green);
    scene.loadGraph("faces.obj");
    scene.addPlayer();
    scene.addPlayer();
    scene.players[1].movement_speed = 0.9;
    scene.players[1].setController(new AIController(scene.players[0], scene.graph, scene.walls));
    sf::View view;
    view.setSize(sf::Vector2f(64 * width/height, 64));
    view.setCenter(sf::Vector2f(0, 0));
    window.setView(view);
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
        auto n_it = scene.graph.nodes.end();
        for (auto it = scene.graph.nodes.begin(); it != n_it; it++) {
            const auto &nn = *it;
#if 0
        }
#endif
#else
        for (const auto &nn : scene.graph.nodes) {
#endif
            //drawPoint(nn.position);
            Triangle *tri = dynamic_cast<Triangle *>(nn.shape.get());
            if (tri == nullptr) {
                Quad *quad = (Quad *)nn.shape.get();
                drawTri(quad->a, sf::Color::Red);
                drawTri(quad->b, sf::Color::Red);
            } else {
                drawTri(*tri, sf::Color::Blue);
            }
            for (const auto &ne : nn.edges) {
                drawLine(Line(nn.position,
                              ne.to->position), sf::Color::White);
            }
        }
#if _MSC_VER < 1800
		auto w_it = scene.walls.end();
		for (auto it = scene.walls.begin(); it != w_it; it++) {
			auto &w = *it;
#if 0
        }
#endif
#else
		for (Line &w : scene.walls) {
#endif
            drawLine(w);
		}
#if _MSC_VER < 1800
		auto e_it = scene.players.end();
		for (auto it = scene.players.begin(); it != e_it; it++) {
			auto &p = *it;
#if 0
        }
#endif
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
        const auto &aic = *((AIController *)scene.players[1].controller.get());
        if (!aic.path.empty())
            drawPoint(aic.path.back()->position);
		window.display();
	}
}
