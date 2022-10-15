#include "DELETE.hpp"
#include <unistd.h>
#include <errno.h>

HttpResponse DELETE::execute( const HttpRequest& req ) {
    const ServerInfo host_info = req.getHostInfo();

    if (!isMethodAllowed("DELETE", req.getLocationInfo())) {
        return HttpResponse::createErrorResponse(
            HttpResponse::METHOD_NOT_ALLOWED,
            host_info
        );
    }

    if (unlink(req.getResolvedPath().c_str()) < 0) {
        return HttpResponse::createErrorResponse(
            HttpResponse::NOT_FOUND,
            host_info
        );
    }

    return (HttpResponse(HttpResponse::NO_CONTENT));
}
