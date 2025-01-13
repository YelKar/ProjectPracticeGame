#pragma once


#include <SFML/Graphics/RectangleShape.hpp>
#include "PlayField.h"
#include <queue>
#include <cmath>

class Platform {
    bool taken = false;
    sf::RectangleShape shape;
    const sf::Vector2f defaultPosition{0, -200};
    sf::Vector2f position = defaultPosition;

    const sf::Vector2f size{100, 8};
    std::queue<sf::Vector2f> targets{};
    PlayField &playField;
    float speed = 200;

    bool gotTarget = false;

    template<typename T>
    T sign(T num) {
        if (num < 0) {
            return -1;
        }
        if (num > 0) {
            return 1;
        }
        return 0;
    }

    void updatePosition(sf::Vector2f target, float dt) {
        float yStep = std::min(dt * speed, std::abs(target.y - position.y));

        if (static_cast<int>(position.y) == static_cast<int>(target.y)) {
            if (playField.intersection(getRect()) == nullptr) {
                if (position.x > target.x) {
                    position.x -= std::min(dt * speed, std::abs(target.x - position.x));
                } else if (position.x < target.x) {
                    position.x += std::min(dt * speed, std::abs(target.x - position.x));
                }
            }
        } else if (playField.intersection(getRect()) != nullptr) {
            position.x += dt * speed;
        } else {
            position.y += sign(target.y - position.y) * yStep;
        }
    }

public:
    explicit Platform(PlayField &playField) : playField(playField) {
        shape.setSize(size);
        shape.setFillColor(Config::Workspace::PLATFORM_COLOR);
    }

    void addTarget(sf::Vector2f newTarget) {
        targets.push(newTarget - sf::Vector2f{size.x / 2, size.y});
    }

    [[nodiscard]] bool isGotTarget() const {
        return gotTarget;
    }

    void nextTarget() {
        targets.pop();
    }

    void clearTarget() {
        while(!targets.empty()) {
            targets.pop();
        }
    }

    bool intersects(sf::Rect<float> rect) {
        return rect.intersects(sf::FloatRect{position, size});
    }

    void update(float dt) {
        if (targets.empty()) {
            updatePosition(defaultPosition, dt);
        } else {
            updatePosition(targets.front(), dt);
        }
    }

    operator sf::Drawable&() {
        shape.setPosition(position);
        return shape;
    }

    sf::FloatRect getRect() {
        return {
            position, size
        };
    }

    bool hasTargets() {
        return !targets.empty();
    }

    void take() {
        taken = true;
    }

    void put() {
        taken = false;
    }

    void toggle() {
        taken = !taken;
    }

    bool isTaken() const {
        return taken;
    }
};
