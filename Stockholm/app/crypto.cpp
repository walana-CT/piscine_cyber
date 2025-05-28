#include <fstream>
#include <vector>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>
#include <cryptopp/modes.h>
#include <cryptopp/aes.h>
#include <cryptopp/osrng.h>
#include <iostream>
#include <stdexcept>

using namespace CryptoPP;

void encryptFile(const std::string& inFile, const std::string& key) {
    try {
        AutoSeededRandomPool prng;
        byte iv[AES::BLOCKSIZE];
        prng.GenerateBlock(iv, sizeof(iv));

        CBC_Mode<AES>::Encryption encryptor;
        encryptor.SetKeyWithIV(reinterpret_cast<const byte*>(key.data()), key.size(), iv);

        // Temp output file path
        std::string tempFile = inFile + ".tmp";

        FileSink fs(tempFile.c_str());
        fs.Put(iv, AES::BLOCKSIZE); // Write IV first

        FileSource fsIn(inFile.c_str(), true,
            new StreamTransformationFilter(encryptor, new Redirector(fs))
        );

        std::remove(inFile.c_str());
        std::rename(tempFile.c_str(), inFile.c_str());
    }
    catch (const CryptoPP::Exception& e) {
        std::cerr << "Encryption error: " << e.what() << "\n";
    }
}

void decryptFile(const std::string& inFile, const std::string& key) {
    try {
        std::ifstream fin(inFile, std::ios::binary);
        if (!fin) throw std::runtime_error("Cannot open file: " + inFile);

        // Lire IV
        byte iv[AES::BLOCKSIZE];
        fin.read(reinterpret_cast<char*>(iv), AES::BLOCKSIZE);

        // Lire le reste du fichier (le ciphertext)
        std::vector<byte> ciphertext((std::istreambuf_iterator<char>(fin)),
                                     std::istreambuf_iterator<char>());
        fin.close();

        CBC_Mode<AES>::Decryption decryptor;
        decryptor.SetKeyWithIV(reinterpret_cast<const byte*>(key.data()), key.size(), iv);

        // Déchiffrer dans un buffer
        std::string recovered;

        StringSource ss(ciphertext.data(), ciphertext.size(), true,
            new StreamTransformationFilter(decryptor,
                new StringSink(recovered)
            )
        );

        // Écrire dans un fichier temporaire
        std::string tempFile = inFile + ".tmp";
        std::ofstream fout(tempFile, std::ios::binary);
        fout.write(recovered.data(), recovered.size());
        fout.close();

        std::remove(inFile.c_str());
        std::rename(tempFile.c_str(), inFile.c_str());
    }
    catch (const CryptoPP::Exception& e) {
        std::cerr << "Decryption error: " << e.what() << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Decryption file error: " << e.what() << "\n";
    }
}