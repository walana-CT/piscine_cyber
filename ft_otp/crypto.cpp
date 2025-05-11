#include "crypto.hpp"
#include <fstream>

const uint8_t XOR_KEY = 0x5A;  // Clé de chiffrement simple

void encrypt_key_to_file(const std::string& filename, const std::vector<uint8_t>& key) {
    std::ofstream ofs(filename, std::ios::binary);
    for (auto b : key)
        ofs.put(b ^ XOR_KEY);
}

std::vector<uint8_t> decrypt_key_from_file(const std::string& filename) {
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) return {};
    std::vector<uint8_t> key;
    char ch;
    while (ifs.get(ch))
        key.push_back(static_cast<uint8_t>(ch) ^ XOR_KEY);
    return key;
}

std::vector<uint8_t> hex_to_bytes(const std::string& hex) {
    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < hex.length(); i += 2) {
        uint8_t byte = std::stoi(hex.substr(i, 2), nullptr, 16);
        bytes.push_back(byte);
    }
    return bytes;
}