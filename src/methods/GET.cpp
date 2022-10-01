#include "GET.hpp"

HttpResponse GET::execute( const HttpRequest& req ) {
    HttpResponse resp;
    // TODO: exit if file not found, or occur an error
    std::string content = loadFile(req.getPath());

    resp.setBody(content);
    // TODO: set accurate header
    resp.addHeader("Content-Length", std::to_string(content.size()));
    resp.addHeader("Connection", "close");
    // resp.addHeader("Content-Type", "text/html");

    return resp;
}
