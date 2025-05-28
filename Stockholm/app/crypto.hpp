#ifndef CRYPTO_HPP
#define CRYPTO_HPP

#include <string>

void encryptFile(const std::string& filename, const std::string& key);
void decryptFile(const std::string& filename, const std::string& key);

#endif