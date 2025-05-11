#include "hotp.hpp"
#include "hmac_sha1.hpp"  // à implémenter ou intégrer
#include <cstring>


uint32_t generate_hotp(const std::vector<uint8_t>& key, uint64_t counter) {
    uint8_t msg[8];
    for (int i = 7; i >= 0; --i) {
        msg[i] = counter & 0xFF;
        counter >>= 8;
    }

    std::vector<uint8_t> hmac = hmac_sha1(key, msg, 8);

    int offset = hmac[19] & 0x0F;
    uint32_t binary =
        ((hmac[offset] & 0x7F) << 24) |
        ((hmac[offset+1] & 0xFF) << 16) |
        ((hmac[offset+2] & 0xFF) << 8) |
        (hmac[offset+3] & 0xFF);

    return binary % 1000000;
}