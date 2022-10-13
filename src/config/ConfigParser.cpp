#include "ConfigParser.hpp"
#include "../utils/utils.hpp"
#include <iostream>

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

bool ConfigParser::conf(ConfigLexer &lexer) {
    while (!lexer.Equal(TK_EOF)) {
        bool ret = this->server(lexer);
        if (!ret) {
            exit(1);
        }
    }
    return true;
}

bool ConfigParser::server(ConfigLexer &lexer) {
    lexer.Skip("server");
    lexer.Skip("{");

    info_.servers_info.push_back(ServerInfo());
    while (!lexer.Equal("}")) {
        bool ret = this->server_conf(lexer);
        if (!ret) {
            exit(1);
        }
    }

    lexer.Skip("}");
    return true;
}

bool ConfigParser::server_conf(ConfigLexer &lexer) {
    ServerInfo &server_info = *info_.servers_info.rbegin();

    if (lexer.Equal("listen")) {
        lexer.Skip("listen");
        std::vector<std::string> splited =
            splitString(lexer.GetToken().str, ":");
        lexer.Skip(TK_WORD);

        // TODO: validate 0.0.0.0:4242
        if (splited.size() != 2) {
            std::cerr << "listen" << std::endl;
            exit(1);
        }
        server_info.listen.ip = splited[0];
        server_info.listen.port = stringToInt(splited[1]);

        lexer.Skip(";");
        return true;
    }

    if (lexer.Equal("server_name")) {
        lexer.Skip("server_name");

        server_info.server_name = lexer.GetToken().str;
        lexer.Skip(TK_WORD);

        lexer.Skip(";");
        return true;
    }

    if (lexer.Equal("client_max_body_size")) {
        lexer.Skip("client_max_body_size");

        // TODO: validate NUM
        server_info.client_max_body_size = stringToInt(lexer.GetToken().str);
        lexer.Skip(TK_WORD);

        lexer.Skip(";");
        return true;
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
            // TODO: validate NUM
            int status = stringToInt(error_page_args[i]);
            server_info.error_pages_map[status] = page;
        }

        lexer.Skip(";");
        return true;
    }

    if (lexer.Equal("location")) {
        lexer.Skip("location");
        std::string path = lexer.GetToken().str;
        lexer.Skip(TK_WORD);
        lexer.Skip("{");

        while (!lexer.Equal("}")) {
            // TODO: pathの重複は許す？
            bool ret = location_conf(lexer, server_info.locations_info_map[path]);
            if (!ret) {
                exit(1);
            }
            server_info.locations_info_map[path].location_path = path;
            // TODO: validation
        }

        lexer.Skip("}");
        return true;
    }

    return false;
}

bool ConfigParser::location_conf(ConfigLexer &lexer,
                                 LocationInfo &location_info) {
    if (lexer.Equal("alias")) {
        lexer.Skip("alias");

        location_info.alias = lexer.GetToken().str;
        lexer.Skip(TK_WORD);

        lexer.Skip(";");
        return true;
    }

    if (lexer.Equal("allow_methods")) {
        lexer.Skip("allow_methods");

        std::vector<std::string> methods;
        while (!lexer.Equal(";")) {
            methods.push_back(lexer.GetToken().str);
            lexer.Skip(TK_WORD);
        }

        for (size_t i = 0; i < methods.size(); i++) {
            // TODO: refactor
            if (methods[i] != "GET" && methods[i] != "POST" &&
                methods[i] != "DELETE") {
                std::cerr << "allow_methods error" << std::endl;
                exit(1);
            }
            location_info.allow_methods.insert(methods[i]);
        }

        lexer.Skip(";");
        return true;
    }

    if (lexer.Equal("return")) {
        lexer.Skip("return");

        location_info.return_path = lexer.GetToken().str;
        lexer.Skip(TK_WORD);

        lexer.Skip(";");
        return true;
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
        return true;
    }

    if (lexer.Equal("index")) {
        lexer.Skip("index");

        location_info.index = lexer.GetToken().str;
        lexer.Skip(TK_WORD);

        lexer.Skip(";");
        return true;
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
        return true;
    }

    if (lexer.Equal("save_folder")) {
        lexer.Skip("save_folder");

        location_info.save_folder = lexer.GetToken().str;
        lexer.Skip(TK_WORD);

        lexer.Skip(";");
        return true;
    }

    if (lexer.Equal("allow_cgi_extensions")) {
        lexer.Skip("allow_cgi_extensions");

        std::vector<std::string> extentions;
        while (!lexer.Equal(";")) {
            extentions.push_back(lexer.GetToken().str);
            lexer.Skip(TK_WORD);
        }

        for (size_t i = 0; i < extentions.size(); i++) {
            location_info.allow_cgi_extensions.push_back(extentions[i]);
        }

        lexer.Skip(";");
        return true;
    }

    return false;
}
