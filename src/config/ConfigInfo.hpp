#ifndef CONFIGINFO_HPP
#define CONFIGINFO_HPP

#include <map>
#include <set>
#include <string>
#include <vector>

#define INDENT_WIDTH 2

// LocationInfo
#define DEFAULT_INDEX "index.html"

// ServerInfo
#define DEFAULT_IP "0.0.0.0"
#define DEFAULT_PORT 4242
#define DEFAULT_CLIENT_MAX_BODY_SIZE 1024

struct Address {
    std::string ip;
    int port;
};

struct LocationInfo {
    std::string location_path;

    std::string alias;
    std::set<std::string> allow_methods;
    std::string return_path;
    bool autoindex;
    std::string index;
    bool allow_file_upload;
    std::string save_folder;
    std::vector<std::string> allow_cgi_extensions;

    LocationInfo() {
        autoindex = false;
        allow_file_upload = false;

        index = DEFAULT_INDEX;
    }
};

struct ServerInfo {
    Address listen;
    std::string server_name;
    size_t client_max_body_size;
    std::map<int, std::string> error_pages_map;
    std::map<std::string, LocationInfo> locations_info_map;

    ServerInfo() {
        listen.ip = DEFAULT_IP;
        listen.port = DEFAULT_PORT;
        client_max_body_size = DEFAULT_CLIENT_MAX_BODY_SIZE;
    }
};

struct ConfigInfo {
    std::vector<ServerInfo> servers_info;
};

void debugLocationInfo(const LocationInfo &info, int indent_level);
void debugServerInfo(const ServerInfo &info, int indent_level);
void debugConfigInfo(const ConfigInfo &info, int indent_level);

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

#endif /* CONFIGINFO_HPP */
