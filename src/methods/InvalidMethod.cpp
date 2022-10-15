#include "InvalidMethod.hpp"

HttpResponse InvalidMethod::execute( const HttpRequest& req ) {
    return (
        HttpResponse::createErrorResponse(
            HttpResponse::NOT_IMPLEMENTED,
            req.getHostInfo()
        )
    );
}
