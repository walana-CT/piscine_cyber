#pragma once
#include <vector>
#include <string>

void encrypt_key_to_file(const std::string& filename, const std::vector<uint8_t>& key);
std::vector<uint8_t> decrypt_key_from_file(const std::string& filename);
std::vector<uint8_t> hex_to_bytes(const std::string& hex);