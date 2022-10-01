#include "HttpRequest.hpp"
#include <iostream>

static std::vector<std::string> splitString(const std::string &str, const std::string& delim);

template <typename T>
static void printVector(const std::vector<T>& v) {
    std::cout << "[";
    for (size_t i = 0; i < v.size(); i++) {
        std::cout << v[i] << ", ";
    }
    std::cout << "]" << std::endl;
}

HttpRequest::HttpRequest( const std::string& request )
: method_(""), path_(""), version_(""), headers_(Headers()), body_("")
{
    std::cout << "=== req ===" << std::endl;
    std::cout << request;
    std::cout << "===========" << std::endl;
    if (parse(request)) {
        throw std::logic_error("parse error");
    }
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

    return (0);
}

int HttpRequest::parseTop( const std::string& top_row ) {
    std::vector<std::string> tokens = splitString(top_row, " ");

    if (tokens.size() != 3) {
        return (1);
    }

    method_ = tokens[0];
    path_ = tokens[1];
    version_ = tokens[2];
    
    return (0);
}

int HttpRequest::parseHeader( const std::vector<std::string>& headers ) {
    for (size_t i = 0; i < headers.size(); ++i) {
        std::string::size_type colon_pos = headers[i].find(":");
        std::string key = headers[i].substr(0, colon_pos);
        std::string value = headers[i].substr(colon_pos + 1);

        if (key == "Accept") {
            headers_.accept = splitString(value, ", ");
        } else if (key == "Accept-Language") {
            headers_.accept_language = value;
        } else if (key == "Accept-Encoding") {
            headers_.accept_encoding = splitString(value, ", ");
        } else if (key == "User-Agent") {
            headers_.user_agent = value;
        } else if (key == "Host") {
            headers_.host = value;
        } else if (key == "Connection") {
            headers_.connection = value;
        } else {
            return (1);
        }
    }

    return (0);
}

void HttpRequest::printRequest( void ) const {
    std::cout << "method: " << method_ << std::endl;
    std::cout << "path: " << path_ << std::endl;
    std::cout << "version: " << version_ << std::endl;
    std::cout << "accept: ";
    printVector(headers_.accept);
    std::cout << "accept-lang: " << headers_.accept_language << std::endl;
    std::cout << "user-agent: " << headers_.user_agent << std::endl;
    std::cout << "host: " << headers_.host << std::endl;
    std::cout << "connection: " << headers_.connection << std::endl;
    std::cout << "==== body ===" << std::endl;
    std::cout << body_;
}

static std::vector<std::string> splitString(const std::string &str, const std::string& delim) {
    std::vector<std::string> vec;

    std::string::size_type prev = 0;
    while (true) {
        std::string::size_type pos = str.find(delim, prev);
        if (pos == std::string::npos)
            break;
        std::string sub = str.substr(prev, pos - prev);
        vec.push_back(sub);
        prev = pos + delim.size();
    }
    vec.push_back(str.substr(prev));
    return vec;
}
