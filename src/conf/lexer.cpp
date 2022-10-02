#include <cctype>
#include <string>
#include <vector>
#include <iostream>
#include "Config.hpp"

static void skipSpace( std::string& str, std::string::size_type& idx ) {
    while (std::isspace(str[idx]) && idx < str.size()) {
        ++idx;
    }
}

static bool isTokenLiteral( char c ) {
    return (std::isalnum(c) || c == '/' || c == '_' || c == '.'
        || c == '{' || c == '}' || c == ':');
}

static void addToken(
    std::string& token_str,
    Config::TokenType type,
    std::vector<Config::Token>& tokens )
{
    Config::Token token = { token_str, type };
    tokens.push_back(token);
    token_str = "";
}

static void printVector(std::vector<Config::Token>& v) {
    std::cout << "[";
    for (size_t i = 0; i < v.size(); i++) {
        std::cout << "{ " << v[i].str << ", " << v[i].type << "}," << std::endl;
    }
    std::cout << "]" << std::endl;
}

int Config::lexer(
    std::string& file_content,
    std::vector<Token>& tokens )
{
    std::string token_str = "";
    std::string::size_type i = 0;

    while (i < file_content.size()) {
        char c = file_content[i++];

        if (std::isspace(c)) {  
            skipSpace(file_content, i);
            if (token_str != "") {
                addToken(token_str, WORD, tokens);
            }
        } else if (c == ';') {
            addToken(token_str, WORD, tokens);
            token_str = c;
            addToken(token_str, OP, tokens);
        } else if (isTokenLiteral(c)) {
            token_str += c;
            if (i == file_content.size() - 1) {
                addToken(token_str, WORD, tokens);
            }
        }
    }

    // printVector(tokens);
    return (0);
}
