#include <iostream>

#include "Game.h"


void Game::init() {
    window.setSize({800, 600});
    window.setTitle("sdgfhj");

    std::cout << "qwerty\n";
}

void Game::draw() {
    for (auto entity : entities) {
        entity->draw(&window);
    }
}

void Game::loop() {
    while (window.isOpen())
    {
        handleEvents();
        update();

        window.clear();
        draw();
        window.display();
    }
}

void Game::update() {
    for (auto entity : this->entities) {
        entity->update();
    }
}

void Game::handleEvents() {
    sf::Event event{};
    while (window.pollEvent(event))
    {
        for (auto entity : this->entities) {
            entity->handleEvent(event);
        }
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            default:
                break;
        }
    }
}

void Game::addEntity(Entity &entity) {
    entities.push_back(&entity);
}
