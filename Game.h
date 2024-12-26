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
#include "Sound.h"
#include "util/Compilation.h"


class Game {
    std::wstring code;

    EventManager commonEventManager;
    EventManager gameEventManager;
    EventManager menuEventManager;
    EventManager gameOverScreenEventManager;


    GameState gameState = GameState::MENU;

//    sf::RenderWindow window{sf::VideoMode(1900, 1000), "qwerty", sf::Style::Default};
    sf::RenderWindow window{sf::VideoMode::getDesktopMode(), "qwerty", sf::Style::Fullscreen};
    sf::View camera;
    PlayField playField;
    GameOverScreen gameOverScreen;

    Player player;

    Menu menu;

    sf::Clock clock;
    Button quitBtn;

    std::vector<std::string> levels = {
        "multiplicationTable.cpp",
        "3.c",
        "4.cpp",
    };

    int currentLevel = 0;

public:
    Game()
    : player(playField),
    menu(window, menuEventManager, gameState),
    gameOverScreen(
        gameOverScreenEventManager,
        sf::Vector2i{static_cast<int>(window.getSize().x), static_cast<int>(window.getSize().y / 2)},
        gameState
    ) {
        if (!Config::font.loadFromFile(Config::FONT_PATH)) {
            exit(1);
        }
        camera.setSize((sf::Vector2f) window.getSize());
        clear();

        quitBtn
            .setSize({24, 24})
            .setText(L"×")
            .setInsideTextPosition({5, 0})
            .setTextColor(sf::Color::White)
            .setBackgroundColor(sf::Color(200, 0, 0))
            .setFont(Config::font)
            .setTextSize(20)
            .Connect(commonEventManager, sf::Event::MouseButtonPressed, [this](auto){
                window.close();
            });
    }

    void Init() {
        menu.Init();

        player.listenEvents(gameEventManager);

        commonEventManager.AddEventListener(sf::Event::Closed, [this](auto _) {
            window.close();
        });

        commonEventManager.AddEventListener(sf::Event::KeyPressed, [this](sf::Event event) {
            if (event.key.scancode == sf::Keyboard::Scan::Escape) {
                gameState = GameState::MENU;
                player.clearActions();
            }
        });

        clock.restart();
    }

    void clear() {
        IO::getTextFromFile("input/" + levels[currentLevel], code);
        player.setPosition({0, -100});
        player.init();

        playField.setCode(code);
        playField.update();
    }

    void nextLevel() {
        currentLevel ++;
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
                gameOverScreen.getShape().setPosition(
                    camera.getCenter().x - static_cast<float>(window.getSize().x) / 2,
                    camera.getCenter().y
                );
                playField.draw(window);
                window.draw(gameOverScreen.getShape());
                break;
            case GameState::MENU:
                menu.update();
                menu.drawMenu();
                camera.setCenter(window.getDefaultView().getCenter());
                break;
            case GameState::GAME:
                gameUpdate(dt);
                break;
            case GameState::GAME_START:
                clear();
                gameUpdate(dt);
                std::cout << "start\n";
                gameState = GameState::GAME;
                break;
        }
        quitBtn
            .setPosition(
                sf::Vector2f{
                    camera.getCenter().x + static_cast<float>(window.getSize().x) / 2.f - quitBtn.getSize().x - 5,
                    camera.getCenter().y - static_cast<float>(window.getSize().y) / 2.f + 5
                }
            )
            .setClickAreaPosition(
                sf::Vector2f{
                    static_cast<float>(window.getSize().x) - quitBtn.getSize().x - 5,
                    5
                }
            )
            .Render();
        window.setView(camera);
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
                default:
                    break;
            }
            commonEventManager.HandleEvent(event);
        }
    }

    void gameUpdate(float dt) {
        player.update(dt);

        if (!player.isActive()) {
            gameState = GameState::GAME_END;
            Config::sounds.play(Config::sounds.TAKE);

            auto result = Compilation::compileAndRun(playField.getCode());

            gameOverScreen.setText(result.text);
            gameOverScreen.isWon(result.ok);

            if (result.ok) {
                nextLevel();
            }
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
