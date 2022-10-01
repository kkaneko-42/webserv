#ifndef NEW_CONNECTION_EVENT_HPP
#define NEW_CONNECTION_EVENT_HPP

#include "Event.hpp"
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>

class Server;

class NewConnectionEvent : public Event {
    public:
        NewConnectionEvent( int listen_sd, Server& server );
        int handler( void );

    private:
        int listen_sd_;
        Server& server_;
};

#endif
