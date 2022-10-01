#ifndef HTTP_METHOD_GET_HPP
#define HTTP_METHOD_GET_HPP

#include "HttpMethod.hpp"
#include "../utils/utils.hpp"

class GET : public HttpMethod {
    public:
        HttpResponse execute( const HttpRequest& req );
};

#endif
