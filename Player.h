#pragma once

#include "Inventory.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class Player {
    float hSpeed = 100;
    float hDirection = 0;
    float vSpeed = 0;
    float vAcceleration = 1000;
    float vAccelerationDirection = 1;

    sf::Vector2f size{15, 30};
    sf::Vector2f position;
    sf::Color color;
    sf::RectangleShape rect;


    float jumpSpeed = 200;
    bool isVerticalMovement = false;

    struct {
        bool left;
        bool right;
        bool jump;
    } actions{};

    PlayField &playField;
    std::shared_ptr<PlayField::letterImage> focusLetter;
    Inventory inventory;
    bool active = true;

public:
    explicit Player(PlayField &playField) : playField(playField) {
        color = Config::Workspace::TEXT_COLOR;
    }

    void setSize(sf::Vector2f size_) {
        size = size_;
    }

    sf::Vector2f getSize() {
        return size;
    }

    [[nodiscard]] bool isActive() const {
        return active;
    }
    void jump() {
        if (!isVerticalMovement) {
            vSpeed = -jumpSpeed * vAccelerationDirection;
        }
    }

    void update(float dt) {
        if (!active) {
            return;
        }

        if (actions.jump) {
            jump();
        }

        if (actions.left) {
            moveTo(position - sf::Vector2f{hSpeed * dt, 0});
            hDirection = -1;
        }

        if (actions.right) {
            moveTo(position + sf::Vector2f{hSpeed * dt, 0});
            hDirection = 1;
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

        if (playField.getHeight() < static_cast<int>(position.y)) {
            active = false;
        }
    }

    bool moveTo(sf::Vector2f pos) {
        focusLetter = playField.intersection({pos, size});
        if (focusLetter == nullptr) {
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

    std::shared_ptr<PlayField::letterImage> getFocusLetter() {
        return focusLetter;
    }

    sf::Drawable& getShape() {
        rect.setSize(size);
        rect.setPosition(position);
        rect.setFillColor(color);
        return rect;
    }

    void changeGravity() {
        this->vAccelerationDirection = -vAccelerationDirection;
    }

    sf::Drawable& getInventoryShape() {
        return inventory;
    }

    void listenEvents(EventManager& eventManager) {
        eventManager.AddEventListener(sf::Event::KeyPressed, movementListener(true));
        eventManager.AddEventListener(sf::Event::KeyReleased, movementListener(false));

        eventManager.AddEventListener(sf::Event::KeyPressed, actionsListener());
    }

private:
    void inventoryChangeEventsListen(sf::Event event) {
        if (this->focusLetter == nullptr) {
            return;
        }

        if (event.key.shift) {
            auto letterFromInventory = this->inventory.toggle();
            if (letterFromInventory) {
                this->playField.put(this->focusLetter->index + (hDirection == 1), letterFromInventory);
            }
        } else {
            this->playField.take(this->focusLetter->index);
            auto letterFromInventory = this->inventory.toggle(this->focusLetter->letter);
            if (letterFromInventory) {
                this->playField.put(this->focusLetter->index, letterFromInventory);
            }
        }
        this->playField.update();
    }

    Listener movementListener (bool flag) {
        return [this, flag] (sf::Event event) {
            if (flag && event.key.shift) {
                switch (event.key.scancode) {
                    case sf::Keyboard::Scan::A:
                        if (actions.left) {
                            return;
                        }
                        this->playField.moveLetter(focusLetter->index, -1);
                        break;
                    case sf::Keyboard::Scan::D:
                        if (actions.right) {
                            return;
                        }
                        this->playField.moveLetter(focusLetter->index, 1);
                        break;
                    default:
                        break;
                }
                this->playField.update();
                return;
            }

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
    }

    Listener actionsListener() {
        return [this] (sf::Event event) {
            switch (event.key.scancode) {
                case sf::Keyboard::Scan::Q:
                    this->changeGravity();
                    break;
                case sf::Keyboard::Scan::E:
                    this->inventoryChangeEventsListen(event);
                    break;
                case sf::Keyboard::Scan::Tab:
                    if (event.key.shift) {
                        this->inventory.backward();
                    } else {
                        this->inventory.forward();
                    }
                default:
                    break;
            }
        };
    }
};
