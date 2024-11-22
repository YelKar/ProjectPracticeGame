#include <SFML/Graphics.hpp>
#include "common/Entity.h"


class Game {
    std::vector<Entity*> entities;
    sf::RenderWindow window{sf::VideoMode(800, 600), "SFML works!", sf::Style::Default};
public:
    Game() = default;

    void addEntity(Entity& entity);
    void draw();
    void update();

    void init();
    void loop();
    void handleEvents();
};