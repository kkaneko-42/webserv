#include "HttpRequest.hpp"
#include <iostream>
#include "../utils/utils.hpp"
#include "../methods/HttpMethod.hpp"
#include <cassert>
#include "ChunkParser.hpp"
#include <typeinfo>

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
    if (empty_line_pos == std::string::npos) {
        return (1);
    }

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

    // method
    method_ = generateHttpMethod(tokens[0]);

    // path
    std::string::size_type pos = tokens[1].find("?");
    if (pos == std::string::npos) {
        path_ = tokens[1];
    } else {
        path_ = tokens[1].substr(0, pos);
        query_string_ = tokens[1].substr(pos + 1);
    }

    // version
    version_ = tokens[2];

    return (0);
}
// foo, , bar
int HttpRequest::parseHeader( const std::vector<std::string>& headers ) {
    for (size_t i = 0; i < headers.size(); ++i) {
        if (validateHeaderLine(headers[i])) {
            return (1);
        }

        std::string::size_type delim_pos = headers[i].find(":");
        std::string key = toLowerString(headers[i].substr(0, delim_pos));
        std::string value = toLowerString(trimString(headers[i].substr(delim_pos + 1)));

        if (key == "accept") {
            headers_.accept = trimStringIter(splitString(value, ","));
        } else if (key == "accept-language") {
            headers_.accept_language = value;
        } else if (key == "accept-encoding") {
            headers_.accept_encoding = trimStringIter(splitString(value, ","));
        } else if (key == "user-agent") {
            headers_.user_agent = value;
        } else if (key == "host") {
            headers_.host = value;
        } else if (key == "connection") {
            headers_.connection = value;
        } else if (key == "transfer-encoding") {
            std::set<std::string> st;
            std::vector<std::string> splitedValue = trimStringIter(splitString(value, ","));
            for (size_t i = 0; i < splitedValue.size(); ++i) {
                st.insert(toLowerString(splitedValue[i]));
            }
            headers_.transfer_encoding = st;
        }
    }

    return (0);
}

int HttpRequest::validateHeaderLine( const std::string& header_line ) const {
    std::string::size_type delim_pos = header_line.find(":");
    if (delim_pos == std::string::npos) {
        return (1);
    }

    std::string key = toLowerString(header_line.substr(0, delim_pos));
    if (validateHeaderFieldName(key)) {
        return (1);
    }

    std::string value = toLowerString(trimString(header_line.substr(delim_pos + 1)));
    if (validateHeaderFieldContent(value)) {
        return (1);
    }

    return (0);
}

int HttpRequest::validateHeaderFieldName( const std::string& name ) const {
    const std::string charset = "!#$%&'*+-.^_`|~";

    if (hasSpace(name)) {
        return (1);
    }

    for (std::string::size_type i = 0; i < name.size(); ++i) {
        if (!isalnum(name[i]) && (charset.find(name[i]) == std::string::npos)) {
            return 1;
        }
    }

    return 0;
}

int HttpRequest::validateHeaderFieldContent( const std::string& content ) const {
    const std::string charset = "\x09";

    for (std::string::size_type i = 0; i < content.size(); ++i) {
        if (!isprint(content[i]) && (charset.find(content[i]) == std::string::npos)) {
            return 1;
        }
    }

    return 0;
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

std::string HttpRequest::getResolvedPath( void ) const {
    assert(path_.size() >= location_info_.location_path.size());
    std::string full_path = path_.substr(location_info_.location_path.size());
    const std::string alias = location_info_.alias;
    return alias + full_path;
}

std::string HttpRequest::getUploadPath( void ) const {
    std::string filename = path_.substr(location_info_.location_path.size());
    
    if (location_info_.save_folder == "") {
        return (location_info_.alias + filename);
    }
    return (location_info_.save_folder + filename);
}

int HttpRequest::hostMatching( 
    const std::vector<ServerInfo>& servers_info,
    std::pair< std::string, int > listen_addr
    )
{
    std::cout << "====== hostMatching ======" << std::endl;    

    // Host: xxxxxxxxxxxx
    const std::string hostname = this->getHostName();

    std::string ip = listen_addr.first;
    int port = listen_addr.second;

    std::cout << "host: " << hostname << std::endl;
    std::cout << "listen.ip: " << ip << std::endl;
    std::cout << "listen.port: " << port << std::endl;

    // server_name matching
    std::cout << "@@@ server_name matching @@@" << std::endl;    
    for (size_t i = 0; i < servers_info.size(); ++i) {
        ServerInfo info = servers_info[i];

        std::cout << "i=" << i << std::endl;
        std::cout << "info.ip: " << info.listen.ip << std::endl;
        std::cout << "info.port: " << info.listen.port << std::endl;

        if (ip != info.listen.ip || port != info.listen.port) {
            continue;
        }
        if (info.server_name == hostname) {
            host_info_ = info;
            return 0;
        }
    }

    // default server matching
    std::cout << "@@@ default server matching @@@" << std::endl;    
    for (size_t i = 0; i < servers_info.size(); ++i) {
        ServerInfo info = servers_info[i];

        std::cout << "i=" << i << std::endl;
        std::cout << "info.ip: " << info.listen.ip << std::endl;
        std::cout << "info.port: " << info.listen.port << std::endl;

        if (ip == info.listen.ip && port == info.listen.port) {
            host_info_ = info;
            return 0;
        }
    }

    std::cout << "errrrrrrrrrrrrrrrrrrrrrrrr" << std::endl;

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

    (void)flag_resolve;
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
