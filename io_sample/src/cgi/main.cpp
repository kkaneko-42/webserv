#include <iostream>
#include "HttpRequest.hpp"
#include "Config.hpp"

int main( int ac, char** av ) {
    if (ac > 2) {
        return 1;
    }
    std::string conf_file = (ac == 2) ? av[1] : "default.conf";

    // ファイル -> Configオブジェクト
    Config conf;
    conf.parse(conf_file);

    // Configを使ってServer起動
    std::vector<ServerInfo> servers_info;
    for (size_t i = 0; i < servers_info.size(); ++i) {
        
    }

    return 0;
}

class Server {
    private:
        Config conf;
        struct pollfd fds[MAX_POLL_FDS];

    public:
        static const int MAX_POLL_FDS = 200;

        Run();
        launchServer();
};
