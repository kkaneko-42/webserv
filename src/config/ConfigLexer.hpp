#ifndef CONFIGLEXER_HPP
#define CONFIGLEXER_HPP

#include <string>
#include <vector>

enum TokenKind {
    TK_WORD,
    TK_OP,
    TK_EOF,
};

struct Token {
    TokenKind kind;
    std::string str;
};

class ConfigLexer {
  private:
    std::vector<Token> tok_;
    size_t index_;

  public:
    ConfigLexer();
    ~ConfigLexer();

    void Run(std::string &str);

    void Debug();
    bool Equal(TokenKind kind, std::string str = "");
    bool Equal(std::string str);
    void Skip(TokenKind kind, std::string str = "");
    void Skip(std::string str);
    void Next();
    Token &GetToken();

    size_t getOpLen(std::string &str, size_t i);
};

#endif /* CONFIGLEXER_HPP */
