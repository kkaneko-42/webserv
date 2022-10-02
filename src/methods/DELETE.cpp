#include "DELETE.hpp"
#include <unistd.h>
#include <errno.h>

HttpResponse DELETE::execute( const HttpRequest& req ) {
    const ServerInfo host_info = req.getHostInfo();

    if (!isMethodAllowed("DELETE", req.getLocationInfo())) {
        return HttpResponse::createErrorResponse(
            HttpResponse::Status::METHOD_NOT_ALLOWED,
            host_info
        );
    }

    if (unlink(req.getPath().c_str()) < 0) {
        // TODO: support multiple errno
        return HttpResponse::createErrorResponse(
            HttpResponse::Status::NOT_FOUND,
            host_info
        );
    }

    return (HttpResponse());
}
