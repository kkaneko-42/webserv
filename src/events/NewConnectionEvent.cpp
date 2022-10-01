#include "NewConnectionEvent.hpp"
#include "../Server.hpp"

NewConnectionEvent::NewConnectionEvent( int listen_sd, Server& server )
: listen_sd_(listen_sd), server_(server) {}

int NewConnectionEvent::handler( void ) {
    while (true) {
        // clinet connectionをすべてaccept
        int new_sd = accept(listen_sd_, NULL, NULL);
        
        if (new_sd < 0) {
            // clientの待ち受けキューが空
            // つまりすべてのconnectionをacceptし終わった時
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break;
            } else {
                // error
                // TODO: 失敗時、どういう挙動？
                perror("accept");
                exit(EXIT_FAILURE);
            }
        }

        server_.addSocketDescriptor(new_sd, POLLIN);
    }

    return (0);
}
