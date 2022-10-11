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

    try {
        content = std::string(
            std::istreambuf_iterator<char>(ifs), 
            std::istreambuf_iterator<char>()
        );
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return (1);
    }

    return (0);
}
