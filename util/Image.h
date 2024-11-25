#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Image : public sf::Texture {
    sf::Sprite sprite{};
    sf::Image image;

public:
    explicit Image(const std::string& src) {
        if (!image.loadFromFile(src)) {
            exit(1);
        }

        this->loadFromImage(image);
        sprite.setTexture(*this);
    }
    Image(const std::string& src, sf::IntRect rect) {
        if (!this->loadFromFile(src, rect)) {
            exit(1);
        }
        sprite.setTexture(*this);
    }

    void setMaskColor(sf::Color color) {
        image.createMaskFromColor(color);
        this->loadFromImage(image);
        sprite.setTexture(*this);
    }

    void setPosition(sf::Vector2f pos) {
        sprite.setPosition(pos);
    }

    void setOrigin(sf::Vector2f pos) {
        sprite.setOrigin(pos);
    }

    void setRect(sf::IntRect rect) {
        sprite.setTextureRect(rect);
    }

    sf::Sprite& getSprite() {
        return sprite;
    }

    explicit operator sf::Sprite&() {
        return sprite;
    }
};

