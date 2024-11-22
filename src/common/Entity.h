#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

class Entity {
public:
    virtual void draw(sf::RenderWindow* window) = 0;
    virtual void update() = 0;
    virtual void handleEvent(sf::Event& event) = 0;
};