#include "GET.hpp"

HttpResponse GET::execute( const HttpRequest& req ) {
    const ServerInfo host_info = req.getHostInfo();

    if (!isMethodAllowed("GET", req.getLocationInfo())) {
        return HttpResponse::createErrorResponse(
            HttpResponse::Status::METHOD_NOT_ALLOWED,
            host_info
        );
    }

    std::string content;
    if (loadFile(req.getPath(), content)) {
        return HttpResponse::createErrorResponse(
            HttpResponse::Status::BAD_REQUEST,
            host_info
        );
    }

    HttpResponse resp;
    resp.setBody(content);
    // TODO: set accurate header
    resp.addHeader("Content-Length", std::to_string(content.size()));
    resp.addHeader("Connection", "close");
    resp.addHeader("Content-Type", "text/html");
    std::cout << resp.marshal() << std::endl;

    return resp;
}
