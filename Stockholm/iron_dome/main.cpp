#include "daemon.hpp"
#include "iron_dome.hpp"



int main() {
    // Lancer en mode daemon
    daemonize();

    // Lancer la surveillance
    watchDirectory("/home/infection", "/var/log/iron_dome.log");

    return 0;
}