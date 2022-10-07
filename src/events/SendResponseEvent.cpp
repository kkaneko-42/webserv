#include "SendResponseEvent.hpp"
#include "../Server.hpp"
#include <unistd.h>

SendResponseEvent::SendResponseEvent( int& client_sd, Server& server )
: client_sd_(client_sd), server_(server) {}

int SendResponseEvent::handler( void ) {
    HttpResponse resp = server_.getCachedResponse(client_sd_);
    std::string resp_str = resp.marshal();

    send(client_sd_, resp_str.c_str(), resp_str.size(), 0);

    server_.removeCachedResponse(client_sd_);
    if (close(client_sd_) < 0) {
        perror("close");
    }
    client_sd_ = CLOSED_FD;

    return (0);
}
