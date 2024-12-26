#pragma once


#include <string>
#include <vector>
#include <SFML/Audio.hpp>
#include <iostream>
#include <map>
#include <memory>

class Sounds {
    std::map<std::string, std::shared_ptr<sf::SoundBuffer>> sounds{};
    sf::Sound sound;
public:
    const std::string KEY = "assets/key.ogg";
    const std::string CLICK = "assets/click.ogg";
    const std::string TAKE = "assets/take.ogg";

    Sounds() {
        add(KEY);
        add(CLICK);
        add(TAKE);
    }

    void add(const std::string& path) {
        auto buffer = std::make_shared<sf::SoundBuffer>();
        if (!buffer->loadFromFile(path)) {
            std::cerr << "File " << path << " is not found" << std::endl;
            exit(1);
        }
        sounds[path] = buffer;
    }

    std::shared_ptr<sf::SoundBuffer> get(const std::string &path) {
        return sounds[path];
    }

    void play(const std::string &path) {
        sound.setBuffer(*this->get(path));
        sound.play();
    }
};
