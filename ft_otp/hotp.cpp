#include "hotp.hpp"
#include "hmac_sha1.hpp"  // à implémenter ou intégrer
#include <cstring>
#include "debug.hpp"
#include <iostream>
#include "iomanip"
#include "debug.hpp"

uint32_t generate_hotp(const std::vector<uint8_t>& key, uint64_t counter) {

    uint8_t msg[8];
    for (int i = 7; i >= 0; --i) {
        msg[i] = counter & 0xFF;
        counter >>= 8;
    }

    #if DEBUG
        std::cout << "  ";
        for (int i = 0; i < 8; ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)msg[i] << " ";
        }
        std::cout << std::dec << std::endl << std::endl << std::endl;  // reset to decimal output      
    #endif

    std::vector<uint8_t> hmac = hmac_sha1(key, msg, 8);

    #if DEBUG
        print_hex(hmac);
        std::cout << std::endl << std::endl;
    #endif


    #if DEBUG
        std::cout << "    dynamic truncation of hmac: " << std::endl;
        print_hex(hmac);
    #endif
    //dynamic truncating 
    int offset = hmac[19] & 0x0F;
    uint32_t binary =
        ((hmac[offset] & 0x7F) << 24) |
        ((hmac[offset+1] & 0xFF) << 16) |
        ((hmac[offset+2] & 0xFF) << 8) |
        (hmac[offset+3] & 0xFF);


    return binary % 1000000;
}