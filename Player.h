#pragma once


#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class Player {
    float hSpeed = 200;
    float vSpeed = 0;
    float vAcceleration = 1000;
    float vAccelerationDirection = 1;

    sf::Vector2f size{20, 30};
    sf::Vector2f position;
    sf::Color color;
    sf::RectangleShape rect;


    float jumpSpeed = 400;
    bool isVerticalMovement = false;

    struct {
        bool left;
        bool right;
        bool jump;
    } actions{};

    PlayField &playField;

public:
    explicit Player(PlayField &playField) : playField(playField) {
        color = sf::Color::White;
    }

    void setSize(sf::Vector2f size_) {
        size = size_;
    }

    sf::Vector2f getSize() {
        return size;
    }

    void jump() {
        if (!isVerticalMovement) {
            vSpeed = -jumpSpeed * vAccelerationDirection;
        }
    }

    void update(float dt) {
        if (actions.jump) {
            jump();
        }

        if (actions.left) {
            moveTo(position - sf::Vector2f{hSpeed * dt, 0});
        }

        if (actions.right) {
            moveTo(position + sf::Vector2f{hSpeed * dt, 0});
        }

        vSpeed += vAcceleration * vAccelerationDirection * dt;
        auto newYPosition = position.y + vSpeed * dt;
        if (moveTo({position.x, newYPosition})) {
            isVerticalMovement = true;
        } else {
            if (
                vSpeed > 0 && vAccelerationDirection > 0
                || vSpeed < 0 && vAccelerationDirection < 0
            ) {
                isVerticalMovement = false;
            }
            vSpeed = 0;
        }
    }

    bool moveTo(sf::Vector2f pos) {
        if (!playField.intersects({pos, size})) {
            position = pos;
            return true;
        }
        return false;
    }

    void setPosition(sf::Vector2f pos) {
        position = pos;
    }

    sf::Vector2f getPosition() {
        return position;
    }

    void listenEvents(EventManager& eventManager) {
        auto actionsListener = [this] (bool flag) {
            return [this, flag] (sf::Event event) {
                switch (event.key.scancode) {
                    case sf::Keyboard::Scan::Space:
                    case sf::Keyboard::Scan::W:
                    case sf::Keyboard::Scan::Up:
                        actions.jump = flag;
                        break;
                    case sf::Keyboard::Scan::A:
                    case sf::Keyboard::Scan::Left:
                        actions.left = flag;
                        break;
                    case sf::Keyboard::Scan::D:
                    case sf::Keyboard::Scan::Right:
                        actions.right = flag;
                        break;
                    default:
                        break;
                }
            };
        };
        eventManager.AddEventListener(sf::Event::KeyPressed, actionsListener(true));
        eventManager.AddEventListener(sf::Event::KeyReleased, actionsListener(false));

        eventManager.AddEventListener(sf::Event::KeyPressed, [this] (sf::Event event) {
            switch (event.key.scancode) {
                case sf::Keyboard::Scan::Q:
                    this->vAccelerationDirection = -vAccelerationDirection;
                default:
                    break;
            }
        });
    }

    sf::Drawable& getShape() {
        rect.setSize(size);
        rect.setPosition(position);
        rect.setFillColor(color);
        return rect;
    }
};
