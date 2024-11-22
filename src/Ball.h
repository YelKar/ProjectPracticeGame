#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "common/Entity.h"

class Ball : public Entity {
private:
    sf::CircleShape shape;
    sf::Vector2f velocity{10.f, 15.f};
    sf::Clock clock;
public:
    Ball() {
        shape.setRadius(10);
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(0, 0);
    }

    void draw(sf::RenderWindow* window) override {
        window->draw(shape);
    }

    void handleEvent(sf::Event& event) override {}

    void update() override {

        std::cout << "awerty\n";
        auto pos = shape.getPosition();
        shape.setPosition(pos + velocity * clock.restart().asSeconds());

//        if (velocity.y > 0) {
//            velocity.y -= std::min(velocity.y, 10.f);
//        }
    }
};
