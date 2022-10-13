#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "Config.hpp"
#include "ConfigLexer.hpp"

class Config;

class ConfigParser {
  private:
    ConfigInfo &info_;

  public:
    ConfigParser(ConfigInfo &info);
    ~ConfigParser();

    void Run(ConfigLexer &lexer);
    void Debug();

    bool conf(ConfigLexer &lexer);
    bool server(ConfigLexer &lexer);
    bool server_conf(ConfigLexer &lexer);
    bool location_conf(ConfigLexer &lexer, LocationInfo &path);
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
