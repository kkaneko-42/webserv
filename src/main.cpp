#include "Server.hpp"

int main( int ac, char** av ) {
    std::string conf_file;

    if (ac > 2) {
        // print usage
        return (1);
    }
    conf_file = (ac == 2) ? av[1] : "default.conf";

    Server server;
    server.Init(conf_file);
    server.Run();

    return (0);
}
