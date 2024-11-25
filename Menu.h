#pragma once


#include <SFML/Graphics/RenderWindow.hpp>
#include "util/EventManager.h"
#include "util/Button.h"
#include "GameState.h"
#include "Globals.h"

class Menu {
    sf::RenderWindow& window;
    EventManager& eventManager;
    GameState &gameState;
    Button newGameBtn;
    Button resumeBtn;

    std::vector<Button> buttons;

    sf::RectangleShape background;
    sf::RectangleShape numBarDivider;
    sf::Text numBarNumber;

    int btnTextSize = 60;
public:

    Menu(sf::RenderWindow &window_, EventManager &eventManager, GameState &gameState)
        : window(window_), eventManager(eventManager), gameState(gameState) {
//        this->gameState = gameState;
    }

    void Init() {
        sf::Vector2f size{
            500,
            80,
        };

        background.setSize((sf::Vector2f) window.getSize());
        background.setFillColor(sf::Color(58, 58, 57));

        buttons.push_back(
            Button{}
                .setBackgroundColor(background.getFillColor())
                .setTextSize(btnTextSize)
                .setTextColor(sf::Color::Green)
                .setFont(font)
                .setText(L"start();")
                .setSize(size)
                .setPosition({200, 200})
                .Render()
                .Connect(eventManager, sf::Event::MouseButtonPressed, [this](auto){
                    gameState = GameState::GAME;
                }
            )
        );

        buttons.push_back(
            Button{}
                .setBackgroundColor(background.getFillColor())
                .setTextSize(btnTextSize)
                .setTextColor(sf::Color(255, 124, 0))
                .setFont(font)
                .setText(L"resume();")
                .setSize(size)
                .setPosition({200, 300})
                .Render()
                .Connect(eventManager, sf::Event::MouseButtonPressed, [this](auto){
                    std::cout << "qwerty\n";
                    gameState = GameState::GAME;
                }
            )
        );

        numBarDivider.setSize({3, 220});
        numBarDivider.setFillColor(sf::Color(124, 124, 124));
        numBarDivider.setPosition({180, 180});

        numBarNumber.setFont(font);
        numBarNumber.setFillColor(numBarDivider.getFillColor());
        numBarNumber.setCharacterSize(btnTextSize);
        numBarNumber.setStyle(sf::Text::Style::Bold);
    }

    void update() {
    }
    void drawMenu() {
        window.draw(background);
        int lineNum = 1;
        for (auto button : buttons) {
            numBarNumber.setPosition(numBarDivider.getPosition().x - 50, button.getPosition().y);
            numBarNumber.setString(std::to_string(lineNum++));
            window.draw([this] () {return numBarNumber;}());
            window.draw(button);
        }
        window.draw(numBarDivider);
    }
};
