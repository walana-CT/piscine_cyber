#pragma once
#include <vector>
#include <cstdint>

// Fonction qui calcule le hash SHA-1
std::vector<uint8_t> sha1(const std::vector<uint8_t>& data);
std::vector<uint8_t> hmac_sha1(const std::vector<uint8_t>& key, const uint8_t* msg, size_t msg_len);