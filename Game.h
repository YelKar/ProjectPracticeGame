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
#include "Bonuses.h"
#include "Platform.h"
#include "Shortcuts.h"


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
    Bonuses bonuses;
    GameOverScreen gameOverScreen;

    Player player;

    Menu menu;

    sf::Clock clock;
    Button quitBtn;
    Platform platform;

    Shortcuts shortcutRender;

    std::vector<std::string> levels = {
        "multiplicationTable.cpp",
        "3.c",
        "4.cpp",
    };

    int currentLevel = 0;
    std::map<GameState, std::vector<Shortcut>> shortcuts;

public:
    Game()
    : player(playField),
    platform(playField),
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
        initShortcuts();

        player.listenEvents(gameEventManager);

        initEventListeners();

        clock.restart();
    }

    void initEventListeners() {
        commonEventManager.AddEventListener(sf::Event::Closed, [this](auto _) {
            window.close();
        });

        commonEventManager.AddEventListener(sf::Event::KeyPressed, [this](sf::Event event) {
            if (event.key.scancode == sf::Keyboard::Scan::Escape) {
                gameState = GameState::MENU;
                player.clearActions();
            }
        });

        gameEventManager.AddEventListener(sf::Event::KeyPressed, [this](sf::Event event) {
            if (event.key.scancode == sf::Keyboard::Scan::T) {
                if (event.key.shift) {
                    if (platform.getRect().intersects(sf::FloatRect(player.getPosition(), player.getSize()))) {
                        platform.toggle();
                    } else {
                        platform.put();
                    }
                } else if (platform.hasTargets()) {
                    platform.nextTarget();
                } else {
                    givePlatform();
                }
            }
        });
    }

    void clear() {
        IO::getTextFromFile("input/" + levels[currentLevel], code);
        player.setPosition({0, -100});
        player.init();

        playField.setCode(code);
        playField.update();

        bonuses.clear();
        bonuses.setPlayFieldSize(playField.getSize());
        bonuses.addRandomCountOfRandomBonuses(4);
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
                shortcutRender.Render(shortcuts[GameState::GAME_END]);
                shortcutRender.updatePosition((sf::Vector2f) window.getSize(), camera.getCenter());
                window.draw(shortcutRender);
                break;
            case GameState::MENU:
                menu.update();
                menu.drawMenu();
                camera.setCenter(window.getDefaultView().getCenter());
                break;
            case GameState::GAME:
                gameUpdate(dt);
                shortcutRender.Render(shortcuts[GameState::GAME]);
                shortcutRender.updatePosition((sf::Vector2f) window.getSize(), camera.getCenter());
                window.draw(shortcutRender);
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

        auto intersection = bonuses.intersection({player.getPosition(), player.getSize()});
        if (intersection != nullptr) {
            bonuses.take(intersection->index);
            player.activateBonus();
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

        auto inventory = (sf::Sprite&) player.getInventoryShape();
        inventory.setPosition(
            sf::Vector2f (window.getSize() - inventory.getTexture()->getSize())
            + camera.getCenter() - sf::Vector2f (window.getSize()) / 2.f
        );

        platform.update(dt);
        updatePlatformCollision();

        bonuses.draw(window);
        window.draw(player.getShape());
        window.draw(inventory);

        window.draw(platform);

        window.draw(shortcutRender);
        window.setView(camera);
    }

    void updatePlatformCollision() {
        if (platform.isTaken()) {
            auto platformRect = platform.getRect();
            player.setPosition(sf::Vector2f{
                platformRect.left + (platformRect.width - player.getSize().x) / 2,
                platformRect.top - player.getSize().y + platformRect.height - 5,
            });
            if (!platform.hasTargets()) {
                platform.put();
            }
        }
        if (player.makeAction()) {
            platform.put();
        }
    }

    void givePlatform() {
        if (playField.visibleErrors()) {
            platform.addTarget(player.getPosition() + sf::Vector2f{0, player.getSize().y});
            auto error = playField.getFirstErrorPosition();
            platform.addTarget(error + sf::Vector2f{0, 4});
        }
    }

    void initShortcuts() {
        shortcuts[GameState::GAME].push_back({
            .keys = L"A / ArrayLeft",
            .description = L"Движение налево",
        });
        shortcuts[GameState::GAME].push_back({
            .keys = L"D / ArrayRight",
            .description = L"Движение направо",
        });
        shortcuts[GameState::GAME].push_back({
            .keys = L"W / ArrayUp",
            .description = L"Прыжок",
        });
        shortcuts[GameState::GAME].push_back({
            .keys = L"Q",
            .description = L"Изменить гравитацию",
        });
        shortcuts[GameState::GAME].push_back({
            .keys = L"E",
            .description = L"Взять/Обменять буквы",
        });
        shortcuts[GameState::GAME].push_back({
            .keys = L"Shift+E",
            .description = L"Положить букву",
        });
        shortcuts[GameState::GAME].push_back({
            .keys = L"Shift+A",
            .description = L"Сдвинуть букву влево",
        });
        shortcuts[GameState::GAME].push_back({
            .keys = L"Shift+D",
            .description = L"Сдвинуть букву вправо",
        });
        shortcuts[GameState::GAME].push_back({
            .keys = L"T",
            .description = L"Вызвать платформу\nВозможно только при действии бонуса"
        });
        shortcuts[GameState::GAME].push_back({
            .keys = L"Shift+T",
            .description = L"Встать/Сойти"
        });


        shortcuts[GameState::GAME_END].push_back({
            .keys = L"ESC",
            .description = L"Выйти"
        });
    }
};
