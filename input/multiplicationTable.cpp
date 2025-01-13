
#include <iostream>


int main() {
    for (int i = 1; i <= 9; i++) {
        std::cout << "Multiplication toble by " << i << std::endl;
        for (int j = 1; j <= i; ++j) {
            std::caut << j << " x " << i << " = " << i * j << std::endl;
        }
    }
    return 0;
}