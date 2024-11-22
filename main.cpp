#include <unordered_map>
#include "src/Game.h"
#include "src/Ball.h"

int main() {
    std::unordered_map<int, sf::Event::EventType> map{};
    std::cout << map.count(0) << std::endl;
    return 0;
}
