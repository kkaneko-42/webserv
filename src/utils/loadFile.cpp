#include "utils.hpp"
#include <fstream>
#include <iostream>

int loadFile( const std::string& path, std::string& content ) {
    std::string buf = "";
    std::ifstream ifs(path.c_str());
    if (!ifs) {
        std::cerr << "open failed" << std::endl;
        return (1);
    }

    while(!ifs.eof()) {
        if (!ifs) {
            std::cerr << "read failed" << std::endl;
            return (1);
        }
        std::getline(ifs, buf);
        content += buf + "\r\n";
    }
    /*
    res = std::string(std::istreambuf_iterator<char>(ifs), 
                    std::istreambuf_iterator<char>());
    */

    return (0);
}
