#include "iron_dome.hpp"

#include <sys/inotify.h>
#include <unistd.h>
#include <fstream>
#include <string>
#include <cstring>
#include <iostream>
#include <unordered_map>
#include <ctime>


std::unordered_map<std::string, time_t> entropyCheckCache;
const int CACHE_TIMEOUT = 30; // secondes

// Seuil en octets par seconde (ex: 10 MB)
constexpr long READ_THRESHOLD = 10 * 1024 * 1024;

// Stocke les lectures précédentes
std::map<pid_t, long> previousReadBytes;


void logEvent(const std::string& filename, const std::string& logFile) {
    std::cerr << "Log event called" << std::endl;
    std::ofstream log(logFile, std::ios::app);
    log << "File event: " << filename << "\n";
    log.flush();  // force l'écriture
}


void checkReadAbuse(const std::string& logFile) {
    for (const auto& dir : std::filesystem::directory_iterator("/proc")) {
        if (!dir.is_directory()) continue;

        std::string pidStr = dir.path().filename().string();
        if (!std::all_of(pidStr.begin(), pidStr.end(), ::isdigit)) continue;

        pid_t pid = std::stoi(pidStr);
        std::ifstream ioFile("/proc/" + pidStr + "/io");
        if (!ioFile.is_open()) continue;

        std::string line;
        long readBytes = 0;

        while (std::getline(ioFile, line)) {
            if (line.find("read_bytes:") == 0) {
                readBytes = std::stol(line.substr(12));
                break;
            }
        }

        long prev = previousReadBytes[pid];
        long delta = readBytes - prev;
        previousReadBytes[pid] = readBytes;

        if (delta > READ_THRESHOLD) {
            std::ofstream log(logFile, std::ios::app);
            log << "High disk read detected for PID " << pid
                << " (" << delta / 1024 << " KB/s)" << std::endl;
        }
    }
}


void watchDirectory(const std::string& path, const std::string& logFile) {
    std::cerr << "Watch directory called" << std::endl;
    int fd = inotify_init1(IN_NONBLOCK);
    if (fd < 0) return;

    int wd = inotify_add_watch(fd, path.c_str(), IN_CREATE | IN_DELETE | IN_MODIFY);
    if (wd < 0) {
        close(fd);
        return;
    }

    const int bufSize = 1024 * (sizeof(struct inotify_event) + 256);
    char buffer[bufSize];

    while (true) {
        int length = read(fd, buffer, bufSize);
        if (length < 0) {
            sleep(1);
            continue;
        }

        int i = 0;
        while (i < length) {
            struct inotify_event *event = (struct inotify_event*)&buffer[i];
            if (event->len) {
                std::string fileName = event->name;
                std::string fullPath = path + "/" + fileName;

                std::string action;
                if (event->mask & IN_CREATE) action = "created: ";
                else if (event->mask & IN_MODIFY) action = "modified: ";
                else if (event->mask & IN_DELETE) action = "deleted: ";

                logEvent(action + fileName, logFile);

                // Si fichier modifié ou créé, on vérifie l'entropie
                if ((event->mask & (IN_CREATE | IN_MODIFY)) &&
                    std::filesystem::is_regular_file(fullPath)) {

                    time_t now = time(nullptr);
                    if (entropyCheckCache.count(fullPath) &&
                        now - entropyCheckCache[fullPath] < CACHE_TIMEOUT) {
                        // Déjà analysé récemment
                    } else {
                        double entropy = calculateEntropy(fullPath);
                        entropyCheckCache[fullPath] = now;

                        if (entropy > 7.8) {
                            std::ofstream log(logFile, std::ios::app);
                            log << "⚠️ High entropy file: " << fullPath
                                << " (" << entropy << " bits/byte)" << std::endl;
                        }
                    }
                }
            }
            i += sizeof(struct inotify_event) + event->len;
        }

        checkReadAbuse(logFile);  // si cette fonction existe chez toi
        detectCryptoUsage(logFile);
        sleep(1);
    }

    inotify_rm_watch(fd, wd);
    close(fd);
}