#pragma once

#include "Inventory.h"
#include "util/Image.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>


class Cursor {
    const std::string path = "assets/cursor.png";
    Image image;
    sf::Vector2i imageSize{16, 28};

    sf::Clock walkClock;
    bool walk = false;
    int direction = -1;

public:
    Cursor() : image(path) {}

    void setPosition(sf::Vector2f pos) {
        if (direction == 1) {
            image.setPosition(
                pos - sf::Vector2f{
                    static_cast<float>(imageSize.x)
                    * image.getSprite().getScale().x,
                    0
                }
            );
        } else {
            image.setPosition(pos);
        }
    }

    void setHeight(float height) {
        auto size = image.getSize();
        image.getSprite().setScale(
            height / static_cast<float>(size.y),
            height / static_cast<float>(size.y)
        );
    }

    sf::Sprite& operator [](int num) {
        image.setRect({
            sf::Vector2i{num * imageSize.x, 2},
            imageSize
        });
        return image.getSprite();
    }

    void start(int newDirection) {
        if (!walk) {
            walk = true;
            walkClock.restart();
        }
        if (newDirection > 0) {
            auto scale = image.getSprite().getScale();
            image.getSprite().setScale(
                sf::Vector2f{
                    -1 * std::abs(scale.x),
                    scale.y
                }
            );
        } else if (newDirection < 0) {
            auto scale = image.getSprite().getScale();
            image.getSprite().setScale(
                sf::Vector2f{
                    std::abs(scale.x),
                    scale.y
                }
            );
        }
        direction = newDirection;
    }

    void stop() {
        walk = false;
    }

    operator sf::Sprite&() { // NOLINT
        if (!walk) {
            return (*this)[0];
        }
        return (*this)[static_cast<int>(walkClock.getElapsedTime().asSeconds() * 20) % 4 + 1];
    }
};

class Player {
    const float hSpeed = 100;
    float hDirection = 0;
    float vSpeed = 0;
    const float vAcceleration = 1000;
    float vAccelerationDirection = 1;

    sf::Vector2f size{20, 56};
    sf::Vector2f position;
    sf::Color color;

    float jumpSpeed = 200;
    bool isVerticalMovement = false;

    struct {
        bool left;
        bool right;
        bool jump;
    } actions{};

    PlayField &playField;
    std::shared_ptr<PlayField::letterImage> focusLetter;
    int oldFocusPos = -1;
    Inventory inventory;
    bool active = true;
    Cursor cursor;

    sf::Clock errorsShowTimer;

    bool compilationDelay = false;

public:
    explicit Player(PlayField &playField) : playField(playField) {}

    void setSize(sf::Vector2f size_) {
        size = size_;
    }

    sf::Vector2f getSize() {
        return size;
    }

    [[nodiscard]] bool isActive() const {
        return active;
    }

    void init() {
        color = Config::Workspace::TEXT_COLOR;
        cursor.setHeight(size.y);
        active = true;
        vAccelerationDirection = 1;
        vSpeed = 0;
        clearActions();
    }

    void jump() {
        if (!isVerticalMovement) {
            vSpeed = -jumpSpeed * vAccelerationDirection;
            Config::sounds.play(Config::sounds.CLICK);
            oldFocusPos = -1;
        }
    }

    void update(float dt) {
        if (compilationDelay) {
            compilationDelay = false;
            dt = 0;
        }

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

        if (actions.right - actions.left) {
            cursor.start((int) actions.right - (int) actions.left);
        } else {
            cursor.stop();
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

        if (errorsShowTimer.getElapsedTime().asMilliseconds() > 3000) {
            playField.hideErrors();
        }
    }

    bool moveTo(sf::Vector2f pos) {
        focusLetter = playField.intersection({pos, size});

        if (focusLetter == nullptr) {
            position = pos;
            return true;
        }

        if (oldFocusPos != focusLetter->index) {
            Config::sounds.play(Config::sounds.KEY);

            oldFocusPos = focusLetter->index;
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
        cursor.setPosition(position);
        return cursor;
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

    void clearActions() {
        actions = {};
    }

private:
    void inventoryChangeEventsListen(sf::Event event) {
        if (this->focusLetter == nullptr) {
            return;
        }

        Config::sounds.play(Config::sounds.TAKE);

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
                case sf::Keyboard::Scan::C:
                    playField.checkErrors();
                    errorsShowTimer.restart();
                    compilationDelay = true;
                    break;
                default:
                    break;
            }
        };
    }
};
