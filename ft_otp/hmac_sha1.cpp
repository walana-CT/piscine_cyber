#include "hmac_sha1.hpp"
#include <vector>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include "debug.hpp"

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


// Définir la taille des blocs (512 bits pour SHA-1)
#define SHA1_BLOCK_SIZE 64
#define SHA1_DIGEST_SIZE 20


// Fonction de rotation de bits à gauche (shift circulaire)
inline uint32_t ROTATE_LEFT(uint32_t value, uint32_t bits) {
    return (value << bits) | (value >> (32 - bits));
}

// Fonction F selon l'algorithme SHA-1 pour chaque étape
inline uint32_t F(uint32_t t, uint32_t B, uint32_t C, uint32_t D) {
    if (t < 20) return (B & C) | (~B & D); // Fonction F pour 0 <= t < 20
    if (t < 40) return B ^ C ^ D;          // Fonction F pour 20 <= t < 40
    if (t < 60) return (B & C) | (B & D) | (C & D); // Fonction F pour 40 <= t < 60
    return B ^ C ^ D;                      // Fonction F pour t >= 60
}

std::vector<uint8_t> sha1(const std::vector<uint8_t>& data) {
    size_t length = data.size();
    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xEFCDAB89;
    uint32_t h2 = 0x98BADCFE;
    uint32_t h3 = 0x10325476;
    uint32_t h4 = 0xC3D2E1F0;

    std::vector<uint8_t> msg(data.begin(), data.end());

    // 1. Padding du message pour qu'il ait une longueur multiple de 512 bits
    msg.push_back(0x80); // Ajouter 0x80 (bit '1' suivi de 7 bits '0')

    // Compléter jusqu'à ce que la taille soit 64 bits avant la fin du message
    while (msg.size() % SHA1_BLOCK_SIZE != 56) {
        msg.push_back(0x00); // Ajouter des zéros jusqu'à atteindre 56 octets (448 bits)
    }

    // 2. Ajouter la longueur du message original en bits (sur 8 octets)
    uint64_t bit_length = length * 8;
    for (int j = 0; j < 8; ++j) {
        msg.push_back(static_cast<uint8_t>((bit_length >> (56 - j * 8)) & 0xFF));
    }

    // 3. Traitement du message par blocs de 512 bits (64 octets)
    for (size_t i = 0; i < msg.size() / SHA1_BLOCK_SIZE; ++i) {
        uint32_t W[80];

        // Remplir les 16 premiers mots de W (32 bits chacun)
        for (int t = 0; t < 16; ++t) {
            W[t] = (msg[i * SHA1_BLOCK_SIZE + t * 4] << 24) |
                   (msg[i * SHA1_BLOCK_SIZE + t * 4 + 1] << 16) |
                   (msg[i * SHA1_BLOCK_SIZE + t * 4 + 2] << 8) |
                   (msg[i * SHA1_BLOCK_SIZE + t * 4 + 3]);
        }

        // Étendre les 16 mots en 80 mots
        for (int t = 16; t < 80; ++t) {
            W[t] = ROTATE_LEFT(W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16], 1);
        }

        uint32_t A = h0;
        uint32_t B = h1;
        uint32_t C = h2;
        uint32_t D = h3;
        uint32_t E = h4;

        // Application de la fonction SHA-1 pour chaque étape
        for (int t = 0; t < 80; ++t) {
            uint32_t K;
            if (t < 20)       K = 0x5A827999;
            else if (t < 40)  K = 0x6ED9EBA1;
            else if (t < 60)  K = 0x8F1BBCDC;
            else              K = 0xCA62C1D6;

            uint32_t temp = ROTATE_LEFT(A, 5) + F(t, B, C, D) + E + W[t] + K;
            E = D;
            D = C;
            C = ROTATE_LEFT(B, 30);
            B = A;
            A = temp;
        }

        // Mise à jour des variables de hachage
        h0 += A;
        h1 += B;
        h2 += C;
        h3 += D;
        h4 += E;
    }

    // 4. Retourner le résultat sous forme de digest (20 octets)
    std::vector<uint8_t> result(SHA1_DIGEST_SIZE);
    result[0] = (h0 >> 24) & 0xFF;
    result[1] = (h0 >> 16) & 0xFF;
    result[2] = (h0 >> 8) & 0xFF;
    result[3] = h0 & 0xFF;

    result[4] = (h1 >> 24) & 0xFF;
    result[5] = (h1 >> 16) & 0xFF;
    result[6] = (h1 >> 8) & 0xFF;
    result[7] = h1 & 0xFF;

    result[8] = (h2 >> 24) & 0xFF;
    result[9] = (h2 >> 16) & 0xFF;
    result[10] = (h2 >> 8) & 0xFF;
    result[11] = h2 & 0xFF;

    result[12] = (h3 >> 24) & 0xFF;
    result[13] = (h3 >> 16) & 0xFF;
    result[14] = (h3 >> 8) & 0xFF;
    result[15] = h3 & 0xFF;

    result[16] = (h4 >> 24) & 0xFF;
    result[17] = (h4 >> 16) & 0xFF;
    result[18] = (h4 >> 8) & 0xFF;
    result[19] = h4 & 0xFF;

    return result;
}

