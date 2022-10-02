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

    // もしrequestをcacheする場合、以下はSendResponseEvent->hander内に記述
    HttpResponse resp;
    /* create response */
    if (req.hostMatching(server_.getConf().getServersInfo())) { // host matching
        resp = HttpResponse::createErrorResponse(HttpResponse::Status::NOT_FOUND);
    } else if (req.locationMatching()) { // location matching
        resp = HttpResponse::createErrorResponse(HttpResponse::Status::NOT_FOUND, req.getHostInfo());
    } else {
        resp = req.getMethod()->execute(req);
    }

    /* cache response */
    server_.cacheResponse(client_sd_, resp);
    std::cout << "==== RESPONSE ====" << std::endl;
    std::cout << resp.marshal() << std::endl;
    std::cout << "==================" << std::endl;

    return (0);
}
