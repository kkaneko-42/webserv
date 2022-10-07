#include "HttpRequest.hpp"
#include <iostream>
#include "../utils/utils.hpp"
#include "../methods/HttpMethod.hpp"
#include <cassert>
#include "ChunkParser.hpp"

template <typename T>
static void printVector(const std::vector<T>& v) {
    std::cout << "[";
    for (size_t i = 0; i < v.size(); i++) {
        std::cout << v[i] << ", ";
    }
    std::cout << "]" << std::endl;
}

HttpRequest::HttpRequest( void )
: method_(NULL), path_(""), version_(""),
headers_(Headers()), body_("")
{}

HttpRequest::~HttpRequest( void ) {
    delete method_;
}

int HttpRequest::parse( const std::string& request ) {
    const std::string header_body_delim = "\r\n\r\n";

    std::string::size_type empty_line_pos = request.find(header_body_delim);
    body_ = request.substr(empty_line_pos + header_body_delim.size());

    std::string headers = request.substr(0, empty_line_pos);
    std::vector<std::string> rows = splitString(headers, "\r\n");
    
    if (parseTop(rows[0])) {
        return (1);
    }
    rows.erase(rows.begin());
    if (parseHeader(rows)) {
        return (1);
    }

    std::cout << "request {" << std::endl;
    printRequest();
    std::cout << "}" << std::endl;

    return (0);
}

int HttpRequest::parseTop( const std::string& top_row ) {
    std::vector<std::string> tokens = splitString(top_row, " ");

    if (tokens.size() != 3) {
        return (1);
    }

    method_ = generateHttpMethod(tokens[0]);
    if (method_ == NULL) {
        return (1);
    }
    path_ = tokens[1];
    version_ = tokens[2];
    
    return (0);
}

int HttpRequest::parseHeader( const std::vector<std::string>& headers ) {
    for (size_t i = 0; i < headers.size(); ++i) {
        std::string::size_type delim_pos = headers[i].find(":");
        std::string key = toLowerString(headers[i].substr(0, delim_pos));
        std::string value = toLowerString(trimString(headers[i].substr(delim_pos + 2)));

        if (key == "accept") {
            headers_.accept = splitString(value, ", ");
        } else if (key == "accept-language") {
            headers_.accept_language = value;
        } else if (key == "accept-encoding") {
            headers_.accept_encoding = splitString(value, ", ");
        } else if (key == "user-agent") {
            headers_.user_agent = value;
        } else if (key == "host") {
            headers_.host = value;
        } else if (key == "connection") {
            headers_.connection = value;
        } else if (key == "transfer-encoding") {
            std::set<std::string> st;
            std::vector<std::string> splitedValue = splitString(value, ", ");
            for (size_t i = 0; i < splitedValue.size(); ++i) {
                st.insert(toLowerString(splitedValue[i]));
            }
            headers_.transfer_encoding = st;
        }
    }

    return (0);
}

void HttpRequest::printRequest( void ) const {
    std::cout << "method: " << typeid(*method_).name() << std::endl;
    std::cout << "path: " << path_ << std::endl;
    std::cout << "version: " << version_ << std::endl;
    std::cout << "accept: ";
    printVector(headers_.accept);
    std::cout << "accept-lang: " << headers_.accept_language << std::endl;
    std::cout << "user-agent: " << headers_.user_agent << std::endl;
    std::cout << "host: " << headers_.host << std::endl;
    std::cout << "connection: " << headers_.connection << std::endl;
    
    std::cout << "transfer-encoding: [";
    std::set<std::string>::iterator it;
    for (it = headers_.transfer_encoding.begin(); it != headers_.transfer_encoding.end(); ++it) {
        std::cout << *it << ", ";
    }
    std::cout << "]" << std::endl;

    std::cout << "==== body ===" << std::endl;
    std::cout << body_;
}

std::string HttpRequest::getPath( void ) const {
    assert(path_.size() >= location_info_.location_path.size());
    std::string full_path = path_.substr(location_info_.location_path.size());
    const std::string root = location_info_.root;
    return root + full_path;
}

std::string HttpRequest::getUploadPath( void ) const {
    return (location_info_.save_folder + path_);
}

int HttpRequest::hostMatching( const std::vector<ServerInfo>& servers_info ) {
    // get hostname (127.0.0.1:4242, example.com:4242, etc...)
    const std::string hostname = this->getHostName();
    const std::string hostname_resolved = this->getHostName(true);

    for (size_t i = 0; i < servers_info.size(); ++i) {
        ServerInfo info = servers_info[i];
        const std::string listen_str =
            info.listen.addr + ":" + std::to_string(info.listen.port);

        if ((hostname == listen_str) || (hostname_resolved == listen_str)) {
            host_info_ = info;
            return 0;
        }
    }

    return (1);
}

int HttpRequest::locationMatching( void ) {
    std::string path = path_;
    // path == "/hoge/fuga/index.html"

    while (true) {
        std::string::size_type pos = path.rfind("/");
        if (pos == std::string::npos) {
            return 1;
        }

        path = path.substr(0, pos + 1);
        // path == "/hoge/fuga/"

        if (host_info_.locations_info_map.count(path) != 0) {
            location_info_ = host_info_.locations_info_map[path];
            break;
        }

        path = path.substr(0, pos);
        // path == "/hoge/fuga"
    }

    return 0;
}

std::string HttpRequest::getHostName( bool flag_resolve ) const {
    std::string hostname = headers_.host;

    if (!flag_resolve) {
        return (hostname);
    }

    // resolve "localhost"
    std::string::size_type localhost_pos = hostname.find("localhost");
    if (localhost_pos != std::string::npos) {
        hostname.replace(
            localhost_pos,
            std::string("localhost").size(),
            "127.0.0.1"
        );
    }

    // resolve server_name
    std::string::size_type server_name_pos = hostname.find(host_info_.server_name);
    if (server_name_pos != std::string::npos) {
        hostname.replace(
            server_name_pos,
            host_info_.server_name.size(),
            host_info_.listen.addr
        );
    }

    return (hostname);
}

bool HttpRequest::isChunked( void ) const {
    return (headers_.transfer_encoding.count("chunked") != 0);
}

int HttpRequest::unchunk( void ) {
    if (!this->isChunked()) {
        return 1;
    }

    std::string unchunked;
    try {
        unchunked = ChunkParser::parse(body_);
    } catch (const std::exception& e) {
        return 1;
    }

    body_ = unchunked;
    return 0;
}