std::vector<uint8_t> hmac_sha1(const std::vector<uint8_t>& key, const uint8_t* msg, size_t msg_len) {
    // Step 1: if the key is longer than 64-bytes, we hash it with SHA-1
        #if DEBUG
            std::cout << "  calculating HMAC-SHA-1(K,C)" << std::endl;
            std::cout << "  HMAC-SHA-1(K,C) = SAH-1(K XOR opad, SHA-1(K XOR ipad, C))" << std::endl << std::endl;            
        #endif


    std::vector<uint8_t> key_copy = key;
    if (key_copy.size() > SHA1_BLOCK_SIZE) {
        #if DEBUG
            std::cout << "key hashed with sha 1 to reduce it to 64-bit" << std::endl;
        #endif
        key_copy = sha1(key_copy); // Hacher la clé avec SHA-1
    }

    //Step 2: if the key is shorter than 64-bytes, we fill it with zeros.
    #if DEBUG
        std::cout << "key filled with 0 to make it it to 64-bit" << std::endl << std::endl;
    #endif
    if (key_copy.size() < SHA1_BLOCK_SIZE) {
        key_copy.resize(SHA1_BLOCK_SIZE, 0x00); // Compléter avec des zéros
    }

    // creating opad and ipad
    std::vector<uint8_t> ipad(SHA1_BLOCK_SIZE, 0x36);
    std::vector<uint8_t> opad(SHA1_BLOCK_SIZE, 0x5C);

    // XOR the key with opad and ipad
    for (size_t i = 0; i < SHA1_BLOCK_SIZE; ++i) {
        ipad[i] ^= key_copy[i];
        opad[i] ^= key_copy[i];
    }

    #if DEBUG
        std::cout << "K XOR opad = ";
        print_vect(opad);
        std::cout << std::endl << "K XOR ipad = ";
        print_vect(ipad);  
        std::cout << std::endl << std::endl;     
    #endif    

    // Step 3: claculating the hmac
    //calculating SHA-1(K XOR ipad, C)
    std::vector<uint8_t> inner_input = ipad;
    inner_input.insert(inner_input.end(), msg, msg + msg_len);
    #if DEBUG
        std::cout << "    Calculating: innerhash = SHA-1(K XOR ipad, C)" << std::endl;
        print_hex(inner_input);
        std::cout << " ---->" << std::endl;
    #endif
    std::vector<uint8_t> inner_hash = sha1(inner_input);
    #if DEBUG
        print_hex(inner_hash);
        std::cout << std::endl << std::endl;
    #endif

    // Appliquer SHA-1 sur (opad || hash(ipad || msg))
    std::vector<uint8_t> outer_input = opad;
    outer_input.insert(outer_input.end(), inner_hash.begin(), inner_hash.end());
    #if DEBUG
        std::cout << "    Calculating: HMAC-SHA-1(K,C) = SAH-1(K XOR opad, innerhash)" << std::endl;
        print_hex(inner_input);
        std::cout << " ---->" << std::endl;
    #endif
    return sha1(outer_input);

}