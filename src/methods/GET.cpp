#include "GET.hpp"

HttpResponse GET::execute( const HttpRequest& req ) {

    // get virtual host info from "Host" header.
    // location matching
    // append root path

    HttpResponse resp;
    // TODO: exit if file not found, or occur an error
    std::string content;
    if (loadFile(req.getPath(), content)) {
        return resp;
    }

    resp.setBody(content);
    // TODO: set accurate header
    resp.addHeader("Content-Length", std::to_string(content.size()));
    resp.addHeader("Connection", "close");
    // resp.addHeader("Content-Type", "text/html");
    std::cout << resp.marshal() << std::endl;

    return resp;
}
