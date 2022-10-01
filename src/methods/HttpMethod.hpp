#ifndef HTTP_METHOD_HPP
#define HTTP_METHOD_HPP

#include "../HttpResponse.hpp"
#include "../HttpRequest.hpp"

class HttpMethod {
    public:
        virtual ~HttpMethod( void ) {}
        virtual HttpResponse execute( const HttpRequest& req ) = 0;
};

#endif
