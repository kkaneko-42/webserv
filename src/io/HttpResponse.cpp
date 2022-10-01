#include "HttpResponse.hpp"

void HttpResponse::addHeader( const std::string& key, const std::string& value ) {
    headers_.push_back(std::pair(key, value));
}

std::string HttpResponse::marshal( void ) const {
    std::string str = "";

    str += version_ + " " + status_ + " " + statusMsg(status_) + "\r\n";
    for (size_t i = 0; i < headers_.size(); ++i) {
        std::string key = headers_[i].first;
        std::string value = headers_[i].second;

        str += key + ": " + value + "\r\n";
    }
    str += "\r\n";
    str += body_;

    return str;
}

std::string HttpRequest::statusMsg( Status status ) const {
    std::string msg;

    switch (status)
    {
        case OK:
            msg = "OK";
            break;
        case CREATED:
            msg = "Created";
            break;
        case BAD_REQUEST:
            msg = "Bad Request";
            break;
        case INTERNAL_SERVER_ERROR:
            msg = "Internal Server Error";
            break;
        
        default:
            msg = "Invalid Status";
            break;
    }

    return (msg);
}
