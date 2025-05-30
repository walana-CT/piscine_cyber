#include <fstream>
#include <vector>
#include <cmath>
#include <filesystem>
#include <iostream>


double calculateEntropy(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) return 0.0;

    std::vector<size_t> freq(256, 0);
    char byte;
    size_t total = 0;

    while (file.get(byte)) {
        freq[static_cast<unsigned char>(byte)]++;
        total++;
    }

    if (total == 0) return 0.0;

    double entropy = 0.0;
    for (size_t count : freq) {
        if (count == 0) continue;
        double p = static_cast<double>(count) / total;
        entropy -= p * std::log2(p);
    }

    return entropy;
}



void detectHighEntropyFiles(const std::string& directory, const std::string& logFile) {
    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
        if (!entry.is_regular_file()) continue;

        double entropy = calculateEntropy(entry.path().string());
        if (entropy > 7.8) {
            std::ofstream log(logFile, std::ios::app);
            log << "High entropy detected: " << entry.path()
                << " (" << entropy << " bits/byte)" << std::endl;
        }
    }
}