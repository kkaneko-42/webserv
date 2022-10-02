#ifndef SEND_RESPONSE_EVENT
#define SEND_RESPONSE_EVENT

#include "Event.hpp"
#include "../io/HttpResponse.hpp"

class Server;

class SendResponseEvent : public Event {
    public:
        SendResponseEvent( int& client_sd, Server& server );
        int handler( void );

    private:
        int& client_sd_;
        Server& server_;
};

#endif
