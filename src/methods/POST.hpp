#ifndef HTTP_METHOD_POST_HPP
#define HTTP_METHOD_POST_HPP

#include "HttpMethod.hpp"
#include "../utils/utils.hpp"

class POST : public HttpMethod {
    public:
        HttpResponse execute( const HttpRequest& req );
};

#endif