


#include "Game.h"


int main() {
//    std::wstring text;
//    IO::getTextFromFile("input/1.c", text);
//
//    sf::Vector2i workSpaceSize = {800, 400};
//
////    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "qwerty", sf::Style::Fullscreen);
//    sf::RenderWindow window(sf::VideoMode(800, 600), "qwerty", sf::Style::Default);
//
//    sf::View camera;
//    camera.setSize((sf::Vector2f) window.getSize());
//    PlayField playField;
//    playField.setCode(text);
//    playField.update();
//
//    Player player(playField);
//    player.setPosition({0, -100});
//
//    EventManager commonEventManager;
//    EventManager gameEventManager;
//    EventManager menuEventManager;
//    GameState gameState = GameState::MENU;
//
//    commonEventManager.AddEventListener(sf::Event::Closed, [&window, &playField] (auto _) {
//        std::cout << "hello\n";
//        window.close();
//    });
//
//    if (!Config::font.loadFromFile(Config::FONT_PATH)) {
//        exit(1);
//    }
//
//    Menu menu(window, menuEventManager, gameState);
//    menu.Init();
//
//    player.listenEvents(gameEventManager);
//    // todo разделить длинные функции на покороче
//    // todo 20-30 строк, 2-3 уровня вложенности
//    sf::Clock clock;
//
//    while (window.isOpen()) {
//        auto dt = clock.restart().asSeconds();
//
//        window.clear();
//        sf::Event event{};
//        while (window.pollEvent(event)) {
//            switch (gameState) {
//                case GameState::MENU:
//                    menuEventManager.HandleEvent(event);
//                    break;
//                case GameState::GAME:
//                    gameEventManager.HandleEvent(event);
//                    break;
//                case GameState::GAME_END:
//                    break;
//            }
//            commonEventManager.HandleEvent(event);
//        }
//
//        switch (gameState) {
//            case GameState::GAME_END:
//
//                break;
//            case GameState::MENU:
//                menu.update();
//                menu.drawMenu();
//                break;
//            case GameState::GAME:
//                player.update(dt);
//
//                if (!player.isActive()) {
//                    gameState = GameState::GAME_END;
//                }
//
//                auto playerPos = player.getPosition();
//                auto playerSize = player.getSize();
//                camera.setCenter(
//                    std::max(playerPos.x + playerSize.x / 2, static_cast<sf::Vector2f>(window.getSize()).x / 2),
//                    std::max(playerPos.y + playerSize.y / 2, static_cast<sf::Vector2f>(window.getSize()).y / 2)
//                );
//
//                auto focusedLetter = player.getFocusLetter();
//
//                playField.setFocus(focusedLetter != nullptr ? focusedLetter->index : -1);
//                playField.draw(window);
//
//                window.draw(player.getShape());
//                auto inventory = (sf::Sprite&) player.getInventoryShape();
//                inventory.setPosition(
//                    sf::Vector2f (window.getSize() - inventory.getTexture()->getSize())
//                    + camera.getCenter() - sf::Vector2f (window.getSize()) / 2.f
//                );
//                window.draw(inventory);
//                window.setView(camera);
//                break;
//        }
//        window.display();
//    }
    Game game;
    game.Init();
    game.Loop();
    return 0;
}
