#include "debug.hpp"

void print_hex(const std::vector<uint8_t>& vec) {
    for (uint8_t byte : vec) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
    }
    std::cout << std::dec; // remet en mode décimal après
}

void print_vect(const std::vector<uint8_t>& vect) {
        for (int i = 0; i < 8; ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)vect[i] << " ";
        }
        std::cout << std::dec;  // reset to decimal output
}
