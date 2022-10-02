#ifndef HTTP_METHOD_DELETE_HPP
#define HTTP_METHOD_DELETE_HPP

#include "HttpMethod.hpp"
#include "../utils/utils.hpp"

class DELETE : public HttpMethod {
    public:
        HttpResponse execute( const HttpRequest& req );
};

#endif