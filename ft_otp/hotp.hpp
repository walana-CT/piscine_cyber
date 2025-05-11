#pragma once
#include <vector>
#include <cstdint>

uint32_t generate_hotp(const std::vector<uint8_t>& key, uint64_t counter);