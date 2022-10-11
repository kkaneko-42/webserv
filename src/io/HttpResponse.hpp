#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <utility>
#include <vector>
#include <string>

struct ServerInfo;
struct LocationInfo;
class HttpRequest;

class HttpResponse {
    public:
        enum Status {
            OK = 200,
            CREATED,

            SEE_OTHER = 303,

            BAD_REQUEST = 400,
            FORBIDDEN = 403,
            NOT_FOUND = 404,
            METHOD_NOT_ALLOWED,
            PAYLOAD_TOO_LARGE = 413,

            INTERNAL_SERVER_ERROR = 500,
        };

        HttpResponse( void );

        void setVersion( const std::string& version ) { version_ = version; }
        void setStatus( Status status ) { status_ = status; }
        void addHeader( const std::string& key, const std::string& value );
        void setBody( const std::string& body ) { body_ = body; }

        std::string marshal( void ) const;
        static HttpResponse unmarshal( const std::string& str );
    
        static std::string statusMsg( Status status );
        static HttpResponse createErrorResponse( Status status, const ServerInfo& host_info );
        static HttpResponse createErrorResponse( Status status );
        static HttpResponse createDirListingResponse( const HttpRequest& req );
        static HttpResponse createRedirectionResponse( Status status, const std::string& redirect_path );

    private:
        std::string version_;
        Status status_;
        std::vector< std::pair<std::string, std::string> > headers_;
        std::string body_;
};

#endif
