#ifndef HTTP_METHOD_HPP
#define HTTP_METHOD_HPP

#include "../io/HttpResponse.hpp"
#include "../io/HttpRequest.hpp"

class HttpMethod {
    public:
        virtual ~HttpMethod( void ) {}
        virtual HttpResponse execute( const HttpRequest& req ) = 0;
};

HttpMethod* generateHttpMethod( const std::string& name );

#endif
