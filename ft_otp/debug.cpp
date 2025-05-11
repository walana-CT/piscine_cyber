#include "debug.hpp"

void print_hex(const std::vector<uint8_t>& vec) {
    for (uint8_t byte : vec) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
    }
    std::cout << std::dec << std::endl; // remet en mode décimal après
}