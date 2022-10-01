#include "Config.hpp"
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>

int Config::doParse( const std::vector<Token>& tokens ) {
    std::vector<Token>::const_iterator begin = tokens.begin();
    if (conf(tokens, begin)) {
        return (1);
    }

    return (0);
}

int Config::conf(
    const std::vector<Token>& tokens,
    std::vector<Token>::const_iterator& it )
{
    while (it < tokens.end()) {
        if (server(tokens, it)) {
            return (1);
        }
    }

    return (0);
}

int Config::server(
    const std::vector<Token>& tokens,
    std::vector<Token>::const_iterator& it )
{
    // NOTE: possibly segv
    // now: it->str == "server"
    ++it;
    // now: it->str == "{"
    ++it;
    serversInfo.push_back(ServerInfo());
    while (it->str != "}") {
        if (server_conf(tokens, it)) {
            return (1);
        }
    }

    // now: it->str == "}"
    ++it;

    return (0);
}

int Config::server_conf(
    const std::vector<Token>& tokens,
    std::vector<Token>::const_iterator& it )
{
    const std::string item = it->str;
    int ret = 0;

    ++it;
    // now: it->str == <value>
    if (item == "listen") {
        ret = listen_conf(tokens, it);
    } else if (item == "server_name") {
        serversInfo.rbegin()->server_name = it->str;
        ++it;
        // now: it->str == ";"
        ++it;
    } else if (item == "client_max_body_size") {
        serversInfo.rbegin()->client_max_body_size = stoul(it->str);
        ++it;
        // now: it->str == ";"
        ++it;
    } else if (item == "error_page") {
        ret = error_page_conf(tokens, it);
    } else if (item == "location") {
        // NOTE: possibly segv
        std::string location_path = it->str;
        serversInfo.rbegin()->locations_info_map[location_path] = LocationInfo(); 
        ++it;
        // now: "it->str == {"
        ++it;
        while (it->str != "}") {
            if (location_conf(tokens, it, location_path)) {
                return (1);
            }
        }
        // now: it->str == "}"
        ++it;
    } else {
        ret = 1;
    }

    return (0);
}

// 127.0.0.1;
// 80;
// 127.0.0.1:80;
int Config::listen_conf(
    const std::vector<Token>& tokens,
    std::vector<Token>::const_iterator& it )
{
    if (it == tokens.end()) {
        return (1);
    }

    // assuming addr:port
    std::string::size_type colon_pos = it->str.find(":");
    serversInfo.rbegin()->listen.addr = it->str.substr(0, colon_pos);
    serversInfo.rbegin()->listen.port = std::stoi(it->str.substr(colon_pos + 1));

    // now: it->str == addr:port
    ++it;
    // now: it->str == ";"
    ++it;
    return (0);
}

int Config::error_page_conf(
    const std::vector<Token>& tokens,
    std::vector<Token>::const_iterator& it )
{
    std::vector<Token>::const_iterator err_status = it;
    std::string error_page_html;

    while ((it + 1)->str != ";") {
        ++it;
    }
    error_page_html = it->str;
    while (err_status < it) {
        serversInfo.rbegin()->error_pages_map[stoi(err_status->str)] = error_page_html;
        ++err_status;
    }
    // now: it->str == "/40x.html"
    ++it;
    // now: it->str == ";"
    ++it;
    return (0);
}

// location_path????
int Config::location_conf(
    const std::vector<Token>& tokens,
    std::vector<Token>::const_iterator& it,
    const std::string& location_path )
{
    const std::string item = it->str;
    LocationInfo& info = serversInfo.rbegin()->locations_info_map[location_path];

    ++it;
    // now: it->str == <value>
    if (item == "root") {
        info.root = it->str;
        ++it;
    } else if (item == "allow_methods") {
        while (it->str != ";") {
            info.allow_methods.push_back(it->str);
            ++it;
        }
    } else if (item == "return") {
        info.return_path = it->str;
        ++it;
    } else if (item == "autoindex") {
        info.autoindex = (it->str == "on") ? true : false;
        ++it;
    } else if (item == "index") {
        info.index = it->str;
        ++it;
    } else if (item == "allow_file_upload") {
        info.allow_file_upload = (it->str == "on") ? true : false;
        ++it;
    } else if (item == "save_folder") {
        info.save_folder = it->str;
        ++it;
    } else if (item == "allow_cgi_extensions") {
        while (it->str != ";") {
            info.allow_cgi_extensions.push_back(it->str);
            ++it;
        }
    } else {
        return (1);
    }
    // now: it->str == ";"
    ++it;
    return (0);
}
