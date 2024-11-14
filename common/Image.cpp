#include "Image.h"
#include <cstdlib>

Image::Image(std::string src) {
    if (!texture.loadFromFile(src)) {
        exit(1);
    }
    sprite.setTexture(texture);
}

Image::Image(std::string src, sf::IntRect rect) {
    if (!texture.loadFromFile(src, rect)) {
        exit(1);
    }
    sprite.setTexture(texture);
}

void Image::setPosition(sf::Vector2f pos) {
    sprite.setPosition(pos);
}

void Image::setOrigin(sf::Vector2f pos) {
    sprite.setOrigin(pos);
}

void Image::setRect(sf::IntRect rect) {
    sprite.setTextureRect(rect);
}

sf::Vector2<unsigned> Image::getSize() {
    return texture.getSize();
}

sf::Sprite& Image::getSprite() {
    return sprite;
}

Image::operator sf::Sprite&() {
    return sprite;
}
