#include "RecieveRequestEvent.hpp"
#include "../io/HttpResponse.hpp"
#include "../methods/HttpMethod.hpp"
#include <unistd.h>

RecieveRequestEvent::RecieveRequestEvent( int& client_sd )
: client_sd_(client_sd) {}

int RecieveRequestEvent::handler( void ) {
    // get request string
    // NOTE: possibly segv
    char buf[1024];
    recv(client_sd_, buf, 1023, 0);
    std::string raw_req = buf;

    /*
    // parse request
    HttpRequest req;
    if (req.parse(raw_req)) {
        return (1);
    }

    // send response
    // NOTE: creating "SendResponseEvent" class is better
    HttpResponse resp = req.getMethod()->execute(req);
    std::string resp_str = resp.marshal();
    send(client_sd_, resp_str.c_str(), resp_str.size(), 0);
    */
    // for debug(echo back)
    send(client_sd_, raw_req.c_str(), raw_req.size(), 0);

    close(client_sd_);
    client_sd_ = CLOSED_FD;
    return (0);
}
