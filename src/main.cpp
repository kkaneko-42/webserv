#include "Server.hpp"
#include <csignal>

int main( int ac, char** av ) {
    std::string conf_file;

    if (ac > 2) {
        std::cerr << USAGE << std::endl;
        return (1);
    }
    conf_file = (ac == 2) ? av[1] : "./default.conf";

    Server server;
    if (server.Init(conf_file)) {
        return (1);
    }

    signal(SIGPIPE, SIG_IGN);
    if (server.Run()) {
        return (1);
    }

    return (0);
}
