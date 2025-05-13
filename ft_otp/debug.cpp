#include "debug.hpp"

void print_hex(const std::vector<uint8_t>& vec) {
    for (uint8_t byte : vec) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
    }
    std::cout << std::dec; // reset to decimal output
}

void print_vect(const std::vector<uint8_t>& vect) {
        for (int i = 0; i < 8; ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)vect[i] << " ";
        }
        std::cout << std::dec;  // reset to decimal output
}


//small test to test if my SHA-1 is giving the same output another one
void test_sha1(const std::string& msg_str, const std::string& expected_hex) {
    std::vector<uint8_t> data(msg_str.begin(), msg_str.end());
    std::vector<uint8_t> digest = sha1(data);

    std::ostringstream oss;
    for (uint8_t b : digest)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)b;

    std::string result = oss.str();
    std::cout << "Input    : " << msg_str << "\n";
    std::cout << "Expected : " << expected_hex << "\n";
    std::cout << "Got      : " << result << "\n";
    std::cout << ((result == expected_hex) ? "✅ OK" : "❌ MISMATCH") << "\n";
}