#include "RecieveRequestEvent.hpp"
#include "../io/HttpResponse.hpp"
#include "../methods/HttpMethod.hpp"
#include <unistd.h>
#include "../Server.hpp"

RecieveRequestEvent::RecieveRequestEvent( int client_sd, Server& server )
: client_sd_(client_sd), server_(server) {}

int RecieveRequestEvent::handler( void ) {
    // get request string
    // NOTE: possibly segv
    char buf[1024];
    int ret = recv(client_sd_, buf, 1023, 0);
    if (ret < 0) {
        perror("recv");
        return (1);
    }
    buf[ret] = '\0';
    std::string raw_req = buf;

    // parse request
    HttpRequest req;
    if (req.parse(raw_req)) {
        return (1);
    }

    // cache response
    HttpResponse resp = req.getMethod()->execute(req);
    server_.cacheResponse(client_sd_, resp);

    return (0);
}
