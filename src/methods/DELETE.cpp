#include "DELETE.hpp"
#include <unistd.h>
#include <errno.h>

HttpResponse DELETE::execute( const HttpRequest& req ) {
    HttpResponse resp;

    if (unlink(req.getPath().c_str()) < 0) {
        // create error response
    }

    return (resp);
}
