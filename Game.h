#include <SFML/Graphics.hpp>


class Game {
public:
    static void init() {

    }

    static void loop(sf::RenderWindow& window) {
        while (window.isOpen())
        {
            handleEvents(window);

            window.clear();

            window.display();
        }
    }

    static void handleEvents(sf::RenderWindow& window) {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                default:
                    break;
            }
        }
    }
};