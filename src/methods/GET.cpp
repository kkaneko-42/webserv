#include "GET.hpp"

HttpResponse GET::execute( const HttpRequest& req ) {
    HttpResponse resp;
    // TODO: exit if file not found, or occur an error
    std::string content = loadFile(req.getPath());

    resp.setBody(content);
    resp.addHeader("Content-Length", std::to_string(content.size()));
    // TODO: select connection header
    resp.addHeader("Connection", "close");
}
