#include "common/PixelImage.cpp"
#include "assets/letters.cpp"

sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!"/*, sf::Style::Fullscreen*/);

int main() {

    while (window.isOpen())
    {
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

        window.clear();
        drawPixelImage(window, LETTERS['A'], {20, 20}, 10);
        window.display();
    }
    return 0;
}
