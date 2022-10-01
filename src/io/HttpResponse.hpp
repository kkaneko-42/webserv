#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <utility>
#include <vector>
#include <string>

class HttpResponse {
    public:
        enum Status {
            OK = 200,
            CREATED,

            BAD_REQUEST = 400,

            INTERNAL_SERVER_ERROR = 500,
        };

        HttpResponse( void );

        void setVersion( const std::string& version ) { version_ = version; }
        void setStatus( Status status ) { status_ = status; }
        void addHeader( const std::string& key, const std::string& value );
        void setBody( const std::string& body ) { body_ = body; }
        
        std::string marshal( void ) const;
        std::string statusMsg( Status status ) const;

    private:
        std::string version_;
        Status status_;
        std::vector< std::pair<std::string, std::string> > headers_;
        std::string body_;
};

#endif
