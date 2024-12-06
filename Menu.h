#pragma once


#include <SFML/Graphics/RenderWindow.hpp>
#include "util/EventManager.h"
#include "util/Button.h"
#include "GameState.h"
#include "Config.h"

class Menu {
    sf::RenderWindow& window;
    EventManager& eventManager;
    GameState &gameState;

    std::vector<Button> buttons;

    sf::RectangleShape background;
    sf::RectangleShape numBarDivider;
    sf::Text numBarNumber;

    int btnTextSize = 60;

    sf::RenderTexture texture;
    sf::Sprite shape;

    struct ButtonProps {
        std::wstring text;
        sf::Vector2f position;
        sf::Color color;
        Listener listener;
    };

    const std::vector<ButtonProps> buttonsProps = {
        {
            .text = L"start();",
            .position = {200, 200},
            .color = sf::Color::Green,
            .listener = [this](auto){
                std::cout << "39\n";
                gameState = GameState::GAME;
                std::cout << "41\n";
            },
        },
        {
            .text = L"resume();",
            .position = {200, 300},
            .color = sf::Color(255, 124, 0),
            .listener = [this](auto){
                gameState = GameState::GAME;
            },
        }
    };
public:

    Menu(sf::RenderWindow &window, EventManager &eventManager, GameState &gameState)
        : window(window), eventManager(eventManager), gameState(gameState) {
    }

    void Init() {
//        texture.create()

        sf::Vector2f size{
            500,
            80,
        };

        background.setSize((sf::Vector2f) window.getSize());
        background.setFillColor(sf::Color(58, 58, 57));


        for (const ButtonProps& buttonProps : buttonsProps) {
            buttons.emplace_back(
                (new Button{})
                    ->setBackgroundColor(background.getFillColor())
                    .setTextSize(btnTextSize)
                    .setTextColor(buttonProps.color)
                    .setFont(Config::font)
                    .setText(buttonProps.text)
                    .setSize(size)
                    .setPosition(buttonProps.position)
                    .Render()
                    .Connect(eventManager, sf::Event::MouseButtonPressed, buttonProps.listener)
            );
        }

        numBarDivider.setSize({3, 220});
        numBarDivider.setFillColor(sf::Color(124, 124, 124));
        numBarDivider.setPosition({180, 180});

        numBarNumber.setFont(Config::font);
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
            window.draw(numBarNumber);
            window.draw(button);
        }
        window.draw(numBarDivider);
    }
};
