#pragma once

#include <SFML/Graphics.hpp>

struct Pixel {
    sf::Color color;
    sf::Vector2f pos;
};

using PixelImage = std::vector<Pixel>;


void drawPixelImage(sf::RenderWindow& window, const PixelImage& image, sf::Vector2f pos, float pixelSize) {
    sf::RectangleShape pixelShape({pixelSize, pixelSize});

    for (auto pixel : image) {
        pixelShape.setPosition(pixel.pos * pixelSize + pos);
        pixelShape.setFillColor(pixel.color);

        window.draw(pixelShape);
    }
}
