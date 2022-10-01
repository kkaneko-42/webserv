#include "RecieveRequestEvent.hpp"

RecieveRequestEvent::RecieveRequestEvent( int client_sd )
: client_sd_(client_sd) {}

int RecieveRequestEvent::handler( void ) {
    HttpRequest req;
    
    // get request string
    char buf[1024];
    recv(client_sd_, buf, 1023, 0);
    std::string raw_req = buf;

    // parse request
    if (req.parse(raw_req)) {
        return (1);
    }

    // send response
    // NOTE: creating "SendResponseEvent" class is better
    HttpResponse resp = req.getMethod()->execute(req);

    return (0);
}
