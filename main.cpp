

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>
#include "util/Image.h"
#include "PlayField.h"
#include "util/EventManager.h"
#include "util/Button.h"
#include "GameState.h"
#include "Globals.h"
#include "Menu.h"
#include "Player.h"


const std::wstring text = LR"TEXT(
#include <stdio.h>



int main() {
    int sum = 0; // Counter for the sum of numbers

    printf("Iterating through numbers from 1 to 20 with various conditions:\n");

    for (int i = 1; i <= 20; i++) {
        // Check for even or odd
        if (i % 2 == 0) {
            printf("Number %d: Even\n", i);
        } else {
            printf("Number %d: Odd\n", i);
        }

        // Check divisibility by 3
        if (i % 3 == 0) {
            printf("Number %d is divisible by 3\n", i);
        }

        // Add the number to the sum
        sum += i;
    }

    printf("\nThe sum of all numbers from 1 to 20: %d\n", sum);

    return 0;
}
)TEXT";


int main() {
//    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "qwerty", sf::Style::Fullscreen);
    sf::RenderWindow window(sf::VideoMode(800, 600), "qwerty", sf::Style::Default);


    PlayField playField;
    playField.setCode(text);
    playField.update();

    Player player(playField);
    player.setPosition({100, -100});

    EventManager commonEventManager;
    EventManager gameEventManager;
    EventManager menuEventManager;
    GameState gameState = GameState::MENU;

    commonEventManager.AddEventListener(sf::Event::Closed, [&window] (auto _) {
        window.close();
    });

    if (!font.loadFromFile("assets/Font2.ttf")) {
        exit(1);
    }

    Menu menu(window, menuEventManager, gameState);
    menu.Init();

    sf::RectangleShape background;
    background.setSize((sf::Vector2f) window.getSize());
    background.setFillColor(sf::Color(58, 58, 57));

    player.listenEvents(gameEventManager);

    sf::Clock clock;

    while (window.isOpen()) {
        auto dt = clock.restart().asSeconds();

        window.clear();
        sf::Event event{};
        while (window.pollEvent(event)) {
            switch (gameState) {
                case GameState::MENU:
                    menuEventManager.HandleEvent(event);
                    break;
                case GameState::GAME:
                    gameEventManager.HandleEvent(event);
                    break;
            }
            commonEventManager.HandleEvent(event);
        }

        switch (gameState) {
            case GameState::MENU:
                menu.update();
                menu.drawMenu();
                break;
            case GameState::GAME:
                player.update(dt);

                window.draw(background);
                auto letters = playField.letterImages;
                for (const auto& line : letters) {
                    for (const auto& letter : line.second) {
                        sf::RectangleShape rect(letter.second.hitBox.getSize());
                        rect.setPosition(letter.second.hitBox.getPosition());
                        rect.setFillColor(sf::Color(50, 50, 50));
                        window.draw(rect);
                        window.draw(letter.second.symbol);
                    }
                }
                window.draw(player.getShape());
                break;
        }

        window.display();
    }
    return 0;
}
