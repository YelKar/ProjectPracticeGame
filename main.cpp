#include "common/PixelImage.cpp"
#include "assets/letters.cpp"
#include "Game.h"

sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!"/*, sf::Style::Fullscreen*/);

int main() {
    Game::init();
    Game::loop(window);
    return 0;
}
