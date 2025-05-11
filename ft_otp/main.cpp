#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <iomanip>
#include <ctime>
#include "hotp.hpp"
#include "crypto.hpp"
#include "debug.hpp"

// Fonction pour lire la clé depuis un fichier
std::string read_key_from_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file for reading: " << filename << std::endl;
        return {};
    }

    // Lire le contenu du fichier
    std::string key((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return key;
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./otp -g <hexkey> | -k\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string filename = "ft_otp.key";

    if (mode == "-g") {
        if (argc != 3) {
            std::cerr << "Usage: ./otp -g <hexkey>\n";
            return 1;
        }

        std::string hexkey = read_key_from_file(argv[2]);
        if (hexkey.size() < 64 || hexkey.find_first_not_of("0123456789abcdefABCDEF") != std::string::npos) {
            std::cerr << "Invalid hex key (min 64 hex characters).\n";
            return 1;
        }

        std::vector<uint8_t> key = hex_to_bytes(hexkey);
        encrypt_key_to_file(filename, key);
        std::cout << "Key stored successfully.\n";

    } else if (mode == "-k") {
        std::vector<uint8_t> key = decrypt_key_from_file(filename);
        if (key.empty()) {
            std::cerr << "Failed to load key.\n";
            return 1;
        }

        uint64_t counter = static_cast<uint64_t>(std::time(nullptr)) / 30;  // TOTP style counter

        #if DEBUG
            std::cout << "about to generate totp" << std::endl;
            std::cout << "the code is given by following formula" << std::endl << "  HOTP(K,C) = Truncate(HMAC-SHA-1(K,C))" << std::endl << std::endl;
            std::cout << "  K : key stored in " << filename << std::endl << "  ";
            print_hex(key); 
            std::cout << std::endl;        
            std::cout << "  C : message. here obtained via global unix time" << filename << std::endl << "  ";
            std::cout << counter << std::endl << std::endl;
        #endif

        uint32_t otp = generate_hotp(key, counter);

        #if DEBUG
            std::cout << "obtained otp: " << otp << std::endl;
        #endif

        std::cout << std::setfill('0') << std::setw(6) << otp << std::endl;

    } else {
        std::cerr << "Unknown option.\n";
        return 1;
    }

    return 0;
}