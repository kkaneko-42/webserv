#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <string>
#include <vector>
#include <exception>
#include <iostream>
#include "../conf/Config.hpp"

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
            std::set<std::string>       transfer_encoding;
        };

        HttpRequest( void );
        ~HttpRequest( void );
        std::string getPath( void ) const;
        std::string getRawPath( void ) const { return path_; }
        std::string getUploadPath( void ) const;
        std::string getHostName( bool flag_resolve = false ) const;
        ServerInfo getHostInfo( void ) const { return host_info_; }
        LocationInfo getLocationInfo( void ) const { return location_info_; }
        HttpMethod* getMethod( void ) const { return method_; }
        std::string getBody( void ) const { return body_; }
        bool isChunked( void ) const;
        int unchunk( void );
        int parse( const std::string& request );
        int hostMatching(
            const std::vector<ServerInfo>& servers_info,
            std::pair< std::string, int > listen_addr
            );
        int locationMatching( void );
        void printRequest( void ) const;

    private:
        int parseTop( const std::string& top_row );
        int parseHeader( const std::vector<std::string>& headers );

        HttpMethod* method_; // deletable(allocate in heap, or NULL)
        ServerInfo host_info_;
        LocationInfo location_info_;
        std::string path_; // /hoge
        std::string version_; // HTTP/1.1
        Headers headers_;
        std::string body_;
};

#endif
