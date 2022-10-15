#ifndef INVALID_METHOD_HPP
#define INVALID_METHOD_HPP

#include "HttpMethod.hpp"

class InvalidMethod : public HttpMethod {
    public:
        HttpResponse execute( const HttpRequest& req );
};

#endif
