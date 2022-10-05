#include "GET.hpp"
#include "../utils/utils.hpp"
#include "../cgi/CgiExecutor.hpp"

HttpResponse GET::execute( const HttpRequest& req ) {
    const ServerInfo host_info = req.getHostInfo();
    const LocationInfo location = req.getLocationInfo();
    std::string path = req.getPath();

    if (!isMethodAllowed("GET", location)) {
        return HttpResponse::createErrorResponse(
            HttpResponse::Status::METHOD_NOT_ALLOWED,
            host_info
        );
    }

    if (location.return_path != "") {
        return HttpResponse::createRedirectionResponse(
            HttpResponse::Status::SEE_OTHER,
            location.return_path
        );
    }

    if (pathIsDir(path)) {
        if (location.index != "") {
            path += location.index;
        } else if (location.autoindex) {
            return HttpResponse::createDirListingResponse(req);
        }
    }

    CgiExecutor cgi;
    cgi.init(req);
    if (cgi.isExecutable()) {
        return cgi.execute(req);
    }

    std::string content;
    if (loadFile(path, content)) {
        return HttpResponse::createErrorResponse(
            HttpResponse::Status::NOT_FOUND,
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
