#include "ConfigParser.hpp"
#include "../utils/utils.hpp"
#include <iostream>
#include <cctype>

ConfigParser::ConfigParser(ConfigInfo &info) : info_(info) {
}

ConfigParser::~ConfigParser() {
}

void ConfigParser::Run(ConfigLexer &lexer) {
    this->conf(lexer);
}

void ConfigParser::Debug() {
    std::cout << "========= parser ==========" << std::endl;
    debugConfigInfo(info_, 0);
}

void ConfigParser::conf(ConfigLexer &lexer) {
    if (lexer.Equal(TK_EOF)) {
        std::cerr << "server does not exist" << std::endl;
        exit(1);
    }

    while (!lexer.Equal(TK_EOF)) {
        this->server(lexer);
    }
    return;
}

void ConfigParser::server(ConfigLexer &lexer) {
    lexer.Skip("server");
    lexer.Skip("{");

    info_.servers_info.push_back(ServerInfo());
    while (!lexer.Equal("}")) {
        this->server_conf(lexer);
    }

    lexer.Skip("}");
}

void ConfigParser::server_conf(ConfigLexer &lexer) {
    ServerInfo &server_info = *info_.servers_info.rbegin();

    if (lexer.Equal("listen")) {
        lexer.Skip("listen");
        std::vector<std::string> splited =
            splitString(lexer.GetToken().str, ":");
        lexer.Skip(TK_WORD);

        // validation: 0.0.0.0:4242
        if (splited.size() != 2 || !this->isIpv4(splited[0]) ||
            !this->isPort(splited[1])) {
            std::cerr << "listen: error" << std::endl;
            exit(1);
        }

        server_info.listen.ip = splited[0];
        server_info.listen.port = stringToInt(splited[1]);

        lexer.Skip(";");
        return;
    }

    if (lexer.Equal("server_name")) {
        lexer.Skip("server_name");

        server_info.server_name = lexer.GetToken().str;
        lexer.Skip(TK_WORD);

        lexer.Skip(";");
        return;
    }

    if (lexer.Equal("client_max_body_size")) {
        lexer.Skip("client_max_body_size");
        std::string body_size = lexer.GetToken().str;
        lexer.Skip(TK_WORD);

        if (!this->isClientMaxBodySize(body_size)) {
            std::cerr << "client_max_body_size error" << std::endl;
            exit(1);
        }
        server_info.client_max_body_size = stringToInt(body_size);

        lexer.Skip(";");
        return;
    }

    if (lexer.Equal("error_page")) {
        lexer.Skip("error_page");

        std::vector<std::string> error_page_args;
        while (!lexer.Equal(";")) {
            error_page_args.push_back(lexer.GetToken().str);
            lexer.Skip(TK_WORD);
        }

        if (error_page_args.size() < 2) {
            std::cerr << "error_page error" << std::endl;
            exit(1);
        }

        std::string page = *(error_page_args.rbegin());

        for (size_t i = 0; i < error_page_args.size() - 1; i++) {
            std::string status_str = error_page_args[i];
            if (!isHttpStatus(status_str)) {
                std::cerr << "error_page error" << std::endl;
                exit(1);
            }
            int status = stringToSize(status_str);
            server_info.error_pages_map[status] = page;
        }

        lexer.Skip(";");
        return;
    }

    if (lexer.Equal("location")) {
        lexer.Skip("location");

        std::string path = lexer.GetToken().str;
        if (!this->isLocationPath(path)) {
            std::cerr << "location error" << std::endl;
            exit(1);
        }

        lexer.Skip(TK_WORD);
        lexer.Skip("{");

        while (!lexer.Equal("}")) {
            location_conf(lexer, server_info.locations_info_map[path]);
            server_info.locations_info_map[path].location_path = path;
        }

        lexer.Skip("}");
        return;
    }

    exit(1);
}

