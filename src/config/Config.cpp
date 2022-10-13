#include "Config.hpp"
#include "../utils/utils.hpp"
#include <iostream>

void Config::Parse(std::string filename) {
    std::string content;
    if (loadFile(filename, content)) {
        std::cerr << "loadFile() failure" << std::endl;
        exit(1);
    }

    ConfigLexer lexer;
    lexer.Run(content);
    lexer.Debug();

    ConfigParser parser(info_);
    parser.Run(lexer);
    parser.Debug();
}

const ConfigInfo &Config::GetConf() const {
    return info_;
}
