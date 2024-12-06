#pragma once


#include <string>
#include <SFML/Graphics.hpp>
#include "util/EventManager.h"
#include "GameState.h"
#include "PlayField.h"
#include "Player.h"
#include "util/IO.h"
#include "Menu.h"
#include "GameOverScreen.h"

class Game {
    std::wstring code;

    EventManager commonEventManager;
    EventManager gameEventManager;
    EventManager menuEventManager;
    EventManager gameOverScreenEventManager;


    GameState gameState = GameState::MENU;

    sf::RenderWindow window{sf::VideoMode(800, 600), "qwerty", sf::Style::Default};
//    sf::RenderWindow window{sf::VideoMode::getDesktopMode(), "qwerty", sf::Style::Fullscreen};
    sf::View camera;
    PlayField playField;
    GameOverScreen gameOverScreen;

    Player player;

    Menu menu;

    sf::Clock clock;
    Button quitBtn;


public:
    Game()
    : player(playField),
    menu(window, menuEventManager, gameState),
    gameOverScreen(gameOverScreenEventManager, (sf::Vector2i) window.getSize()) {
        if (!Config::font.loadFromFile(Config::FONT_PATH)) {
            exit(1);
        }

        IO::getTextFromFile("input/1.c", code);
        camera.setSize((sf::Vector2f) window.getSize());

        player.setPosition({0, -100});

        playField.setCode(code);
        playField.update();

        quitBtn
            .setSize({24, 24})
            .setPosition({static_cast<float>(window.getSize().x) - 25 - 5, 5})
            .setText(L"×")
            .setInsideTextPosition({5, 0})
            .setTextColor(sf::Color::White)
            .setBackgroundColor(sf::Color(200, 0, 0))
            .setFont(Config::font)
            .setTextSize(20)
            .Connect(commonEventManager, sf::Event::MouseButtonPressed, [this](auto){
                window.close();
            })
            .Render();
    }

    void Init() {
        menu.Init();

        player.listenEvents(gameEventManager);

        commonEventManager.AddEventListener(sf::Event::Closed, [this](auto _) {
            window.close();
        });

        clock.restart();
    }

    void Loop() {
        while (window.isOpen()) {
            auto dt = clock.restart().asSeconds();

            handleEvents();
            window.clear();
            update(dt);
            window.draw(quitBtn);
            window.display();
        }
    }

    void update(float dt) {
        switch (gameState) {
            case GameState::GAME_END:
                window.draw(gameOverScreen.getShape());
                window.setView(camera);
                break;
            case GameState::MENU:
                menu.update();
                menu.drawMenu();
                window.setView(window.getDefaultView());
                break;
            case GameState::GAME:
                gameUpdate(dt);
                window.setView(camera);
                break;
        }
    }

    void handleEvents() {
        sf::Event event{};
        while (window.pollEvent(event)) {
            switch (gameState) {
                case GameState::MENU:
                    menuEventManager.HandleEvent(event);
                    break;
                case GameState::GAME:
                    gameEventManager.HandleEvent(event);
                    break;
                case GameState::GAME_END:
                    gameOverScreenEventManager.HandleEvent(event);
                    break;
            }
            commonEventManager.HandleEvent(event);
        }
    }

    void gameUpdate(float dt) {
        player.update(dt);

        if (!player.isActive()) {
            gameState = GameState::GAME_END;

            auto result = IO::compileAndRun(playField.getCode());
            camera.setCenter(sf::Vector2f{window.getSize()} / 2.f);

            gameOverScreen.setText(result.text);
            gameOverScreen.isWon(result.ok);
            return;
        }

        auto playerPos = player.getPosition();
        auto playerSize = player.getSize();
        camera.setCenter(
            std::max(playerPos.x + playerSize.x / 2, static_cast<sf::Vector2f>(window.getSize()).x / 2),
            std::max(playerPos.y + playerSize.y / 2, static_cast<sf::Vector2f>(window.getSize()).y / 2)
        );

        auto focusedLetter = player.getFocusLetter();

        playField.setFocus(focusedLetter != nullptr ? focusedLetter->index : -1);
        playField.draw(window);

        window.draw(player.getShape());
        auto inventory = (sf::Sprite&) player.getInventoryShape();
        inventory.setPosition(
            sf::Vector2f (window.getSize() - inventory.getTexture()->getSize())
            + camera.getCenter() - sf::Vector2f (window.getSize()) / 2.f
        );
        window.draw(inventory);
        window.setView(camera);
    }
};
