#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <iostream>
#include "Config.h"
#include "util/EventManager.h"

class GameOverScreen {
    sf::RenderTexture texture;
    sf::Sprite shape;

    sf::Vector2i size;

    std::wstring text;

    bool victory{};

    sf::Color VICTORY_COLOR = sf::Color::Green;
    sf::Color LOST_COLOR = sf::Color::Red;

    EventManager& eventManager;
    sf::View camera;

    GameState &gameState;

public:
    GameOverScreen(EventManager& eventManager, sf::Vector2i size_, GameState &gameState)
    : size(size_), eventManager(eventManager), gameState(gameState) {
        camera.setCenter((sf::Vector2f) size / 2.f);
        camera.setSize((sf::Vector2f) size);
        texture.create(size.x, size.y);
        listenScroll();
        listenEvents();
    }

    void setText(const std::wstring& text_) {
        text = text_;
    }

    void isWon(bool ok) {
        victory = ok;
    }

    sf::Sprite& getShape() {
        texture.clear(TERMINAL_BACKGROUND_COLOR);
        sf::Text textShape;
        textShape.setString(text);
        textShape.setFillColor(victory ? VICTORY_COLOR : LOST_COLOR);
        textShape.setFont(Config::font);
        textShape.setCharacterSize(20);
        textShape.setPosition(0, 0);
        texture.draw(textShape);
        texture.setView(camera);
        texture.display();
        shape.setTexture(texture.getTexture());
        return shape;
    }

    void listenScroll() {
        eventManager.AddEventListener(sf::Event::MouseWheelScrolled, [this] (sf::Event event) {
            camera.setCenter(camera.getCenter() - sf::Vector2f{0, event.mouseWheelScroll.delta * 15});

            if (camera.getCenter().y < camera.getSize().y / 2) {
                camera.setCenter(camera.getCenter().x, camera.getSize().y / 2);
            }
        });
    }

    void listenEvents() {
        eventManager.AddEventListener(sf::Event::KeyPressed, [this] (sf::Event event) {
            switch (event.key.scancode) {
                case sf::Keyboard::Scan::Space:
                    goToMenu();
                    break;
                default:
                    break;
            }
        });
        eventManager.AddEventListener(sf::Event::MouseButtonPressed, [this] (sf::Event event) {
            goToMenu();
        });
    }

    void goToMenu() {
        gameState = GameState::MENU;
        Config::sounds.play(Config::sounds.TAKE);
    }
};