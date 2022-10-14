#include "ConfigLexer.hpp"
#include <iostream>
#include <map>
#include <iomanip>

ConfigLexer::ConfigLexer() : index_(0) {
}

ConfigLexer::~ConfigLexer() {
}

void ConfigLexer::Run(std::string &str) {
    size_t i = 0;
    std::string word = "";

    while (str[i]) {
        if (std::isspace(str[i])) {
            if (word != "") {
                tok_.push_back((Token){TK_WORD, word});
                word = "";
            }
            i++;
            continue;
        }

        size_t op_len = this->getOpLen(str, i);
        if (op_len > 0) {
            if (word != "") {
                tok_.push_back((Token){TK_WORD, word});
                word = "";
            }
            tok_.push_back((Token){TK_OP, str.substr(i, op_len)});
            i += op_len;
            continue;
        }

        word += str[i];
        i++;
    }
    tok_.push_back((Token){TK_EOF, ""});
}

void ConfigLexer::Debug() {
    std::cout << "======= lexer ========" << std::endl;

    std::map<TokenKind, std::string> kind_map;
    kind_map[TK_WORD] = "TK_WORD";
    kind_map[TK_OP] = "TK_OP";
    kind_map[TK_EOF] = "TK_EOF";

    std::cout << "lexer {" << std::endl;
    for (size_t i = 0; i < tok_.size(); i++) {
        std::cout << "  { ";
        std::cout << "kind: " << std::setw(7) << kind_map[tok_[i].kind] << ", ";
        std::cout << "str: '" << tok_[i].str << "'";
        std::cout << " }," << std::endl;
    }
    std::cout << "}" << std::endl;
}

bool ConfigLexer::Equal(TokenKind kind, std::string str) {
    if (tok_[index_].kind != kind)
        return false;
    if (str != "" && tok_[index_].str != str)
        return false;
    return true;
}

bool ConfigLexer::Equal(std::string str) {
    if (str != "" && tok_[index_].str != str)
        return false;
    return true;
}

void ConfigLexer::Skip(TokenKind kind, std::string str) {
    if (tok_[index_].kind != kind) {
        exit(1);
    }
    if (str != "" && tok_[index_].str != str) {
        exit(1);
    }
    index_++;
}

void ConfigLexer::Skip(std::string str) {
    if (str != "" && tok_[index_].str != str) {
        exit(1);
    }
    index_++;
}

void ConfigLexer::Next() {
    index_++;
}

Token &ConfigLexer::GetToken() {
    return tok_[index_];
}

size_t ConfigLexer::getOpLen(std::string &str, size_t index) {
    std::string keywords[] = {";", "{", "}"};

    for (size_t i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (str.substr(index, keywords[i].size()) == keywords[i]) {
            return keywords[i].size();
        }
    }
    return 0;
}
