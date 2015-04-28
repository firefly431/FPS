#include "TopDownRenderer.h"

#include <SFML/Graphics.hpp>

#include "Circle.h"
#include "Line.h"

TopDownRenderer::TopDownRenderer(int width, int height)
{
	// test
	sf::RenderWindow window(sf::VideoMode(width, height), "Top Down View");
	sf::CircleShape player_shape(50), normed(50);
	Circle player(vector(0, 0), 50);
	player_shape.setFillColor(sf::Color::Yellow);
	player_shape.setPosition(50, 50);
	normed.setFillColor(sf::Color::Transparent);
	normed.setOutlineColor(sf::Color::Green);
	normed.setOutlineThickness(2);
	sf::CircleShape selcursor(10);
	selcursor.setFillColor(sf::Color::Green);
	selcursor.setPosition(50, 50);
	Line wall(vector(100, 200), vector(200, 100));
	sf::Vertex line_shape[] = {sf::Vertex(sf::Vector2f(100, 200)), sf::Vertex(sf::Vector2f(200, 100))};
	sf::Vertex line_shape2[] = {sf::Vertex(sf::Vector2f(400, 500)), sf::Vertex(sf::Vector2f(500, 400))};
	Line arrow(vector(400, 500), vector(500, 400));
	enum {
		PLAYER = 0, WALL1, WALL2, ARROW1, ARROW2, MAX
	} selected = MAX;
	vector *selections[] = {&player.position, &wall.p1, &wall.p2, &arrow.p1, &arrow.p2};
	while (window.isOpen()) {
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed)
				window.close();
			if (ev.type == sf::Event::MouseButtonPressed) {
				vector mp(ev.mouseButton.x, ev.mouseButton.y);
				selected = MAX;
				double mindist = std::numeric_limits<double>::infinity();
				for (auto i = 0; i < MAX; i++) {
					double dist = *selections[i] - mp;
					if (dist < mindist) {
						mindist = dist;
						selected = static_cast<decltype(selected)>(i);
					}
				}
			}
			if (ev.type == sf::Event::MouseButtonReleased) {
				selected = MAX;
			}
		}
		window.clear();
		player_shape.setPosition((float)player.position.x - player.radius, (float)player.position.y - player.radius);
		vector off;
		bool intersect;
		player_shape.setFillColor((intersect = player.intersects(wall, &off)) ? sf::Color::Red : sf::Color::Yellow);
		player_shape.setRadius(player.radius);
		window.draw(player_shape);
		line_shape[0].position.x = (float)wall.p1.x;
		line_shape[0].position.y = (float)wall.p1.y;
		line_shape[1].position.x = (float)wall.p2.x;
		line_shape[1].position.y = (float)wall.p2.y;
		window.draw(line_shape, 2, sf::Lines);
		line_shape2[0].position.x = (float)arrow.p1.x;
		line_shape2[0].position.y = (float)arrow.p1.y;
		line_shape2[1].position.x = (float)arrow.p2.x;
		line_shape2[1].position.y = (float)arrow.p2.y;
		bool arrint;
		vector lwint;
		line_shape2[0].color = line_shape2[1].color = (arrint = arrow.intersects(wall, &lwint)) ? sf::Color::Red : sf::Color::Blue;
		window.draw(line_shape2, 2, sf::Lines);
		if (intersect) {
			normed.setRadius(player.radius);
			vector np = player.position + off;
			normed.setPosition(np.x - player.radius, np.y - player.radius);
			window.draw(normed);
		}
		if (arrint) {
			double radd = selcursor.getRadius();
			selcursor.setPosition(lwint.x - radd, lwint.y - radd);
			window.draw(selcursor);
		}
		if (selected != MAX) {
			sf::Vector2i mp = sf::Mouse::getPosition(window);
			selections[selected]->x = mp.x;
			selections[selected]->y = mp.y;
			double radd = selcursor.getRadius();
			selcursor.setPosition(mp.x - radd, mp.y - radd);
			window.draw(selcursor);
		}
		window.display();
	}
}
