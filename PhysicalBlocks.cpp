#include <SFML/Graphics/Sprite.hpp>
#include "PlayField.h"

class PhysicalBlock {
    PlayField &playField;
    sf::Vector2i size;
    using Point = sf::Vector2i;

    std::vector<Point> points;
    float angle = 0;
    sf::Texture& texture;
public:
    PhysicalBlock(PlayField &playField_, sf::Vector2i size_, sf::Texture& texture_)
    : playField(playField_), size(size_), texture(texture_) {
    }
};