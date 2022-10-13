#ifndef RECIEVE_REQUEST_EVENT_HPP
#define RECIEVE_REQUEST_EVENT_HPP

#include "Event.hpp"
#include "../io/HttpRequest.hpp"
#include <sys/types.h>
#include <sys/socket.h>

#define MAX_RAW_REQUEST_SIZE 4096

class Server;

class RecieveRequestEvent : public Event {
    public:
        RecieveRequestEvent( int client_sd, Server& server );
        int handler( void );

    private:
        std::string recvRequest( void ) const;
        int client_sd_;
        Server& server_;
};

#endif
