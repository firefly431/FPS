#pragma once

#include "Scene.h"
#include <SFML/Graphics.hpp>

class TopDownRenderer
{
    Scene scene;
    sf::RenderWindow window;
    struct {
        sf::Vertex wall[2];
        sf::CircleShape player;
        sf::CircleShape point;
    } shapes;
    void drawLine(const Line &line, const sf::Color &color = sf::Color::Green);
    void drawPlayer(const Player &player);
    void drawPoint(const vector &v);
public:
    TopDownRenderer(int width, int height);
    void mainloop();
};

