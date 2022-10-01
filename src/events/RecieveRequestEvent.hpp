#ifndef RECIEVE_REQUEST_EVENT_HPP
#define RECIEVE_REQUEST_EVENT_HPP

#include "Event.hpp"
#include "../io/HttpRequest.hpp"
#include <sys/types.h>
#include <sys/socket.h>

#define CLOSED_FD -1

class RecieveRequestEvent : public Event {
    public:
        RecieveRequestEvent( int& client_sd );
        int handler( void );

    private:
        int& client_sd_;
};

#endif