void ConfigParser::location_conf(ConfigLexer &lexer,
                                 LocationInfo &location_info) {
    if (lexer.Equal("alias")) {
        lexer.Skip("alias");

        location_info.alias = lexer.GetToken().str;
        // valdation
        if (!this->isDirName(location_info.alias)) {
            std::cerr << "alias error" << std::endl;
            exit(1);
        }

        lexer.Skip(TK_WORD);

        lexer.Skip(";");
        return;
    }

    if (lexer.Equal("allow_methods")) {
        lexer.Skip("allow_methods");

        std::vector<std::string> methods;
        while (!lexer.Equal(";")) {
            methods.push_back(lexer.GetToken().str);
            lexer.Skip(TK_WORD);
        }

        if (methods.size() == 0) {
            std::cerr << "allow_methods error" << std::endl;
            exit(1);
        }

        for (size_t i = 0; i < methods.size(); i++) {
            if (methods[i] != "GET" && methods[i] != "POST" &&
                methods[i] != "DELETE") {
                std::cerr << "allow_methods error" << std::endl;
                exit(1);
            }
            location_info.allow_methods.insert(methods[i]);
        }

        lexer.Skip(";");
        return;
    }

    if (lexer.Equal("return")) {
        lexer.Skip("return");

        location_info.return_path = lexer.GetToken().str;
        lexer.Skip(TK_WORD);

        lexer.Skip(";");
        return;
    }

    if (lexer.Equal("autoindex")) {
        lexer.Skip("autoindex");

        std::string autoindex_bool = lexer.GetToken().str;
        lexer.Skip(TK_WORD);

        if (autoindex_bool == "on")
            location_info.autoindex = true;
        else if (autoindex_bool == "off")
            location_info.autoindex = false;
        else {
            std::cerr << "autoindex error" << std::endl;
            exit(1);
        }

        lexer.Skip(";");
        return;
    }

    if (lexer.Equal("index")) {
        lexer.Skip("index");

        location_info.index = lexer.GetToken().str;
        lexer.Skip(TK_WORD);

        lexer.Skip(";");
        return;
    }

    if (lexer.Equal("allow_file_upload")) {
        lexer.Skip("allow_file_upload");

        std::string allow_file_upload_bool = lexer.GetToken().str;
        lexer.Skip(TK_WORD);

        if (allow_file_upload_bool == "on")
            location_info.allow_file_upload = true;
        else if (allow_file_upload_bool == "off")
            location_info.allow_file_upload = false;
        else {
            std::cerr << "allow_file_upload error" << std::endl;
            exit(1);
        }

        lexer.Skip(";");
        return;
    }

    if (lexer.Equal("save_folder")) {
        lexer.Skip("save_folder");

        location_info.save_folder = lexer.GetToken().str;
        lexer.Skip(TK_WORD);

        // validation
        if (!this->isDirName(location_info.save_folder)) {
            std::cerr << "save_folder error" << std::endl;
            exit(1);
        }

        lexer.Skip(";");
        return;
    }

    if (lexer.Equal("allow_cgi_extensions")) {
        lexer.Skip("allow_cgi_extensions");

        std::vector<std::string> extentions;
        while (!lexer.Equal(";")) {
            extentions.push_back(lexer.GetToken().str);
            lexer.Skip(TK_WORD);
        }

        if (extentions.size() < 1) {
            std::cerr << "allow_cgi_extensions error" << std::endl;
            exit(1);
        }

        for (size_t i = 0; i < extentions.size(); i++) {
            location_info.allow_cgi_extensions.push_back(extentions[i]);
        }

        lexer.Skip(";");
        return;
    }

    exit(1);
}

bool ConfigParser::isIpv4(const std::string &str) {
    std::vector<std::string> splited = splitString(str, ".");
    if (splited.size() != 4)
        return false;

    for (size_t i = 0; i < splited.size(); i++) {
        if (hasZeroPadding(splited[i]))
            return false;

        int num = 0;
        for (size_t j = 0; j < splited[i].size(); j++) {
            if (!isdigit(splited[i][j]))
                return false;
            num = num * 10 + splited[i][j] - '0';
            if (num > 255)
                return false;
        }
    }

    return true;
}

bool ConfigParser::isPort(const std::string &str) {
    if (str.size() == 0 || hasZeroPadding(str))
        return false;

    int port = 0;

    for (size_t i = 0; i < str.size(); i++) {
        if (!isdigit(str[i]))
            return false;

        port = port * 10 + str[i] - '0';

        if (port > MAX_PORT)
            return false;
    }

    return true;
}

bool ConfigParser::isLocationPath(const std::string &str) {
    return str.size() != 0 && str[0] == '/' && str[str.size() - 1] == '/';
}

bool ConfigParser::isClientMaxBodySize(const std::string &str) {
    if (!stringIsNumber(str)) {
        return false;
    }

    size_t n;
    try {
        n = stringToSize(str);
    } catch (const std::overflow_error& e) {
        return false;
    }

    return  n <= DEFAULT_CLIENT_MAX_BODY_SIZE;
}

bool ConfigParser::isDirName(const std::string &str) {
    return str.size() != 0 && str[str.size() - 1] == '/';
}

bool ConfigParser::hasZeroPadding(const std::string &str) {
    return str.size() > 1 && str[0] == '0';
}

bool ConfigParser::isHttpStatus(const std::string &str) {
    if (str.size() != 3) {
        return false;
    }
    return (stringIsNumber(str));
}
