#include "Config.hpp"
#include "../utils/utils.hpp"
#include "../Server.hpp"
#include <fstream>
#include <cctype>
#include <iostream>

int lexer(
    const std::string& file_content,
    std::vector<std::string>& tokens );

int Config::parse( const std::string& filepath ) {
    // load config file
    std::string file_content;
    if (loadFile(filepath, file_content)) {
        return (parseError(1));
    }

    // lexical analyze
    std::vector<Token> tokens;
    if (lexer(file_content, tokens)) {
        return (parseError(1));
    }

    // execute parse
    if (this->doParse(tokens)) {
        return (parseError(1));
    }

    return (0);
}

int Config::parseError( int status ) {
    std::cerr << "parse error" << std::endl;
    serversInfo.clear();
    return (status);
}

template <typename T>
static void printVector(std::vector<T> v) {
    std::cout << "[";
    for (size_t i = 0; i < v.size(); i++) {
        std::cout << v[i] << ", ";
    }
    std::cout << "]" << std::endl;
}

void Config::debugConfig( void ) {
    size_t i = 0;
    std::vector<ServerInfo>::iterator it;
    for (it = serversInfo.begin(); it < serversInfo.end(); ++it, ++i) {
        std::cout << "====== server [" << i << "] ============" << std::endl;
        std::cout << "listen: " << it->listen.addr << ":" << it->listen.port << std::endl;
        std::cout << "server_name: " << it->server_name << std::endl;
        std::cout << "client_max_body_size: " << it->client_max_body_size << std::endl;
        std::cout << "error_pages_map: {" << std::endl;
        std::map<int, std::string>::iterator it2;
        for (it2 = it->error_pages_map.begin(); it2 != it->error_pages_map.end(); ++it2) {
            std::cout << it2->first << " : " << it2->second << "," << std::endl;
        }
        std::cout << "}" << std::endl;
        std::map<std::string, LocationInfo>::iterator it3;
        for (it3 = it->locations_info_map.begin(); it3 != it->locations_info_map.end(); ++it3) {
            std::cout << "@@@ location " << it3->first << " info @@@" << std::endl;
            LocationInfo info = it3->second;
            std::cout << "  alias: " << info.alias << std::endl;
            std::cout << "  allow_methods: ";
            printVector(info.allow_methods);
            std::cout << "  return_path: " << info.return_path << std::endl;
            std::cout << "  autoindex: " << ((info.autoindex) ? "true" : "false") << std::endl;
            std::cout << "  index: " << info.index << std::endl;
            std::cout << "  allow_file_upload: " << ((info.allow_file_upload) ? "true" : "false") << std::endl;
            std::cout << "  save_folder: " << info.save_folder << std::endl;
            std::cout << "  allow_cgi_extensions: ";
            printVector(info.allow_cgi_extensions);
        }
    }
}
