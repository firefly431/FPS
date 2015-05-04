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
    } shapes;
    void drawLine(const Line &line);
    void drawPlayer(const Player &player);
public:
    TopDownRenderer(int width, int height);
    void mainloop();
};

