#pragma once
#include <string>
#include "crypto_detect.hpp"
#include "entropy.hpp"


void watchDirectory(const std::string& path, const std::string& logFile);