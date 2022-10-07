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

    str += "HTTP/" + version_ + " " + sizeToString(status_) + " " + statusMsg(status_) + "\r\n";
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
        case SEE_OTHER:
            msg = "See Other";
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
    resp.addHeader("Content-Length", sizeToString(page_content.size()));

    return (resp);
}

HttpResponse HttpResponse::createErrorResponse( Status status ) {
    const std::string default_page_content =
    "<!DOCTYPE HTML>\r\n"
    "<html><head>\r\n"
    "<title>An error occurred.</title>\r\n"
    "</head><body>\r\n"
    "<h1>" + statusMsg(status) + "</h1>\r\n"
    "<p>The requested URL was not found on this server.</p>\r\n"
    "</body></html>\r\n";

    HttpResponse resp;
    resp.setStatus(status);
    resp.addHeader("Content-Length", sizeToString(default_page_content.size()));
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
        res_body += "<p><a href=\"./" + file_list[i] + "\">" + file_list[i] + "</a></p>\r\n";
    }
    res_body += "</body></html>\r\n";

    HttpResponse resp;
    resp.setBody(res_body);
    resp.addHeader("Content-Length", sizeToString(res_body.size()));

    return (resp);
}

HttpResponse HttpResponse::createRedirectionResponse(
    Status status,
    const std::string& redirect_path )
{
    HttpResponse resp;

    resp.setStatus(status);
    resp.addHeader("Location", redirect_path);

    return (resp);
}

HttpResponse HttpResponse::unmarshal( const std::string& str ) {
    const std::string header_body_delim = "\r\n\r\n";
    HttpResponse resp;
    std::vector<std::string> header_body = splitString(str, header_body_delim);

    if (header_body.size() != 2) {
        return createErrorResponse(NOT_FOUND);
    }

    // header
    std::vector<std::string> headers = splitString(header_body[0], "\r\n");
    for (size_t i = 0; i < headers.size(); ++i) {
        std::string key_value_delim = ":";
        std::vector<std::string> key_value = splitString(headers[i], key_value_delim);
        if (key_value.size() != 2) {
            return createErrorResponse(NOT_FOUND);
        }

        if (toLowerString(key_value[0]) == "status") {
            resp.setStatus(Status(atoi(key_value[1].c_str())));
        } else {
            // NOTE: key_value[1] == "" case
            resp.addHeader(toLowerString(key_value[0]), trimString(key_value[1]));
        }
    }

    // body
    resp.setBody(header_body[1]);

    return (resp);
}
