#pragma once


#include <map>
#include <string>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include "Alphabet.h"
#include "util/Compilation.h"


using namespace Config::Workspace;


class Bonuses {
    sf::Vector2f bonusSize = {
        30, 30
    };
    sf::Vector2i playFieldSize{};

    struct Bonus {
        sf::Vector2f pos;
        int index;
    };
    int lastBonusIndex = 0;

    std::vector<Bonus> bonuses;

    sf::CircleShape bonusShape;

    sf::Vector2f randomPos() const {
        return (sf::Vector2f) sf::Vector2i{
            rand() % playFieldSize.x,
            rand() % playFieldSize.y
        };
    }
public:
    Bonuses() {
        srand(time(0));
        bonusShape.setRadius(bonusSize.x / 2);
        bonusShape.setFillColor(BONUS_COLOR);
        bonusShape.setOutlineColor(BONUS_STROKE_COLOR);
        bonusShape.setOutlineThickness(5);
    }

    void setPlayFieldSize(sf::Vector2i size) {
        playFieldSize = size;
    }

    void clear() {
        bonuses.clear();
    }

    void addRandomBonuses(int count = 1) {
        for (int i = 0; i < count; i++) {
            lastBonusIndex++;
            bonuses.emplace_back(randomPos(), lastBonusIndex);
        }
    }

    void addRandomCountOfRandomBonuses(int max = 10) {
        addRandomBonuses(rand() % max + 1);
    }

    Bonus* intersection(sf::FloatRect rect) {
        for (auto& bonus : bonuses) {
            if (rect.intersects(sf::Rect<float>{
                bonus.pos,
                bonusSize
            }))  {
                return &bonus;
            }
        }
        return nullptr;
    }

    void take(int bonusIndex) {
        std::erase_if(bonuses, [bonusIndex](Bonus bonus) {return bonus.index == bonusIndex;});
    }

    void draw(sf::RenderWindow& window) {
        for (const auto& bonus : bonuses) {
            bonusShape.setPosition(bonus.pos);
            window.draw(bonusShape);
        }
    }
};
