#pragma once


#include <unordered_map>
#include <SFML/Window/Event.hpp>
#include <functional>
#include <list>

class EventManager {
    std::unordered_map<sf::Event::EventType, std::vector<std::function<void(sf::Event&)>>> listeners{};
public:
    void AddEventListener(sf::Event::EventType type, const std::function<void(sf::Event&)>& listener) {
        listeners[type].push_back(listener);
    }

    void HandleEvent(sf::Event event) {
        for (const auto& listener : listeners[event.type]) {
            listener(event);
        }
    }
};
