#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <regex>

bool processUsesCryptoLibs(const std::string& pid) {
    std::ifstream maps("/proc/" + pid + "/maps");
    if (!maps) return false;

    std::string line;
    while (std::getline(maps, line)) {
        if (line.find("libcrypto") != std::string::npos ||
            line.find("libssl") != std::string::npos) {
            return true;
        }
    }
    return false;
}

void detectCryptoUsage(const std::string& logFile) {
    for (const auto& entry : std::filesystem::directory_iterator("/proc")) {
        if (!entry.is_directory()) continue;
        std::string pid = entry.path().filename().string();
        if (!std::regex_match(pid, std::regex("[0-9]+"))) continue;

        if (!processUsesCryptoLibs(pid)) continue;

        std::ifstream stat("/proc/" + pid + "/stat");
        if (!stat) continue;

        std::string ignore;
        long utime = 0, stime = 0;
        for (int i = 0; i < 13; ++i) stat >> ignore;
        stat >> utime >> stime;

        long total_time = utime + stime;
        if (total_time > 1000) { // Seuil à adapter
            std::ofstream log(logFile, std::ios::app);
            log << "⚠️ Processus suspect PID " << pid
                << " utilise une lib crypto avec " << total_time << " ticks CPU\n";
        }
    }
}