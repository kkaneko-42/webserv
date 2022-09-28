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
    error_page 400 401 402 403 404 /40x.html;

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
        root ./html/hoge
        index index.html
    }
}

server {
    listen 0.0.0.0:4243;
}

server {
    listen 0.0.0.0:4244;
}
*/

typedef struct LocationInfo {
    std::string             root;
    std::set<std::string>   allow_methods;
    std::string             return_path;
    bool                    autoindex;
    std::string             index;
    bool                    allow_file_upload;
    std::string             save_folder;
    std::set<std::string>   allow_cgi_extensions;

    // TODO: 初期化する
    struct LocationInfo() {
    }
}   LocationInfo;

typedef struct ServerInfo {
    // listen
    struct {int port, std::string addr} listen;
    
    // server_name
    std::string server_name;

    // client max body size
    size_t client_max_body_size;

    // error_page
    std::map<int, std::string> error_pages_map;

    // location
    //  => key: location path
    std::map<std::string, LocationInfo> locations_info_map;

    // TODO: 初期化する
    struct ServerInfo() {   
    }
} ServerInfo;

class Config
{
    public:
        debugConfig();
        parse();
    
    private:
        std::vector<ServerInfo> serversInfo;
};

class Server {
    public:
        Server(const ServerInfo &serverInfo);

    private:
        int sd_;
        ServerInfo conf_;
};

#endif
