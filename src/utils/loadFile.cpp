#include "utils.hpp"
#include <fstream>

std::string loadFile( const std::string& path ) {
    std::string res = "", buf = "";
    std::ifstream ifs(path.c_str());
    if (!ifs) {
        // TODO:
        std::exit(1);
    }

    while (!ifs.eof()) {
        std::getline(ifs, buf);
        res += buf + "\n";
    }

    if (!ifs) {
        // TODO:
        std::exit(1);
    }

    return (res);
}
