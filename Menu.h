#pragma once


#include <SFML/Graphics/RenderWindow.hpp>
#include "util/EventManager.h"
#include "util/Button.h"
#include "GameState.h"
#include "Config.h"
#include "Game.h"


class Menu {
    sf::RenderWindow& window;
    EventManager& eventManager;
    GameState &gameState;

    std::vector<Button> buttons;

    sf::RectangleShape background;
    sf::RectangleShape numBarDivider;
    sf::Text numBarNumber;

    int btnTextSize = 120;

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
            .position = {800, 300},
            .color = sf::Color::Green,
            .listener = [this](auto){
                gameState = GameState::GAME_START;
                Config::sounds.play(Config::sounds.CLICK);
            },
        },
        {
            .text = L"resume();",
            .position = {800, 500},
            .color = sf::Color(255, 124, 0),
            .listener = [this](auto){
                gameState = GameState::GAME;
                Config::sounds.play(Config::sounds.CLICK);
            },
        }
    };

public:
    Menu(sf::RenderWindow &window, EventManager &eventManager, GameState &gameState)
        : window(window), eventManager(eventManager), gameState(gameState) {
    }

    void Init() {
        sf::Vector2f size{
            1000,
            160,
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
                    .setClickAreaPosition(buttonProps.position)
                    .Render()
                    .Connect(eventManager, sf::Event::MouseButtonPressed, buttonProps.listener)
            );
        }

        numBarDivider.setSize({5, 400});
        numBarDivider.setFillColor(sf::Color(124, 124, 124));
        numBarDivider.setPosition({750, 280});

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
            numBarNumber.setPosition(numBarDivider.getPosition().x - 100, button.getPosition().y);
            numBarNumber.setString(std::to_string(lineNum++));
            window.draw(numBarNumber);
            window.draw(button);
        }
        window.draw(numBarDivider);
    }
};
