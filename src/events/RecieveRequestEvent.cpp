#include "RecieveRequestEvent.hpp"
#include "../io/HttpResponse.hpp"
#include "../methods/HttpMethod.hpp"
#include <unistd.h>
#include "../Server.hpp"
#define BUF_SIZE 1024

RecieveRequestEvent::RecieveRequestEvent( int client_sd, Server& server )
: client_sd_(client_sd), server_(server) {}

std::string RecieveRequestEvent::recvRequest( void ) const {
    char buf[MAX_RAW_REQUEST_SIZE];

    ssize_t read_size = recv(client_sd_, buf, BUF_SIZE - 1, 0);
    if (read_size < 0) {
        throw std::runtime_error("recv failed");
    }
    buf[read_size] = '\0';

    return (buf);
}

int RecieveRequestEvent::handler( void ) {
    // get request string
    // NOTE: possibly segv
    std::string raw_req;
    try {
        raw_req = recvRequest();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        server_.cacheResponse(
            client_sd_,
            HttpResponse::createErrorResponse(HttpResponse::Status::INTERNAL_SERVER_ERROR)
        );
        return (1);
    }

    // parse request
    HttpRequest req;
    if (req.parse(raw_req)) {
        server_.cacheResponse(
            client_sd_,
            HttpResponse::createErrorResponse(
                HttpResponse::Status::BAD_REQUEST
            )
        );
        return (1);
    }

    // もしrequestをcacheする場合、以下はSendResponseEvent->hander内に記述
    // host matching
    if (req.hostMatching(server_.getConf().getServersInfo())) {
        server_.cacheResponse(
            client_sd_,
            HttpResponse::createErrorResponse(
                HttpResponse::Status::NOT_FOUND
            )
        );
        return (1);
    }
    
    // location matching
    if (req.locationMatching()) {
        server_.cacheResponse(
            client_sd_,
            HttpResponse::createErrorResponse(
                HttpResponse::Status::NOT_FOUND,
                req.getHostInfo()
            )
        );
        return (1);
    }
    
    // req body size validation
    if (req.getBody().size() > req.getHostInfo().client_max_body_size) { // validate body size
        server_.cacheResponse(
            client_sd_,
            HttpResponse::createErrorResponse(
                HttpResponse::Status::PAYLOAD_TOO_LARGE
            )
        );
        return (1);
    }
    
    // unchunk
    if (req.isChunked()) {
        if (req.unchunk()) {
            server_.cacheResponse(
                client_sd_,
                HttpResponse::createErrorResponse(
                    HttpResponse::Status::BAD_REQUEST,
                    req.getHostInfo()
                )
            );
            return (1);
        }
    }
    
    // redirection
    if (req.getLocationInfo().return_path != "") {
        server_.cacheResponse(
            client_sd_,
            HttpResponse::createRedirectionResponse(
                HttpResponse::Status::SEE_OTHER,
                req.getLocationInfo().return_path
            )
        );
        return (0);
    }

    // execute method(like GET, POST...)
    HttpResponse resp;
    resp = req.getMethod()->execute(req);

    /* cache response */
    server_.cacheResponse(client_sd_, resp);
    std::cout << "==== RESPONSE ====" << std::endl;
    std::cout << resp.marshal() << std::endl;
    std::cout << "==================" << std::endl;

    return (0);
}
