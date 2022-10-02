#include "POST.hpp"
#include <fstream>
#include <ios>
#include <unistd.h>

HttpResponse POST::execute( const HttpRequest& req ) {
    const ServerInfo host_info = req.getHostInfo();
    const LocationInfo location = req.getLocationInfo();

    if (!isMethodAllowed("POST", location) || !location.allow_file_upload) {
        return HttpResponse::createErrorResponse(
            HttpResponse::Status::METHOD_NOT_ALLOWED,
            host_info
        );
    }

    HttpResponse::Status status;
    if (access(req.getUploadPath().c_str(), F_OK) == 0) {
        status = HttpResponse::Status::OK;
    } else {
        status = HttpResponse::Status::CREATED;
    }

    std::ofstream ofs(req.getUploadPath(), std::ios_base::app);
    if (!ofs) {
        perror("ofs");
        return HttpResponse::createErrorResponse(
            HttpResponse::Status::BAD_REQUEST,
            host_info
        );
    }
    ofs << req.getBody();

    HttpResponse resp;
    resp.setStatus(status);

    return (resp);
}
