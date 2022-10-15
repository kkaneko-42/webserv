#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "Config.hpp"
#include "ConfigLexer.hpp"

#define MAX_PORT 65535

class ConfigParser {
  private:
    ConfigInfo &info_;

  public:
    ConfigParser(ConfigInfo &info);
    ~ConfigParser();

    void Run(ConfigLexer &lexer);
    void Debug();

    void conf(ConfigLexer &lexer);
    void server(ConfigLexer &lexer);
    void server_conf(ConfigLexer &lexer);
    void location_conf(ConfigLexer &lexer, LocationInfo &path);

    bool isPort(const std::string &str);
    bool isIpv4(const std::string &str);
    bool isLocationPath(const std::string &str);
    bool isClientMaxBodySize(const std::string &str);
    bool isHttpStatus(const std::string &str);

    bool isDirName(const std::string &str);
    bool hasZeroPadding(const std::string &str);
};

/*
# EBNF
<conf>        = (<server>)* EOF

<server>      = "server" "{" (<server_conf>)+ "}"
<server_conf> = "listen" <address_word> ";"
              | "server_name" <word> ";"
              | "client_max_body_size" <num> ";"
              | "error_page" (<num>)+ <word> ";"
              | <location>

<location>    = "location" <word> "{" (<location_conf>)+ "}"
<location_conf> = "alias" <word> ";"
                | "allow_methods" (<word>)+ ";"
                | "return" <word> ";"
                | "autoindex" <word> ";"
                | "index" <word> ";"
                | "allow_file_upload" <word> ";"
                | "save_folder" <word> ";"
                | "allow_cgi_extensions" (<word>)+ ";"
*/

#endif /* CONFIGPARSER_HPP */
