#include "ConfigInfo.hpp"
#include <iostream>

static std::string indent(int indent_level) {
    return std::string(indent_level * INDENT_WIDTH, ' ');
}

void debugLocationInfo(const LocationInfo &info, int indent_level) {
    // alias
    std::cout << indent(indent_level) << "alias: " << info.alias << std::endl;
    // allow_methods
    if (info.allow_methods.size() > 0) {
        std::cout << indent(indent_level) << "allow_methods: ";
        std::set<std::string>::const_iterator it;
        for (it = info.allow_methods.begin();
             it != info.allow_methods.end(); it++) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }
    // return
    if (info.return_path != "") {
        std::cout << indent(indent_level) << "return: " << info.return_path
                  << std::endl;
    }
    // autoindex
    std::cout << indent(indent_level)
              << "autoindex: " << (info.autoindex ? "on" : "off") << std::endl;
    // index
    std::cout << indent(indent_level) << "index: " << info.index << std::endl;
    // allow_file_upload
    std::cout << indent(indent_level) << "allow_file_upload: "
              << (info.allow_file_upload ? "on" : "off") << std::endl;
    // save_folder
    if (info.save_folder != "") {
        std::cout << indent(indent_level) << "save_folder: " << info.save_folder
                  << std::endl;
    }
    // allow_cgi_extensions
    if (info.allow_cgi_extensions.size() > 0) {
        std::cout << indent(indent_level) << "allow_methods: ";
        std::vector<std::string>::const_iterator it;
        for (it = info.allow_cgi_extensions.begin();
             it != info.allow_cgi_extensions.end(); it++) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }
}

void debugServerInfo(const ServerInfo &info, int indent_level) {
    std::cout << indent(indent_level) << "server: {" << std::endl;

    // listen
    std::cout << indent(indent_level + 1) << "listen: " << info.listen.ip << ":"
              << info.listen.port << std::endl;
    // server_name
    std::cout << indent(indent_level + 1) << "server_name: " << info.server_name
              << std::endl;
    // client_max_body_size
    std::cout << indent(indent_level + 1)
              << "client_max_body_size: " << info.client_max_body_size
              << std::endl;
    // error_page
    if (info.error_pages_map.size() > 0) {
        std::cout << indent(indent_level + 1) << "error_page: {" << std::endl;
        std::map<int, std::string>::const_iterator it;
        for (it = info.error_pages_map.begin(); it != info.error_pages_map.end();
             it++) {
            std::cout << indent(indent_level + 2) << (*it).first << ": "
                      << (*it).second << std::endl;
        }
        std::cout << indent(indent_level + 1) << "}" << std::endl;
    }
    // location
    if (info.locations_info_map.size() > 0) {
        std::map<std::string, LocationInfo>::const_iterator it;
        for (it = info.locations_info_map.begin(); it != info.locations_info_map.end();
             it++) {
            std::string path = (*it).first;
            LocationInfo location_info = (*it).second;
            std::cout << indent(indent_level + 1) << "location " << path << " {"
                      << std::endl;

            debugLocationInfo(location_info, indent_level + 2);

            std::cout << indent(indent_level + 1) << "}" << std::endl;
        }
    }
    std::cout << indent(indent_level) << "}" << std::endl;
}

void debugConfigInfo(const ConfigInfo &info, int indent_level) {
    // std::cout << indent(indent_level) << "config: {" << std::endl;

    for (int i = 0; i < info.servers_info.size(); i++) {
        debugServerInfo(info.servers_info[i], indent_level);
    }

    // std::cout << indent(indent_level) << "}" << std::endl;
}
