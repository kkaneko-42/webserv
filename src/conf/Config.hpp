#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <vector>
#include <map>
#include <set>
#include <string>

/*
server {
    listen  0.0.0.0:4242;
    # root    ./html; <- いったんなしで実装。locationにかくのを必須にする。
    server_name     example.com;
    client_max_body_size 42;

    error_page 404 /404.html;
        or 
    error_page 400 401 402 403 404 ./40x.html;

    location / {
        (必須)root ./html;
        allow_methods GET POST DELETE;
        return https://www.google.com/; // redirection
        autoindex on; <- index.htmlがあればそれを返す、なければlsページを返す
        index index.html;
        allow_file_upload off;
        save_folder ./upload; (なければrootにおく)

        allow_cgi_extensions py sh; (なければ無効)
    }

    location /hoge/ {
        root ./html/hoge;
        index index.html;
    }
}

server {
    listen 0.0.0.0:4243;

    error_page 400 ./hoge_error.html
}

server {
    listen 0.0.0.0:4244;
}
*/

class Server;

struct LocationInfo {
    std::string                 location_path;
    std::string                 root;
    std::vector<std::string>    allow_methods;
    std::string                 return_path;
    bool                        autoindex;
    std::string                 index;
    bool                        allow_file_upload;
    std::string                 save_folder;
    std::vector<std::string>    allow_cgi_extensions;

    bool locationMathcing( const std::string& path, const std::string& host, const Server& server );

    /*
    // TODO: 初期化する
    LocationInfo()
    : root(""), allow_methods(std::set<std::string>()), return_path(""),
    autoindex(false), index(""), allow_file_upload(false), save_folder(""),
    allow_cgi_extensions(std::vector<std::string>())
    {}
    */
};

struct ServerInfo {
    // listen
    struct { int port; std::string addr; } listen;

    // server_name
    std::string server_name;

    // client max body size
    size_t client_max_body_size;

    // error_page
    std::map<int, std::string> error_pages_map;

    // location
    //  => key: location path
    std::map<std::string, LocationInfo> locations_info_map;

    /*
    // TODO: 初期化する
    ServerInfo()
    : listen({0, "0.0.0.0"}), server_name(""), client_max_body_size(-1),
    error_pages_map(std::map<int, std::string>()),
    locations_info_map(std::map<std::string, LocationInfo>())
    {}
    */
};

class Config {
    public:
        enum TokenType {
            WORD,
            OP
        };

        struct Token {
            std::string str;
            TokenType type;
        };

        void debugConfig( void );
        int lexer(
            std::string& file_content,
            std::vector<Token>& tokens );
        /* BNF Rules */
        int conf(
            const std::vector<Token>& tokens,
            std::vector<Token>::const_iterator& it );
        int server(
            const std::vector<Token>& tokens,
            std::vector<Token>::const_iterator& it );
        int server_conf(
            const std::vector<Token>& tokens,
            std::vector<Token>::const_iterator& it );
        int listen_conf(
            const std::vector<Token>& tokens,
            std::vector<Token>::const_iterator& it );
        int error_page_conf(
            const std::vector<Token>& tokens,
            std::vector<Token>::const_iterator& it );
        int location_conf(
            const std::vector<Token>& tokens,
            std::vector<Token>::const_iterator& it,
            const std::string& location_path );
        int parse( const std::string& filepath );

        std::vector<ServerInfo> getServersInfo( void ) const {
            return serversInfo;
        }

    private:
        int doParse( const std::vector<Token>& tokens );
        int parseError( int status );
        std::vector<ServerInfo> serversInfo;
};
/*
class Server {
    public:
        Server( const ServerInfo &serverInfo );

    private:
        int sd_;
        ServerInfo conf_;
};
*/

#endif /* CONFIG_HPP */
