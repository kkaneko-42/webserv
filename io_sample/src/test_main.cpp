#include "Config.hpp"

int main(int ac, char **av) {
    std::string filepath;

    if (ac < 2) {
        return (1);
    }
    filepath = av[1];

    Config conf;
    conf.parse(filepath);
    conf.debugConfig();

    return (0);
}
