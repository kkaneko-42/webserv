#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <string>
#include <vector>
#include <exception>
#include <iostream>

class HttpMethod;

class HttpRequest {
    public:
        // TODO: header valuesはenum等で状態として管理。stringのままは使いづらそう
        struct Headers {
            std::vector<std::string>    accept;
            std::string                 accept_language; // 複数取れる？
            std::vector<std::string>    accept_encoding;
            std::string                 user_agent;
            std::string                 host; // http1.1では唯一必須
            std::string                 connection;
        };

        HttpRequest( void );
        ~HttpRequest( void );
        std::string getPath( void ) const;
        HttpMethod* getMethod( void ) const { return method_; }
        int parse( const std::string& request );
        void printRequest( void ) const;

    private:
        int parseTop( const std::string& top_row );
        int parseHeader( const std::vector<std::string>& headers );
        
        HttpMethod* method_; // deletable(allocate in heap, or NULL)
        std::string path_; // /hoge
        std::string version_; // HTTP/1.1
        Headers headers_;
        std::string body_;
};

#endif
