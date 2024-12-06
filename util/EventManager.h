#pragma once


#include <unordered_map>
#include <SFML/Window/Event.hpp>
#include <functional>
#include <list>
#include <iostream>

using Listener = std::function<void(sf::Event&)>;

class EventManager {
    std::unordered_map<sf::Event::EventType, std::vector<Listener>> listeners{};
public:
    void AddEventListener(sf::Event::EventType type, const Listener& listener) {
        listeners[type].push_back(listener);
    }

    void HandleEvent(sf::Event event) {
        for (const auto& listener : listeners[event.type]) {
            listener(event);
        }
    }

    void HandleEvent(sf::Event::EventType type, sf::Event event) {
        for (const auto& listener : listeners[type]) {
            listener(event);
        }
    }
};
