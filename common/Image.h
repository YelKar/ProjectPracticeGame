#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Image {
    sf::Texture texture{};
    sf::Sprite sprite{};

public:
    explicit Image(std::string src);
    Image(std::string src, sf::IntRect rect);

    void setPosition(sf::Vector2f pos);
    void setOrigin(sf::Vector2f pos);
    void setRect(sf::IntRect rect);

    sf::Vector2<unsigned> getSize();
    sf::Sprite& getSprite();

    explicit operator sf::Sprite&();
};
