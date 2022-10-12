#include "POST.hpp"
#include "../cgi/CgiExecutor.hpp"
#include <fstream>
#include <ios>
#include <unistd.h>

HttpResponse POST::execute( const HttpRequest& req ) {
    const ServerInfo host_info = req.getHostInfo();
    const LocationInfo location = req.getLocationInfo();

    if (!isMethodAllowed("POST", location)) {
        return HttpResponse::createErrorResponse(
            HttpResponse::METHOD_NOT_ALLOWED,
            host_info
        );
    }

    CgiExecutor cgi;
    cgi.init(req);
    if (cgi.isExecutable()) {
        return cgi.execute(req);
    }

    if (!location.allow_file_upload) {
        return HttpResponse::createErrorResponse(
            HttpResponse::FORBIDDEN,
            host_info
        );
    }

    HttpResponse::Status status;
    if (access(req.getUploadPath().c_str(), F_OK) == 0) {
        status = HttpResponse::OK;
    } else {
        status = HttpResponse::CREATED;
    }

    std::ofstream ofs(req.getUploadPath().c_str(), std::ios_base::app);
    if (!ofs) {
        perror("ofs");
        return HttpResponse::createErrorResponse(
            HttpResponse::NOT_FOUND,
            host_info
        );
    }
    ofs << req.getBody();

    HttpResponse resp;
    resp.setStatus(status);

    return (resp);
}
