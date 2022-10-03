#include "HttpResponse.hpp"
#include "HttpRequest.hpp"
#include "../utils/utils.hpp"
#include "../conf/Config.hpp"

HttpResponse::HttpResponse( void )
: version_("1.1"), status_(OK),
headers_(std::vector< std::pair< std::string, std::string> >()),
body_("") {}

void HttpResponse::addHeader( const std::string& key, const std::string& value ) {
    headers_.push_back(std::make_pair(key, value));
}

std::string HttpResponse::marshal( void ) const {
    std::string str = "";

    str += "HTTP/" + version_ + " " + std::to_string(status_) + " " + statusMsg(status_) + "\r\n";
    for (size_t i = 0; i < headers_.size(); ++i) {
        std::string key = headers_[i].first;
        std::string value = headers_[i].second;

        str += key + ": " + value + "\r\n";
    }
    str += "\r\n";
    str += body_;

    return str;
}

std::string HttpResponse::statusMsg( Status status ) {
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
        case NOT_FOUND:
            msg = "Not Found";
            break;
        case METHOD_NOT_ALLOWED:
            msg = "Method Not Allowed";
            break;
        case PAYLOAD_TOO_LARGE:
            msg = "Payload Too Large";
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

HttpResponse HttpResponse::createErrorResponse( Status status, const ServerInfo& host_info ) {
    HttpResponse resp;

    if (host_info.error_pages_map.count(status) == 0) {
        return (HttpResponse::createErrorResponse(status));
    }

    const std::string page_path = host_info.error_pages_map.at(status);
    std::string page_content;

    if (loadFile(page_path, page_content)) {
        return (HttpResponse::createErrorResponse(status));
    }

    resp.setStatus(status);
    resp.setBody(page_content);
    resp.addHeader("Content-Length", std::to_string(page_content.size()));

    return (resp);
}

HttpResponse HttpResponse::createErrorResponse( Status status ) {
    const std::string default_page_content =
    "<!DOCTYPE HTML>\r\n"
    "<html><head>\r\n"
    "<title>An error occurred.</title>\r\n"
    "</head><body>\r\n"
    "<h1>" + HttpResponse::statusMsg(status) + "</h1>\r\n"
    "<p>The requested URL was not found on this server.</p>\r\n"
    "</body></html>\r\n";

    HttpResponse resp;
    resp.setStatus(status);
    resp.addHeader("Content-Length", std::to_string(default_page_content.size()));
    resp.setBody(default_page_content);

    return (resp);
}

HttpResponse HttpResponse::createDirListingResponse( const HttpRequest& req ) {
    std::vector<std::string> file_list = getFileList(req.getPath());
    std::string res_body;

    res_body += "<!DOCTYPE HTML>\r\n";
    res_body += "<html><head>\r\n";
    res_body += "<h1>Indexof " + req.getRawPath() + "</h1>\r\n";
    res_body += "</head><hr><body>\r\n";

    for (size_t i = 0; i < file_list.size(); ++i) {
        res_body += "<p>" + file_list[i] + "</p>\r\n";
    }
    res_body += "</body></html>\r\n";

    HttpResponse resp;
    resp.setBody(res_body);
    resp.addHeader("Content-Length", std::to_string(res_body.size()));

    return (resp);
}
