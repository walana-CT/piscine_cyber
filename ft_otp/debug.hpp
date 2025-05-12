#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <iomanip>
#include <ctime>
// Active ou désactive le mode debug
#define DEBUG 1
#define DEBUGSHA1 1

void print_hex(const std::vector<uint8_t>& vec);
void print_vect(const std::vector<uint8_t>& vect);