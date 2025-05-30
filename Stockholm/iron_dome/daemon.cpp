#include "daemon.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <iostream>


void daemonize() {
    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS); // Parent quitte

    std::cerr << "before umask" << std::endl;
    umask(0);
    std::cerr << "before setsid" << std::endl;
    setsid();
    std::cerr << "before chdir" << std::endl;

    if (chdir("/") < 0) exit(EXIT_FAILURE);

    std::cerr << "before redirect" << std::endl;

    // Ouvre /dev/null AVANT de fermer les sorties standard
    int fd = open("/dev/null", O_RDWR);
    if (fd < 0) exit(EXIT_FAILURE);

    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    if (fd > STDERR_FILENO) close(fd); // ferme fd si ce n'est pas 0/1/2

    fd = open("/app/daemon_debug.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
    dup2(fd, STDERR_FILENO);
    std::cerr << "Daemon started correctly" << std::endl;

    // À ce stade, std::cerr ne fonctionnera plus
}